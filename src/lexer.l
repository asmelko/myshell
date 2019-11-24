%{
#include <string.h>
#include <err.h>
#include <assert.h>

#include "../include/scanner.h"

enum yytokentype 
{
	SEMICOLON = 258,
    LT = 259,
    GT = 300,
    DGT = 301,
    PIPE = 302,
	WORD = 303
};

char* yylval;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
%}

%x COMMENT

%%

";"								return SEMICOLON;
"<"                             return LT;
">"                             return GT;
">>"                            return DGT;
"|"                             return PIPE;
"#" 							BEGIN(COMMENT);

<COMMENT>
{
	[^\n]*						/* ignore */
	\n							BEGIN(INITIAL);
    <<EOF>>                     BEGIN(INITIAL);
}

[ \r\t\f]+						/* whitespace */

\n								;

[^  \r\t\f;#\n<>|][^  \r\t\f;\n<>|]*    { yylval = strdup(yytext); if(!yylval) err(1,"scanner"); return WORD;}

%%

void scan_init(const char* input)
{
	yy_scan_string(input);
}

void add_redir(command_t* command, int redir)
{
    switch (redir)
    {
    case LT:
        if (command->redir_in)
            free(command->redir_in);
        command->redir_in = yylval;
        break;
    case GT:
    case DGT:
        if (command->redir_out)
            free(command->redir_out);
        command->redir_out = yylval;
        break;
    default:
        assert(0);
        break;
    }
    command->append = redir == DGT;
}

void make_pipe(command_t* command)
{
    command->pipe = (command_t*)malloc(sizeof(command_t));
    if (!command->pipe)
        err(1, "scanner");
    command_init(command->pipe);
}

int get_command(command_t* command)
{
   command_init(command);

    int token, redir = 0;
    entry_t** args_last = &command->args;
    command_t** comm_last = &command;


    while((token = yylex())) {
        if(redir && token != WORD)
            return 1;

        if(token == WORD) {
            if(redir) {
                add_redir(*comm_last, redir);
                redir = 0;
            }
            else if(!(*comm_last)->comm) 
                (*comm_last)->comm = yylval;
            else {
                *args_last = list_create(yylval);
                args_last = &(*args_last)->next;
            }
        }
        else if (token == PIPE) {
            make_pipe(*comm_last);
            comm_last = &(*comm_last)->pipe;
        }
        else if (token == LT || token == GT || token == DGT)
            redir = token;
        else if (token == SEMICOLON) 
            return (*comm_last)->comm == NULL ? 1 : -1;
    }

    return redir ? 1 : 0;
}

void scan_free()
{
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

#pragma GCC diagnostic pop
