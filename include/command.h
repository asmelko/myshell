#ifndef COMMAND_H
#define COMMAND_H

#include "string_list.h"

typedef struct {
    char* comm;
    entry_t* args;
} command_t;

void command_free(command_t* command);

#endif