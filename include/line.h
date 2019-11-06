#ifndef LINE_H
#define LINE_H

#define MAXLINE 1000

typedef struct {
    char data[MAXLINE];
} line_t;

int check_length(const line_t* line);

#endif
