#include "../include/command.h"

#include <stdlib.h>

void command_free(command_t* command)
{
    free(command->comm);
    list_free(command->args);
}
