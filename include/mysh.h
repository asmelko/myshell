#ifndef MYSH_H
#define MYSH_H

#include "line.h"

#define SYNTAX_ERR 2

//current line nuber of processed file
int mysh_line_number;

//initialization function
void mysh_init();

//process one line of input
int mysh_process_line(const line_t* line);

//process whole file
int mysh_process_file(const char* file);

//process stdin
int mysh_process_input();

#endif