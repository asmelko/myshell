#include "../include/line.h"

#include <string.h>
#include <err.h>

void check_length(const char* data)
{
    if (strlen(data) >= MAXLINE)
        errx(125, "line length %d exceeded", MAXLINE - 1);
}

int is_blank(const char* data)
{
    for (size_t i = 0; data[i]!='\0'; ++i)
    {
        if(data[i] != ' ')
            return 0;
    }
    return 1;
}