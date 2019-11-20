#include "../include/processor.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

void processor_init()
{
    last_err_code = 0;
}

int process_exit()
{
    exit(last_err_code);
}

int ret_err(const char* command, const char* message)
{
    if(message)
        fprintf(stderr, "%s: %s\n", command, message);
    else
        fprintf(stderr, "%s: %s\n", command, strerror(errno));
    RETURN(1);
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
            return ret_err("cd", "too many arguments");
    }

    if (chdir(to_path) || 
        setenv("OLDPWD", &from_path[0], 1) ||
        !getcwd(from_path, buff_size) ||
        setenv("PWD", &from_path[0], 1)
    )
        return ret_err("cd", NULL);

    RETURN(0);
}

char** prepare_args(const entry_t* args)
{
    size_t size = list_size(args) + 2;
    char** arg_arr = (char**)malloc(size * sizeof(char*));
    
    if(!arg_arr)
        err(1,"processor");

    int tmp = 1;
    while(args) {
        arg_arr[tmp++] = args->value;
        args = args->next;
    }
    arg_arr[size-1] = NULL;

    return arg_arr; 
}

int process(const command_t* command)
{
    char* comm_begin = strrchr(command->comm, '/');
    
    char* program_name = comm_begin ? comm_begin + 1 : command->comm;
    int absolute = comm_begin ? 1 : 0;

    int pid = fork();

    if(pid == -1)
        ret_err(program_name, NULL);

    if(pid == 0) {
        char** args = prepare_args(command->args);
        args[0] = program_name;

        if(absolute)
            execv(command->comm, args);
        else
            execvp(command->comm, args);

        free(args);
        exit(ret_err(program_name, NULL));
    }

    int status;
    wait(&status);

    if (WIFEXITED(status))
        RETURN(WEXITSTATUS(status));
    if (WIFSIGNALED(status)) {
        int sig = WTERMSIG(status);
        fprintf(stderr, "Killed by signal %d", sig);
        RETURN(128 + sig);
    }
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
        return process(command);
}
