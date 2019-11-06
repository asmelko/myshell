
enum yytokentype 
{
	SEMICOLON = 258,
	WORD = 259
};

char* yylval;

void set_input_string(const char* in);

void end_lexical_scan(void);

int yylex();