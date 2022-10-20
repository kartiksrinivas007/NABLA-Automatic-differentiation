%{
#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include <string.h>
// #include "ast.h"
#include <stdarg.h>
#include <string.h>

extern int yylex();
extern FILE* yyin;
void yyerror(char *);


#ifndef DEBUG 
	#define SHOW printf
#else
	#define SHOW
#endif
// CompStatement *root;

// typedef struct node {
// 	char *token;
// 	// unsigned n_children;
// 	// struct node** children;
// 	struct node* child1;
// 	struct node* child2;
// 	struct node* child3;
// } node;

// void insert_node(char* token, node*, node*, node*); // Create a node with these children

extern int yylineno;
extern int yycolumn;
extern char yytext[];
extern char linebuf[];
extern const char* filename;
// extern void yyerror(char *);
struct YYLTYPE;
extern void lyyerror(struct YYLTYPE t,char *s,...);
extern void warning(const char*);
%}

// TODOs: 
// 1. Some parts here have to added to pdf. Search for "TODO" in this file.
// 2.

%union{
	int ival;
	float fval;
	// some  technique required for the symbol table , to map strings to indices 
	char *string;
	// class Expression *expr;

}

%token<string> IDENTIFIER CONSTANT STRING_LITERAL SIZEOF GRAD COS SIN EXP LOG BACKWARD 
%token<ival> INT_CONST 
%token<fval> FLOAT_CONST 
%token<string> CHAR_CONST 
%token<string> PRINT
%token<string> INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP AT_OP
%token<string> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN AT_ASSIGN
%token<string> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token<string> XOR_ASSIGN OR_ASSIGN TYPE_NAME
%token<string> CHAR INT TENSOR FLOAT CNS VAR BOOL
%token<string> IF ELIF ELSE LOOP ENDIF 
%token<string> DECLARE OPERATIONS GRADIENT
%start start

// %define parse.error verbose 
%locations
%%

start : declarations operations gradient
	;


declarations : DECLARE '{'decl_list'}'
	;	

decl_list : decl_list decl
	| decl
	;
decl : grad_specifier type_specifier init_declarator ';'
	;
grad_specifier : CNS
	| VAR
	;
type_specifier : CHAR
	| INT
	| FLOAT
	| BOOL
	| TENSOR
	;

init_declarator : declarator
	| declarator '=' initializer
	;

declarator : IDENTIFIER
	| declarator'[' INT_CONST ']'
	;

initializer: const_exp
	| '['initializer_list ']'
	;
	// | '[' initializer_list ','initializer ']'

initializer_list : initializer
	| initializer_list ',' initializer
	;

const_exp : constant '+' constant
	| constant
	;

constant : INT_CONST
	| FLOAT_CONST
	;



operations : OPERATIONS '{'assign_stmt_list '}'
	;

assign_stmt_list : assign_stmt
	| assign_stmt_list assign_stmt
	;

assign_stmt : IDENTIFIER assign_op exp ';'
	| ';'
	;

assign_op : ADD_ASSIGN
	| '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| SUB_ASSIGN
	| AT_ASSIGN
	;


exp : additive_exp
	;

additive_exp : additive_exp  '+' multiplicative_exp
	| additive_exp '-' multiplicative_exp
	| multiplicative_exp
	;
multiplicative_exp : multiplicative_exp '*' lib_exp
	| multiplicative_exp '/' lib_exp
	| multiplicative_exp '@' lib_exp
	| lib_exp
	;

lib_exp : IDENTIFIER
	| lib_funcs '(' exp ')'
	| constant
	;

lib_funcs : SIN
	| COS
	| LOG
	| EXP 
	;
gradient : GRADIENT '{' grad_stmt_list '}' 
	;

grad_stmt_list : grad_stmt
	| grad_stmt_list grad_stmt
	;

grad_stmt : BACKWARD '(' IDENTIFIER ')' ';'
	| GRAD'(' IDENTIFIER ')' ';'
	;


%%

void yyerror(char *s)
{
		fprintf(stderr, "%s:%d:%d:\e[1;31m error:\e[0m %s at %s\n%*d |%s\n",filename, yylineno , yycolumn, s, yytext,(int)(strlen(filename)-1), yylineno, linebuf);
		
		if(yylloc.first_line == yylloc.last_line){
			fprintf(stderr, "%*s\e[1;31m%*s", (int)(strlen(filename)+1),"|",yylloc.first_column,"^");
			
			for(int i = yylloc.first_column+1; i<=yylloc.last_column;i++){
				fprintf(stderr, "_");
				}
			fprintf(stderr, "\e[0m\n\n");
		}
}

void warning(const char* s){
	fprintf(stdout, "%s:%d:%d:\e[1;35m warning:\e[0m %s\n%*d |%s\n",filename, yylineno, yycolumn, s,(int)strlen(filename)-1 ,yylineno, linebuf);
	if(yylloc.first_line == yylloc.last_line){
		fprintf(stdout, "%*s\e[1;35m%*s", (int)(strlen(filename)+1),"|",yylloc.first_column,"^");
		
		for(int i = yylloc.first_column+1; i<=yylloc.last_column;i++){
			fprintf(stdout, "_");
			}
		fprintf(stdout, "\e[0m\n\n");
	}

}

void lyyerror(YYLTYPE t,char *s,...){
	va_list ap;
	va_start(ap,s);


	fprintf(stderr, "%s:%d:%d:\e[1;31m error:\e[0m %s at %s\n%*d |%s\n",filename, yylineno , yycolumn, s, yytext,(int)(strlen(filename)-1), yylineno, linebuf);
	
	if(yylloc.first_line == yylloc.last_line){
		fprintf(stderr, "%*s%*s", (int)(strlen(filename)+1),"|",yylloc.first_column,"^");
		
		for(int i = yylloc.first_column+1; i<=yylloc.last_column;i++){
			fprintf(stderr, "_");
		}
	}
	fprintf(stderr, "\n\n");
	
}

int main(int argc, char const *argv[])
{
	// printf("Input argument Number : %d", argc);
	/* Parent p;
	p.func();
	Child c;
	c.func();
	// including ast.cpp statement class
	Statement *s = new Statement(); */
	if(argc > 1){
		if((yyin = fopen(argv[1],"r")) == NULL){
			perror(argv[1]);
			return 1;
		}
		filename = argv[1];
	}
	else{
		filename = "(stdin)";
	}
	yyparse();
	return 0;
}
