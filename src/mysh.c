#include "../include/mysh.h"
#include "../include/scanner.h"
#include "../include/processor.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>

#include <readline/readline.h>
#include <readline/history.h>

int was_syntax_error;

void mysh_init()
{
    mysh_line_number = -1;
    processor_init();
}

void mysh_syntax_err()
{
    if(mysh_line_number == -1)
        fprintf(stderr, "syntax error\n");
    else
        fprintf(stderr, "line %d: syntax error\n", mysh_line_number);
}

int mysh_process_line(const line_t* line)
{
    scan_init(line->data);
    was_syntax_error = 0;

    int com;
    command_t command;
    
    while((com = get_command(&command)) == -1) {
        process_command(&command);
        command_free(&command);
    }
    
    scan_free();

    if(com == 1) {
        mysh_syntax_err();
        was_syntax_error = 1;
        RETURN(SYNTAX_ERR);
    }

    int ret = process_command(&command);
    command_free(&command);
    return ret;
}

int mysh_process_file(const char* name)
{
    int desc = open(name, O_RDONLY);

    if(desc < 0)
        err(1, "process_file");

    int return_value = 0;

    { // read file
        mysh_line_number = 1;
        char c = '\0';
        ssize_t b_read;
        line_t line;
        size_t line_size = 0;

        while ((b_read = read(desc, &c, sizeof(c))) > 0) {
            if (c == '\n') {
                line.data[line_size] = '\0';
                return_value = mysh_process_line(&line);
                ++mysh_line_number;
                line_size = 0;
            }
            else {
                line.data[line_size++] = c;
                check_length_s(line_size);
            }
            if (was_syntax_error)
                break;
        }

        if (b_read == -1)
            err(1, "process_file");
    }

    close(desc);

    return return_value;
}

void get_prompt(char* buff, size_t buff_size)
{
    buff[0] = '\0';
    strcat(buff, "mysh:");

    if (getcwd(buff + 5, buff_size) == NULL)
        buff[5] = '\0';

    strcat(buff, "$ ");
}

int mysh_process_input()
{
    char* data;
    line_t line;
    int return_value;

    size_t buff_size = 256;
    char prompt[buff_size];

    get_prompt(&prompt[0], buff_size);

    while ((data = readline(prompt))) {
        check_length(data);
        strcpy(line.data,data);
        free(data);
        
        return_value = mysh_process_line(&line);

        if (!is_blank(line.data))
            add_history(line.data);
            
        get_prompt(&prompt[0], buff_size);
    }

    return return_value;
}
