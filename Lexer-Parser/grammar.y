%{
#include<stdio.h>
#include<stdlib.h>

extern int yylex();
%}

// TODOs: 
// 1. Some parts here have to added to pdf. Search for "TODO" in this file.
// 2.

%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF GRAD COS SIN EXP LOG BACKWARD INT_CONST FLOAT_CONST CHAR_CONST
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP AT_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN AT_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME
%token CHAR SHORT INT LONG TENSOR FLOAT CNS VAR BOOL
%token IF ELIF ELSE LOOP ENDIF 
%start start
%%

start : compound_statement;

// Statements

compound_statement
	: '{' '}'
	| '{' declaration_list statement_list '}'
	| '{' statement_list '}'
	| '{' declaration_list '}'
	;

statement 
	: expression_statement
	| compound_statement
	| selection_statement // TODO: Add this in pdf, was missing there
	| iteration_statement
	;

selection_statement 
	: 
	| if_section else_section endif_section
	| if_section elif_section endif_section
	| if_section elif_section else_section endif_section
 	;

declaration_list 
	: declaration_list declaration
	| declaration
	;

statement_list 
	: statement
	| statement_list statement
	;

expression_statement 
	: ';'
	| exp ';'
	;

iteration_statement 
	: LOOP '(' expression_statement expression_statement expression ')' statement
	;

if_section 
	: IF '(' exp ')' statement
	;

endif_section 
	: ENDIF
	;

else_section 
	: ELSE statement
	;

elif_section 
	: ELIF '(' exp ')' statement 
	| elif_section ELIF '(' exp ')' statement
	;

// Declarations 
declaration 
	: declaration_type ';'
	| declaration_type init_declarators ';'
	;

declaration_type
	: grad_specifier type_specifier
	| type_specifier
	;

grad_specifier
	: CNS
	| VAR
	;

type_specifier
	: CHAR
	| INT
	| FLOAT
	| BOOL
	| TENSOR
	;

init_declarators
	: init_declarator
	| init_declarators init_declarator
	;

init_declarator 
	: declarator
	| declarator '=' initializer
	;

declarator
	: IDENTIFIER
	| '('declarator')'
	| '('declarator[const_exp]')'
	;

initializer
	: assignment_exp
	| '[' initializer_list ']'
	| '[' initializer_list ',' ']'
	;

initializer_list
	: initializer
	| initializer_list ',' initializer
	;

// Expressions
exp
	: assignment_exp
	| exp ',' assignment_exp
	;

assignment_exp
	: conditional_exp
	| unary_exp assignment_operator assignment_exp
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	| AT_ASSIGN
	;

conditional_exp
	: logical_or_exp
	| logical_or_exp '?' exp ':' conditional_exp
	;

logical_or_exp
	: logical_and_exp
	| logical_or_exp OR_OP logical_and_exp
	;

logical_and_exp
	: inclusive_or_exp
	| logical_and_exp AND_OP inclusive_or_exp
	;

inclusive_or_exp
	: exclusive_or_exp
	| inclusive_or_exp '|' exclusive_or_exp
	;

exclusive_or_exp
	: and_exp
	| exclusive_or_exp '^' and_exp
	;

and_exp
	: equality_exp
	| and_exp '&' equality_exp
	;

equality_exp
	: relational_exp
	| equality_exp EQ_OP relational_exp
	| equality_exp NE_OP relational_exp
	;

relational_exp
	: shift_exp
	| relational_exp '<' shift_exp
	| relational_exp '>' shift_exp
	| relational_exp LE_OP shift_exp
	| relational_exp GE_OP shift_exp
	;

shift_exp
	: additive_exp
	| shift_exp LEFT_OP additive_exp
	| shift_exp RIGHT_OP additive_exp
	;

additive_exp
	: multiplicative_exp
	| additive_exp '+' multiplicative_exp
	| additive_exp '-' multiplicative_exp
	;

multiplicative_exp
	: cast_exp
	| multiplicative_exp '*' cast_exp
	| multiplicative_exp '/' cast_exp
	| multiplicative_exp '%' cast_exp
	;

cast_exp
	: unary_exp
	| '(' type_specifier ')' cast_exp // TODO: Make change in grammar pdf
	;

unary_exp
	: postfix_exp
	| INC_OP unary_exp
	| DEC_OP unary_exp
	| unary_operator cast_exp
	| '('type_specifier')'cast_exp // TODO: Make change in grammar pdf
	| lib_funcs '(' unary_exp ')'
	;

// TODO: Make change in grammar pdf 
lib_funcs
	: GRAD
	| COS
	| SIN
	| EXP
	| LOG
	| BACKWARD
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	| AT_OP
	;

postfix_exp
	: primary_exp
	| postfix_exp '[' exp ']'
	| postfix_exp INC_OP
	| postfix_exp DEC_OP
	;

primary_exp
	: IDENTIFIER
	| constant
	| '(' exp ')'
	;

// Constants
constant
	: INT_CONST
	| CHAR_CONST
	| FLOAT_CONST
	;