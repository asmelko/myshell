#include "../include/command.h"

#include <stdlib.h>

void command_free(command_t* command)
{
    command_t* next = command;

    while(next){
        command_t* tmp = next->pipe;
        free(next->comm);
        free(next->redir_in);
        free(next->redir_out);
        list_free(next->args);
        if (next != command)
            free(next);
        next = tmp;
    }
}

void command_init(command_t* command)
{
    command->comm = NULL;
    command->args = NULL;
    command->redir_in = NULL;
    command->redir_out = NULL;
    command->pipe = NULL;
}