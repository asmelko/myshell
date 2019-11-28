#ifndef SCANNER_H
#define SCANNER_H

#include "command.h"

//initialize lexer with one line of string
void scan_init(const char* input);

//scans input and populates command parameter
//returns  1 on syntax error
//returns  0 on end of input
//returns -1 when some input remains
int get_command(command_t* command);

//frees scaning resources
void scan_free();

#endif