/* simplest version of calculator */
%{
#include <stdio.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
%}

/* declare tokens */
%token EOL
%token WORD
%token SLASH
%token SEMICOLON

%%
calclist: /* nothing */ 
| calclist exp EOL { printf("= %d\n", $1); } 
;
exp: factor 
| exp ADD factor { $$ = $1 + $3; }
| exp SUB factor { $$ = $1 - $3; }
;
factor: term 
| factor MUL term { $$ = $1 * $3; }
| factor DIV term { $$ = $1 / $3; }
;
term: NUMBER 
| ABS term { $$ = $2 >= 0? $2 : - $2; }
;
%%

void set_input_string(const char* in);
void end_lexical_scan(void);

int parse_string(const char* in) {
  set_input_string(in);
  int ret = yyparse();
  end_lexical_scan();
  return ret;
}

int yyerror(char *s)
{
    return fprintf(stderr, "error: %s\n", s);
}
#pragma GCC diagnostic pop
