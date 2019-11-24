#include "../include/command.h"

#include <stdlib.h>

void command_free(command_t* command)
{
    command_t* next = command;

    while(next){
        free(command->comm);
        free(command->redir_in);
        free(command->redir_out);
        list_free(command->args);
        next = next->pipe;
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