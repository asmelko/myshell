#include "../include/processor.h"
#include <stdio.h>

void processor_init()
{
    last_err_code = 0;
}

int process_command(const command_t* command)
{
    if(!command->comm)
        return last_err_code;

    printf("%s\n", command->comm);
    return 1;
}