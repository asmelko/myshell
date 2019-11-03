#include "../include/line.h"

#include <string.h>

int check_length(line_t* line)
{
    return strlen(line->data) == MAXLINE - 1;
}
