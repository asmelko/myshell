#ifndef MYSH_H
#define MYSH_H

#include "line.h"

#define SYNTAX_ERR 2

int mysh_line_number;

void mysh_init();

int mysh_process_line(const line_t* line);
int mysh_process_file(const char* file);
int mysh_process_input();

#endif