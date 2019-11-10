#include "../include/processor.h"

#include <stdio.h>

int process_command(const command_t* command)
{
    printf("%s\n", command->comm);
    return 1;
}