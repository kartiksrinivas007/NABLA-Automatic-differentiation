%{
#include<stdio.h>
#include<stdlib.h>

extern int yylex();
%}

%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF GRAD COS SIN EXP LOG BACKWARD 
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP AT_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN AT_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME
%token CHAR SHORT INT LONG TENSOR FLOAT CNS VAR
%token IF ELIF ELSE LOOP ENDIF 
%start start
%%

start : compound_statement;

compound_statement
	: '{' '}'
	| '{' declaration_list statement_list '}'
	| '{' statement_list '}'
	| '{' declaration_list '}'
	;

declaration_list 
	: declaration_list declaration
	| declaration
	;

statement_list 
	: statement
	| statement_list statement
	;

statement 
	: compound_statement
	| iteration_statement
	| selection_statement
	| expression_statement
	;

expression_statement 
	: ';'
	| expression ';'
	;

iteration_statement 
	: LOOP '(' expression_statement expression_statement expression ')' statement
	;
selection_statement 
	: if_section endif_section
	| if_section else_section endif_section
	| if_section elif_section endif_section
	| if_section elif_section else_section endif_section
 	;
if_section 
	: IF '(' expression ')' statement
	;

endif_section 
	: ENDIF
	;

else_section 
	: ELSE statement
	;
elif_section 
	: ELIF '(' expression ')' statement 
	: elif_section ELIF '(' expression ')' statement 


