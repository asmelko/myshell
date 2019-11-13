#ifndef LINE_H
#define LINE_H

#include <stddef.h>

#define MAXLINE 1000

typedef struct {
    char data[MAXLINE];
} line_t;

void check_length(const char* data);
void check_length_s(size_t size);

int is_blank(const char* data);

#endif