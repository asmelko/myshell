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


int mysh_line_number = -1;

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

    int com;
    command_t command;
    
    while((com = get_command(&command)) == -1) 
    {
        process_command(&command);
        command_free(&command);
    }
    
    scan_free();

    if(com == 1)
    {
        mysh_syntax_err();
        return SYNTAX_ERR;
    }

    return process_command(&command);
}

int mysh_process_file(const char* name)
{
    int desc = open(name, O_RDONLY);

    if(desc < 0)
        err(1, "could not open file");

    int return_value = 0;

    {
        mysh_line_number = 1;
        char c = '\0';
        ssize_t b_read;
        line_t line;
        size_t line_size = 0;

        while ((b_read = read(desc, &c, sizeof(c))) > 0)
        {
            if(c == '\n') {
                line.data[line_size] = '\0';
                return_value = mysh_process_line(&line);
                ++mysh_line_number;
            }
            else {
                line.data[line_size++] = c;
                check_length(line.data);
            }
        }

        if (b_read == -1)
            err(1, "file read problem");
    }

    close(desc);

    return return_value;
}

int mysh_process_input()
{
    char* data;
    line_t line;
    int return_value;

    while((data = readline("mysh$ "))) 
    {
        check_length(data);
        strcpy(line.data,data);
        free(data);
        
        return_value = mysh_process_line(&line);

        if(!is_blank(line.data))
            add_history(line.data);
    }

    return return_value;
}
