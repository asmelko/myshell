#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "command.h"

int last_err_code;

void processor_init();

int process_command(const command_t* command);

#endif