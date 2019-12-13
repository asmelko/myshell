#ifndef LINE_H
#define LINE_H

#include <stddef.h>

//max length of line
#define MAXLINE 1000

//struct for line of input
typedef struct {
	char data[MAXLINE];
} line_t;

//checks length of line end exits if MAXLINE exceeded
void check_length(const char* data);
void check_length_s(size_t size);

//checks if line contains only spaces
int is_blank(const char* data);

#endif