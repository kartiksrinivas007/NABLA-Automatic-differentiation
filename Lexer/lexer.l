%option noyywrap
D			[0-9]
L			[a-zA-Z_]
/* H			[a-fA-F0-9] */
E			[Ee][+-]?{D}+
/* FS			(f|F) */
/* IS			(l|L) */

%{
#include <stdio.h>
// #include "y.tab.h"

static void count();
static void comment();
void yyerror(char *);
void check_integer_overflow();
// extern void lyyerror(YYLTYPE t,char *s,...);
char linebuf[500];
char filename[50];
void warning(const char*);

int yycolumn =1;
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;\
				yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1;\
				yycolumn += yyleng;
%}

%option lex-compat

%%
"/*"						{ comment(); }              /* defined as comment */
"//"[^\n]*      			{ /* consume //-comment */ }            /* comment */
"cns"						{ count(); return CNS; }       /* cns is constant */
"elif"						{ count(); return ELIF; }           /* elif is just like else if in cpp */
"else"						{ count(); return ELSE; }           /* it's else statement */
"endif"						{ count(); return ENDIF;}            /* endif directive used to define "#if"  */
"float"						{ count(); return FLOAT; }           /* float datatype */
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
"declare"					{ count(); return DECLARE;}       /* declare is just like declare in cpp */
"operations"				{ count(); return OPERATIONS;}    /* operations is just like operations in cpp */
"gradient"                     { count(); return GRADIENT;}      /* gradient is just like gradient in cpp */

{L}({L}|{D})*						{ count(); return IDENTIFIER;}   /* D for digit H for hexadecimal E for exponential  L for letters*/
{D}+							{ yylval.ival = atoi(yytext) ; check_integer_overflow(); count(); return CONSTANT; }
L?'(\\.|[^\\'])+'					{ yylval.ival = atoi(yytext) ;count(); return CONSTANT; }
{D}+{E}						{ yylval.ival = atoi(yytext) ;count(); return CONSTANT; }
{D}*"."{D}+({E})?				{ yylval.fval = atof(yytext) ;count(); return CONSTANT; }
{D}+"."{D}*({E})?			{ yylval.fval = atof(yytext) ; count(); return CONSTANT; }
L?\"(\\.|[^\\"])*\"					{ count(); return STRING_LITERAL; }
\"[^\n\"]*[;,]?						{ count(); warning("unterminated string, misssing \" character"); return STRING_LITERAL; }

        /* Tensor multiply operator */
"+="				{ count(); return ADD_ASSIGN; }
"-="				{ count(); return SUB_ASSIGN; }
"*="				{ count(); return MUL_ASSIGN; }
"/="				{ count(); return DIV_ASSIGN; }
"%="				{ count(); return MOD_ASSIGN; }
"@="				{ count(); return AT_ASSIGN;}
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
"-"					{ count(); return '-'; }
"+"					{ count(); return '+'; }
"*"					{ count(); return '*'; }
"@"					{ count(); return '@'; }  
"/"					{ count(); return '/'; }
"%"					{ count(); return '%'; }
"<"					{ count(); return '<'; }
">"					{ count(); return '>'; }
\n.*						{ strcpy(linebuf,yytext+1); yyless(1); yycolumn=1;}
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

void check_integer_overflow()
{
    int is_neg = 0,l1,l2, overflow_detected = 0,n=0;
    char int_max[] = "2147483647", int_min[] = "-2147483648";
    if (yytext[0] == '-')
        is_neg =1;

    l1 = strlen(yytext);
    if(is_neg)
        l2 = strlen(int_min);
    else 
        l2 = strlen(int_max);


    // If number is negative
    if(is_neg)
    {            
        if (l2 < l1)
            overflow_detected = 1;
        else if(l2 == l1)
        {
            for (n = 1; n < l1; n++) {
                if (int_min[n] < yytext[n])
                    overflow_detected =1;
            }
        }
    }

    if (l1 > l2)
        overflow_detected = 1;
    else if (l2 == l1)
    {
        for (n = 0; n < l1; n++) 
        {
            if (yytext[n] > int_max[n])
                overflow_detected =1;
        }
    }

    if(overflow_detected){
        warning("Integer Overflow, number will not fit into 32-bit integer constant");
    }

}


int main(int argc,char** argv)
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
}

