#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "command.h"

#define RETURN(x) last_err_code = x; return x;

int last_err_code;

void processor_init();

int process_command(const command_t* command);

#endif