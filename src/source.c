#include "../include/mysh.h"

#include <getopt.h>
#include <string.h>
#include <err.h>

int main(int argc, char **argv)
{
    int opt, arg_present = 0;
    line_t line;

    while ((opt = getopt(argc, argv, "c:")) != -1) {
        switch (opt) {
        case 'c':
            strncpy(line.data, optarg, MAXLINE);
            line.data[MAXLINE - 1] = '\0';
            arg_present = 1;
            break;
        }
    }

    if(arg_present) {
        if(check_length(&line))
            errx(125,"line length exceeded");
    }
    else {

    }

    return 0;
}
