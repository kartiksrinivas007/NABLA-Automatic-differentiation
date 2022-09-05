D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{
#include <stdio.h>
#include "y.tab.h"

static void count();
static void comment();
void yyerror(char *);
%}


%%
"/*"			{ comment(); }
"//"[^\n]*      { /* consume //-comment */ }
"char"			{ count(); return CHAR; }
"cns"			{ count(); return CNS; }
"elif"			{ count(); return ELIF; }
"else"			{ count(); return ELSE; }
"endif"			{count(); return ENDIF;}
"float"			{ count(); return FLOAT; }
"loop"			{ count(); return LOOP; }
"if"			{ count(); return IF; }
"int"			{ count(); return INT; }
"Tensor"		{ count(); return TENSOR; }
"var"			{ count(); printf("Hello here\n"); return VAR; }
"bool"			{ count(); return BOOL;}

"sizeof"		{ count(); return SIZEOF;}
"grad"			{ count(); return GRAD;}
"backward"		{ count(); return BACKWARD;}
"cos"			{ count(); return COS;}
"sin"			{ count(); return SIN;}
"exp"			{count(); return EXP;}
"log"			{count(); return LOG;}
"print" 		{count(); return PRINT;}
{L}({L}|{D})*		{ count(); return IDENTIFIER;}
0[xX]{H}+{IS}?		{ count(); return CONSTANT; }
0{D}+{IS}?		{ count(); return CONSTANT; }
{D}+{IS}?		{ count(); return CONSTANT; }
L?'(\\.|[^\\'])+'	{ count(); return CONSTANT; }
{D}+{E}{FS}?		{ count(); return CONSTANT; }
{D}*"."{D}+({E})?{FS}?	{ count(); return CONSTANT; }
{D}+"."{D}*({E})?{FS}?	{ count(); return CONSTANT; }
L?\"(\\.|[^\\"])*\"	{ count(); return STRING_LITERAL; }

"@"			{ count(); return AT_OP; }
">>="			{ count(); return RIGHT_ASSIGN; }
"<<="			{ count(); return LEFT_ASSIGN; }
"+="			{ count(); return ADD_ASSIGN; }
"-="			{ count(); return SUB_ASSIGN; }
"*="			{ count(); return MUL_ASSIGN; }
"/="			{ count(); return DIV_ASSIGN; }
"%="			{ count(); return MOD_ASSIGN; }
"&="			{ count(); return AND_ASSIGN; }
"^="			{ count(); return XOR_ASSIGN; }
"|="			{ count(); return OR_ASSIGN; }
"@="			{ count(); return AT_ASSIGN;}
">>"			{ count(); return RIGHT_OP; }
"<<"			{ count(); return LEFT_OP; }
"++"			{ count(); return INC_OP; }
"--"			{ count(); return DEC_OP; }
"&&"			{ count(); return AND_OP; }
"||"			{ count(); return OR_OP; }
"<="			{ count(); return LE_OP; }
">="			{ count(); return GE_OP; }
"=="			{ count(); return EQ_OP; }
"!="			{ count(); return NE_OP; }
";"			{ count(); return ';'; }
"{"			{ count(); return '{'; }
"}"			{ count(); return '}'; }
","			{ count(); return ','; }
":"			{ count(); return ':'; }
"="			{ count(); return '='; }
"("			{ count(); return '('; }
")"			{ count(); return ')'; }
"["			{ count(); return '['; }
"]"			{ count(); return ']'; }
"."			{ count(); return '.'; }
"&"			{ count(); return '&'; }
"!"			{ count(); return '!'; }
"~"			{ count(); return '~'; }
"-"			{ count(); return '-'; }
"+"			{ count(); return '+'; }
"*"			{ count(); return '*'; }
"/"			{ count(); return '/'; }
"%"			{ count(); return '%'; }
"<"			{ count(); return '<'; }
">"			{ count(); return '>'; }
"^"			{ count(); return '^'; }
"|"			{ count(); return '|'; }
"?"			{ count(); return '?'; }
[\n]			{ /*Nothing bro*/}
[ \t\v\f]		{ /*count(); eat these chars*/ }
.			{ /* ignore bad characters */ }

%%

int yywrap()
{
	return(1);
}


static void
comment() {
	char c, c1 = 0;
	while ( (c = input()) != 0 ) {	/* (EOF maps to 0) */
		if ( c == '/' && c1 == '*' )
			return;
		c1 = c;
	}
	printf("Error::Unclosed comment of type */ \n");
}


int column = 0;

void count()
{
	printf(" ");
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;

	// ECHO;
}


// int check_type()
// {
// /*
// * pseudo code --- this is what it should check
// *
// *	if (yytext == type_name)
// *		return(TYPE_NAME);
// *
// *	return(IDENTIFIER);
// */

// /*
// *	it actually will only return IDENTIFIER
// */

// 	return(IDENTIFIER);
// }


/* int main(int argc,char** argv)
{
	if( argc >=2 ){
		yyin = fopen(argv[1] , "r");
		if(yyin ==NULL){
			printf("File not found\n");
			return 1;
		}
	}
	else{
		printf("Use case : ./p1.out \"filename.txt\" \n");
		return 1;
	}
	yylex();
	return 0;
} */

