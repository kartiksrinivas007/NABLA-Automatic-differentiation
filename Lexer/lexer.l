D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F)
IS			(l|L)

%{
#include <stdio.h>
#include "y.tab.h"

static void count();
static void comment();
void yyerror(char *);
%}


%%
"/*"						{ comment(); }              /* defined as comment */
"//"[^\n]*      			{ /* consume //-comment */ }            /* comment */
"char"						{ count(); return CHAR; }         /* lexer will print CHAR */
"cns"						{ count(); return CNS; }       /* cns is constant */
"elif"						{ count(); return ELIF; }           /* elif is just like else if in cpp */
"else"						{ count(); return ELSE; }           /* it's else statement */
"endif"						{ count(); return ENDIF;}            /* endif directive used to define "#if"  */
"float"						{ count(); return FLOAT; }           /* float datatype */
"loop"						{ count(); return LOOP; }            /* loop is just like for loop in cpp*/
"if"						{ count(); return IF; }          /* if */
"int"						{ count(); return INT; }         /* int datatype */
"Tensor"					{ count(); return TENSOR; }          /* tensor datatype */
"var"						{ count(); return VAR; }    /* var is for variable */
"bool"						{ count(); return BOOL;}          /* it is boolean */
"sizeof"					{ count(); return SIZEOF;}        /* sizeof function */
"grad"						{ count(); return GRAD;}          /* gradiance */
"backward"					{ count(); return BACKWARD;}
"cos"						{ count(); return COS;}       /* cos function as keyword*/
"sin"						{ count(); return SIN;}       /* sin function as keyword */
"exp"						{ count(); return EXP;}       /* exponential function as keyword */
"log"						{ count(); return LOG;}       /* logarithmic function as keyword */
"print" 					{ count(); return PRINT;}         /* print is just like print in python */

{L}({L}|{D})*						{ count(); return IDENTIFIER;}   /* D for digit H for hexadecimal E for exponential  L for letters*/
0[xX]{H}+{IS}?						{ yylval.ival = atoi(yytext) ; count(); return CONSTANT; }        /* ? is when it is optional and star is when you can take it any times */
0{D}+{IS}?							{ yylval.ival = atoi(yytext) ;count(); return CONSTANT; }
{D}+{IS}?							{ yylval.ival = atoi(yytext) ;count(); return CONSTANT; }
L?'(\\.|[^\\'])+'					{ yylval.ival = atoi(yytext) ;count(); return CONSTANT; }
{D}+{E}{FS}?						{ yylval.ival = atoi(yytext) ;count(); return CONSTANT; }
{D}*"."{D}+({E})?{FS}?				{ yylval.fval = atof(yytext) ;count(); return CONSTANT; }
{D}+"."{D}*({E})?{FS}?				{ yylval.fval = atof(yytext) ; count(); return CONSTANT; }
L?\"(\\.|[^\\"])*\"					{ count(); return STRING_LITERAL; }

        /* Tensor multiply operator */
">>="				{ count(); return RIGHT_ASSIGN; }   /* below all operators like cpp */
"<<="				{ count(); return LEFT_ASSIGN; }     /* assign operators from line 53 to 63 */
"+="				{ count(); return ADD_ASSIGN; }
"-="				{ count(); return SUB_ASSIGN; }
"*="				{ count(); return MUL_ASSIGN; }
"/="				{ count(); return DIV_ASSIGN; }
"%="				{ count(); return MOD_ASSIGN; }
"&="				{ count(); return AND_ASSIGN; }
"^="				{ count(); return XOR_ASSIGN; }
"|="				{ count(); return OR_ASSIGN; }
"@="				{ count(); return AT_ASSIGN;}
">>"				{ count(); return RIGHT_OP; }
"<<"				{ count(); return LEFT_OP; }
"++"				{ count(); return INC_OP; }
"--"				{ count(); return DEC_OP; }
"&&"				{ count(); return AND_OP; }
"||"				{ count(); return OR_OP; }
"<="				{ count(); return LE_OP; }
">="				{ count(); return GE_OP; }
"=="				{ count(); return EQ_OP; }
"!="				{ count(); return NE_OP; }
";"					{ count(); return ';'; }     /* some other operators from 74 to 96 */
"{"					{ count(); return '{'; }
"}"					{ count(); return '}'; }
","					{ count(); return ','; }
":"					{ count(); return ':'; }
"="					{ count(); return '='; }
"("					{ count(); return '('; }
")"					{ count(); return ')'; }
"["					{ count(); return '['; }
"]"					{ count(); return ']'; }
"&"					{ count(); return '&'; }    
"!"					{ count(); return '!'; }
"~"					{ count(); return '~'; }    /* bitwise operator */
"-"					{ count(); return '-'; }
"+"					{ count(); return '+'; }
"*"					{ count(); return '*'; }
"@"					{ count(); return '@'; }  
"/"					{ count(); return '/'; }
"%"					{ count(); return '%'; }
"<"					{ count(); return '<'; }
">"					{ count(); return '>'; }
"^"					{ count(); return '^'; }
"|"					{ count(); return '|'; }
"?"					{ count(); return '?'; }
[\n]				{	/*count();*/ }
[ \t\v\f]		{ /*count(); eat these chars*/ }
.						{ /* ignore bad characters */ }

%%

int yywrap()
{
	return(1);
}


static void
comment() {
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

	// ECHO;
}


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

