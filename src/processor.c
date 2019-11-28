#include "../include/processor.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

void processor_init()
{
    last_return_value = 0;
}

void process_exit()
{
    exit(last_return_value);
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
            printf("%s\n", to_path);
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

    return 0;
}

char** prepare_args(const entry_t* args)
{
    size_t size = list_size(args) + 2;
    char** arg_arr = (char**)malloc(size * sizeof(char*));
    
    if(!arg_arr)
        err(1, "processor");

    int tmp = 1;
    while (args) {
        arg_arr[tmp++] = args->value;
        args = args->next;
    }
    arg_arr[size-1] = NULL;

    return arg_arr; 
}

void process_redirect(const command_t* command)
{
    if (command->redir_in) {
        int fd = open(command->redir_in, O_RDONLY);

        if (fd == -1)
            err(1, "%s", command->redir_in);

        if (close(0) || dup(fd) == -1 || close(fd))
            err(1 , "process");
    }

    if (command->redir_out) {
        int fd = command->append ? 
            open(command->redir_out, O_WRONLY | O_CREAT | O_APPEND, 0666) : 
            open(command->redir_out, O_WRONLY | O_CREAT, 0666);

        if (fd == -1)
            err(1, "%s", command->redir_out);

        if (close(1) || dup(fd) == -1 || close(fd))
            err(1 , "process");
    }
}

void free_pipes(int* pipes, size_t pipe_count)
{
    for (size_t i = 0; i < pipe_count * 2; ++i)
        if(close(pipes[i]))
            err(1, "process");

    free(pipes);
}

//redirect input and output with respect to pipes
void process_pipes(const command_t* command, int* pipes, size_t pipe_count, size_t idx)
{
    if (!pipes)
        return;

    if (idx) //command in the begin
        if (close(0) || dup(pipes[2 * (idx - 1)]) == -1)
            err(1, "process");

    if (command->pipe) //command in the end
        if (close(1) || dup(pipes[2 * idx + 1]) == -1)
            err(1, "process");

    free_pipes(pipes, pipe_count);

    return;
}

//process one command
int process(const command_t* command, int* pipes, size_t pipe_count, size_t idx)
{
    int pid = fork();

    if (pid == -1)
        exit(ret_err("processor", NULL));

    if (pid)
        return pid;
    
    process_pipes(command, pipes, pipe_count, idx);
    process_redirect(command);

    //process internal commands in piped statement
    if (!strcmp(command->comm, "exit"))
        exit(0);
    else if (!strcmp(command->comm, "cd"))
        exit(process_cd(command));

    char* comm_begin = strrchr(command->comm, '/');
    
    char* program_name = comm_begin ? comm_begin + 1 : command->comm;
    int absolute = comm_begin ? 1 : 0;

    char** args = prepare_args(command->args);
    args[0] = program_name;

    if(absolute)
        execv(command->comm, args);
    else
        execvp(command->comm, args);

    if (errno == ENOENT)
        err(127, "%s", program_name);
    exit(ret_err(program_name, NULL));
}

//create pipes for whole statement
size_t create_pipes(const command_t* command, int** pipes)
{
    size_t pipe_count = 0;
    for (command_t* next = command->pipe; next; next = next->pipe, ++pipe_count);

    if (!pipe_count) { // no pipes there
        *pipes = NULL;
        return pipe_count;
    }

    *pipes = (int*)malloc(2 * pipe_count * sizeof(int));

    if (!*pipes)
        err(1, "processor");

    for (size_t i = 0; i < pipe_count; ++i) // populate pipe array
        if (pipe(&(*pipes)[i * 2]))
            err(1, "processor");

    return pipe_count;
}

int process_command(const command_t* command)
{
    int* pipes;
    size_t pipe_count = create_pipes(command, &pipes);
    int return_value;

    if (pipe_count == 0) { // performs internal commands in the current process if no pipes 
        if (!strcmp(command->comm, "exit"))
            process_exit();
        else if (!strcmp(command->comm, "cd"))
            RETURN(process_cd(command));
    }

    int* process_pids = (int*)malloc((pipe_count + 1) * sizeof(int));
    if (!process_pids)
        err(1, "processor");

    const command_t* next = command;
    for (size_t i = 0; i < pipe_count + 1; ++i, next = next->pipe) // commands-in-pipe process loop
        process_pids[i] = process(next, pipes, pipe_count, i);

    free_pipes(pipes, pipe_count);

    for (size_t i = 0; i < pipe_count + 1; ++i) { // commands-in-pipe wait loop
        int status;
        waitpid(process_pids[i], &status, 0);

        if (WIFEXITED(status))
            return_value = WEXITSTATUS(status);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            fprintf(stderr, "Killed by signal %d\n", sig);
            return_value = 128 + sig;
        }
    }

    free(process_pids);
    
    RETURN(return_value);
}
