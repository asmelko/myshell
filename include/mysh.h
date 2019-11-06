#ifndef MYSH_H
#define MYSH_H

#include "line.h"
#include "lexer.h"

int process_line(const line_t* line);

int get_input();

#endif
