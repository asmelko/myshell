#ifndef COMMAND_H
#define COMMAND_H

#include "string_list.h"

//struct for written command
typedef struct command_node {
    char* comm;                 // string of the command
    entry_t* args;              // linked list of arguments
    char* redir_in;             // input redirection
    char* redir_out;            // output redirection
    int append;                 // append flag for output redirection
    struct command_node* pipe;  // linked list to another command connected with pipe
} command_t;

//free the command
void command_free(command_t* command);

//initialize the command
void command_init(command_t* command);

#endif