#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "command.h"

#define RETURN(x) return last_return_value = x;

//value returned by previous command
int last_return_value;

//initialization function
void processor_init();

//entry function for evaluation of commands
int process_command(const command_t* command);

#endif