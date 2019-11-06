/* recognize tokens for the calculator and print them out */
%{
#include <string.h>
#include <stdlib.h>

#include "../include/lexer.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
%}

%x COMMENT

%%

";"								return SEMICOLON;
"#" 							BEGIN(COMMENT);

<COMMENT>
{
	[^\n]*						/* ignore */
	\n							BEGIN(INITIAL);
}

[ \r\t\f]+						/* whitespace */

\n								;

[^  \r\t\f;#\n]+				{ yylval = strdup(yytext); return WORD;}

%%

void set_input_string(const char* in) 
{
  yy_scan_string(in);
}

void end_lexical_scan(void) 
{
  yy_delete_buffer(YY_CURRENT_BUFFER);
}

#pragma GCC diagnostic pop
