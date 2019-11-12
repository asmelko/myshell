#include "../include/mysh.h"

#include <getopt.h>
#include <string.h>
#include <err.h>
#include <assert.h>

int main(int argc, char **argv)
{
    int opt, arg_present = 0;
    line_t line;

    while ((opt = getopt(argc, argv, "c:")) != -1) {
        switch (opt) {
        case 'c':
            check_length(optarg);
            strcpy(line.data, optarg);
            arg_present = 1;
            break;
        }
    }

    if (argc > optind)
        if(arg_present || argc - optind > 1)
            errx(1,"too many arguments");

    mysh_init();

    if (arg_present)
        return mysh_process_line(&line);

    if (argc > optind) {
        assert(argc - optind == 1);
        return mysh_process_file(argv[optind]);
    }

    return mysh_process_input();
}
