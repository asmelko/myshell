#ifndef COMMAND_H
#define COMMAND_H

#include "string_list.h"

typedef struct command_node {
    char* comm;
    entry_t* args;
    char* redir_in;
    char* redir_out;
    int append;
    struct command_node* pipe;
} command_t;

void command_free(command_t* command);

void command_init(command_t* command);

#endif