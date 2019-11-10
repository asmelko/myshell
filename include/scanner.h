#ifndef SCANNER_H
#define SCANNER_H

#include "command.h"

void scan_init(const char* input);

int get_command(command_t* command);

void scan_free();

#endif