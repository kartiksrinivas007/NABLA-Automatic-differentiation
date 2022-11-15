/* %option noyywrap */
D			[0-9]
L			[a-zA-Z_]
E			[Ee][+-]?{D}+

%{
#include <stdio.h>

#include "error.h"
#include "grammar.tab.h"

static void count();
static void comment();
extern void yyerror(const char *);
extern void warning(const char *);
extern void check_integer_overflow(const char *);
char linebuf[500];
char filename[50];
int yycolumn =1;

#define xels(s) yylval.string = strdup(#s); return s
#define xelc(c) yylval.string = strdup(yytext); return c

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;\
				yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1;\
				yycolumn += yyleng;
%}

%option lex-compat

%%
"/*"						{ comment();}            
"//"[^\n]*      			{}                                          /* consume comment */
"cns"						{ count(); xels(CNS) ;        }                    /* cns is constant */
"float"						{ count(); xels(FLOAT) ;      }          
"int"						{ count(); xels(INT) ;        }        
"Tensor"					{ count(); xels(TENSOR) ;     }         
"var"						{ count(); xels(VAR) ;        }    
"bool"						{ count(); xels(BOOL) ;       }          
"grad"						{ count(); xels(GRAD) ;       }          
"backward"					{ count(); xels(BACKWARD) ;   }
"cos"						{ count(); xels(COS) ;        }       
"sin"						{ count(); xels(SIN) ;        }       
"exp"						{ count(); xels(EXP) ;        }       
"log"						{ count(); xels(LOG) ;        }       
"declare"					{ count(); xels(DECLARE) ;    }       
"operations"				{ count(); xels(OPERATIONS) ; }    
"gradient"                  { count(); xels(GRADIENT) ;   }      

{L}({L}|{D})*				{ count(); xels(IDENTIFIER) ; }
{D}+						{ check_integer_overflow(yytext); xels(INT_CONST) ;   }
{D}+{E}						{ count(); xels(FLOAT_CONST) ;    }
{D}*"."{D}+({E})?			{ count(); xels(FLOAT_CONST) ;    }
{D}+"."{D}*({E})?			{ count(); xels(FLOAT_CONST) ;    }

        /* Tensor multiply operator */
"+="				{ count(); xels(ADD_ASSIGN) ; }
"-="				{ count(); xels(SUB_ASSIGN) ; }
"*="				{ count(); xels(MUL_ASSIGN) ; }
"/="				{ count(); xels(DIV_ASSIGN) ; }
"@="				{ count(); xels(AT_ASSIGN) ;  }
";"					{ count(); xelc(';') ;}    
"{"					{ count(); xelc('{') ;}
"}"					{ count(); xelc('}') ;}
","					{ count(); xelc(',') ;}
":"					{ count(); xelc(':') ;}
"="					{ count(); xelc('=') ;}
"("					{ count(); xelc('(') ;}
")"					{ count(); xelc(')') ;}
"["					{ count(); xelc('[') ;}
"]"					{ count(); xelc(']') ;}
"-"					{ count(); xelc('-') ;}
"+"					{ count(); xelc('+') ;}
"*"					{ count(); xelc('*') ;}
"@"					{ count(); xelc('@') ;}  
"/"					{ count(); xelc('/') ;}
"%"					{ count(); xelc('%') ;}
"<"					{ count(); xelc('<') ;}
">"					{ count(); xelc('>') ;}
\n.*				{ strcpy(linebuf,yytext+1); yyless(1); yycolumn=1;}
[ \t\v\f]		    { }                                                     /* consume whitespace */
.					{ }                                                     /* ignore other characters */

%%

int yywrap()
{
	return(1);
}


static void comment() {
	char c, c1 = 0;
	while ( (c = yyinput()) != 0 ) {	/* (EOF maps to 0) */
		if ( c == '/' && c1 == '*' )
			return;
		c1 = c;
	}
	printf("Error::Unclosed comment of type */ \n");
}


int column = 0;

void count()
{
	yylval.string = strdup(yytext);
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;


}