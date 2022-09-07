
D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS		(f|F)
IS		(l|L)

%{
#include <stdio.h>
static void count();
static void comment();
%}


%%
"/*"				{ comment(); }
"//"[^\n]*  { /* consume //-comment */ }
"char"			{ count(); printf("CHAR"); }
"cns"				{ count(); printf("CNS"); }
"elif"			{ count(); printf("ELIF"); }
"else"			{ count(); printf("ELSE"); }
"endif"			{ count(); printf("ENDIF");}
"float"			{ count(); printf("FLOAT"); }
"loop"			{ count(); printf("LOOP"); }
"if"				{ count(); printf("IF"); }
"int"				{ count(); printf("INT"); }
"Tensor"		{ count(); printf("TENSOR"); }
"var"				{ count(); printf("VAR"); }
"bool"			{ count(); printf("BOOL");}

"sizeof"		{ count(); printf("SIZEOF");}
"grad"			{ count(); printf("GRAD");}
"backward"	{ count(); printf("BACKWARD");}
"cos"				{ count(); printf("COS");}
"sin"				{ count(); printf("SIN");}
"exp"				{ count(); printf("EXP");}
"log"				{ count(); printf("LOG");}
"print" 		{ count(); printf("PRINT");}

{L}({L}|{D})*		  			{ count(); printf("IDENTIFIER");}
0[xX]{H}+{IS}?					{ count(); printf("CONSTANT"); }
0{D}+{IS}?							{ count(); printf("CONSTANT"); }
{D}+{IS}?								{ count(); printf("CONSTANT"); }
L?'(\\.|[^\\'])+'				{ count(); printf("CONSTANT"); }
{D}+{E}{FS}?						{ count(); printf("CONSTANT"); }
{D}*"."{D}+({E})?{FS}?	{ count(); printf("CONSTANT"); }
{D}+"."{D}*({E})?{FS}?	{ count(); printf("CONSTANT"); }
L?\"(\\.|[^\\"])*\"			{ count(); printf("STRING_LITERAL"); }

"@"				{ count(); printf("AT_OP"); }
">>="			{ count(); printf("RIGHT_ASSIGN"); }
"<<="			{ count(); printf("LEFT_ASSIGN"); }
"+="			{ count(); printf("ADD_ASSIGN"); }
"-="			{ count(); printf("SUB_ASSIGN"); }
"*="			{ count(); printf("MUL_ASSIGN"); }
"/="			{ count(); printf("DIV_ASSIGN"); }
"%="			{ count(); printf("MOD_ASSIGN"); }
"&="			{ count(); printf("AND_ASSIGN"); }
"^="			{ count(); printf("XOR_ASSIGN"); }
"|="			{ count(); printf("OR_ASSIGN"); }
"@="			{ count(); printf("AT_ASSIGN");}
">>"			{ count(); printf("RIGHT_OP"); }
"<<"			{ count(); printf("LEFT_OP"); }
"++"			{ count(); printf("INC_OP"); }
"--"			{ count(); printf("DEC_OP"); }
"->"			{ count(); printf("PTR_OP"); }
"&&"			{ count(); printf("AND_OP"); }
"||"			{ count(); printf("OR_OP"); }
"<="			{ count(); printf("LE_OP"); }
">="			{ count(); printf("GE_OP"); }
"=="			{ count(); printf("EQ_OP"); }
"!="			{ count(); printf("NE_OP"); }
";"			{ count(); printf(";"); }
"{"			{ count(); printf("{"); }
"}"			{ count(); printf("}"); }
","			{ count(); printf(","); }
":"			{ count(); printf(":"); }
"="			{ count(); printf("="); }
"("			{ count(); printf("("); }
")"			{ count(); printf(")"); }
"["			{ count(); printf("["); }
"]"			{ count(); printf("]"); }
"&"			{ count(); printf("&"); }
"!"			{ count(); printf("!"); }
"~"			{ count(); printf("~"); }
"-"			{ count(); printf("-"); }
"+"			{ count(); printf("+"); }
"*"			{ count(); printf("*"); }
"/"			{ count(); printf("/"); }
"%"			{ count(); printf("%%"); }
"<"			{ count(); printf("<"); }
">"			{ count(); printf(">"); }
"^"			{ count(); printf("^"); }
"|"			{ count(); printf("|"); }
"?"			{ count(); printf("?"); }
[\n]			{ printf("\n"); }
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

