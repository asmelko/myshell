#ifndef LINE_H
#define LINE_H

#define MAXLINE 1000

typedef struct {
    char data[MAXLINE];
} line_t;

void check_length(const char* data);

int is_blank(const char* data);

#endif