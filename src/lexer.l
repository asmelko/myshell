%{
#include <string.h>

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

[^  \r\t\f;#\n][^  \r\t\f;\n]*  { yylval = strdup(yytext); return WORD;}

%%

void scan_init(const char* input)
{
	yy_scan_string(input);
}

int get_command(command_t* command)
{
   command_init(command);

    int token;
    entry_t** args_last = &command->args;

    while((token = yylex())) {
        if(token == WORD) {
            if(!command->comm) 
                command->comm = yylval;
            else {
                *args_last = list_create(yylval);
                args_last = &(*args_last)->next;
            }
        }
        else if (token == SEMICOLON) 
            return command->comm == NULL ? 1 : -1;
    }

    return 0;
}

void scan_free()
{
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

#pragma GCC diagnostic pop
