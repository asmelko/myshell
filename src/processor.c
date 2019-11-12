#include "../include/processor.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>

void processor_init()
{
    last_err_code = 0;
}

int process_exit()
{
    return last_err_code;
}

int ret_err(const char* command, const char* message)
{
    if(message)
        fprintf(stderr, "%s: %s\n", command, message);
    else
        fprintf(stderr, "%s: %s\n", command, strerror(errno));
    return 1;
}

int process_cd(const command_t* command)
{
    size_t buff_size = 257;
    char from_path[buff_size];
    char* to_path;

    if (!getcwd(from_path, buff_size))
        return ret_err("cd", NULL); 

    if (!command->args || !strcmp(command->args->value, "~")) { //home
        to_path = getenv("HOME");
        if(to_path == NULL)
            return ret_err("cd", "HOME not set");
    }
    else {
        to_path = command->args->value;

        if (!strcmp(to_path, "-")) { //prev
            to_path = getenv("OLDPWD");
            if(!to_path)
                return ret_err("cd", "OLDPWD not set");
        }

        if(command->args->next) //more than 1 arg
            return ret_err("cd", "Too many arguments");
    }

    if (chdir(to_path) || 
        setenv("OLDPWD", &from_path[0], 1) ||
        !getcwd(from_path, buff_size) ||
        setenv("PWD", &from_path[0], 1)
    )
        return ret_err("cd", NULL);

    return 0;
}

int process_rest(const command_t* command)
{
    (void)(command);

    return 0;
}

int process_command(const command_t* command)
{
    if(!command->comm)
        return last_err_code;

    if(!strcmp(command->comm, "exit"))
        return process_exit();
    else if (!strcmp(command->comm, "cd"))
        return process_cd(command);
    else 
        return process_rest(command);
}