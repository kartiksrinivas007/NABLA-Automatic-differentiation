%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

extern int yylex();
extern FILE* yyin;

void yyerror(const char *);


#ifndef DEBUG 
	#define SHOW printf
#else
	#define SHOW
#endif

// class Start* root;

struct YYLTYPE;
extern int yylineno;
extern int yycolumn;
extern char yytext[];
extern char linebuf[];
extern const char* filename;
extern void yyerror(const char *);
extern void lyyerror(struct YYLTYPE t,char *s,...);
extern void warning(const char*);

// #define ccay(x) printf("%s\t->\t",$$); for(int i=0;i<x;i++) printf("%s ",$i); printf("\n")
%}

// TODOs: 
// 1. Some parts here have to added to pdf. Search for "TODO" in this file.
// 2.

%union{
	int ival;
	float fval;
	char *string;
}

%token<string> IDENTIFIER GRAD COS SIN EXP LOG BACKWARD 
%token<string> INT_CONST 
%token<string> FLOAT_CONST 
%token<string> MUL_ASSIGN DIV_ASSIGN ADD_ASSIGN AT_ASSIGN
%token<string> SUB_ASSIGN
%token<string> CHAR INT TENSOR FLOAT CNS VAR BOOL
%token<string> DECLARE OPERATIONS GRADIENT
%token<string> '=' ';' ']' '[' ','

%type<string> decl
%type<string> grad_specifier
%type<string> type_specifier
%type<string> init_declarator
%type<string> declarator
%type<string> initializer
%type<string> constant
%type<string> start
%type<string> decl_list
%type<string> declarations
%type<string> initializer_list

%type<string> assign_stmt
%type<string> assign_stmt_list
%type<string> operations
%type<string> lib_funcs
%type<string> assign_op
%type<string> exp
%type<string> additive_exp
%type<string> multiplicative_exp
%type<string> lib_exp

%type<string> grad_stmt_list
%type<string> grad_stmt
%type<string> gradient
%type<string> grad_type

%start start

// %define parse.error verbose 
%locations
%%

start : declarations operations gradient {/*  $$ = strdup("start"); printf("%s\t->\t%s %s %s\n",$$,$1,$2,$3); */ }
	;


// Declaration
declarations : DECLARE '{'decl_list'}'
	;	

decl_list 
	: decl_list decl 
	| decl 
	;

decl : grad_specifier type_specifier init_declarator ';' 
	| error ';' { yyerror("Syntax error in declaration"); yyerrok; }
	;

grad_specifier 
	: CNS 
	| VAR 
	;

type_specifier : CHAR 
	| INT	
	| FLOAT	
	| BOOL	
	| TENSOR	
	;

init_declarator 
	: declarator 
	| declarator '=' initializer 
	;

declarator 
	: IDENTIFIER  { $$ = strdup("declarator"); }
	| declarator'[' INT_CONST ']' { }
	;

initializer
	: constant 
	| '['initializer_list ']' {$$ = $2;}
	;

initializer_list 
	: initializer 
	| initializer_list ',' initializer 
	;

/* const_exp 
	: constant '+' constant
	| constant
	; */

constant 
	: INT_CONST 
	| FLOAT_CONST 
	;


// Operations
operations 
	: OPERATIONS '{' assign_stmt_list '}' 
	;

assign_stmt_list 
	: assign_stmt 
	| assign_stmt_list assign_stmt 
	;

assign_stmt 	
	: IDENTIFIER assign_op exp ';' 
	| ';' 
	;

assign_op 
	: ADD_ASSIGN 
	| '=' 			
	| MUL_ASSIGN 
	| DIV_ASSIGN 
	| SUB_ASSIGN 
	| AT_ASSIGN 
	;


exp : additive_exp 
	;

additive_exp 
	: additive_exp  '+' multiplicative_exp 
	| additive_exp '-' multiplicative_exp 
	| multiplicative_exp 
	;
	
multiplicative_exp 
	: multiplicative_exp '*' lib_exp 
	| multiplicative_exp '/' lib_exp 
	| multiplicative_exp '@' lib_exp 
	| lib_exp 
	;

lib_exp 
	: IDENTIFIER 
	| lib_funcs '(' exp ')' 
	| constant 
	;

lib_funcs 
	: SIN 
	| COS 
	| LOG 
	| EXP 
	;

// Gradient
gradient 
	: GRADIENT '{' grad_stmt_list '}' 
	;

grad_stmt_list 
	: grad_stmt 
	| grad_stmt_list grad_stmt 
	;

grad_type
	: BACKWARD 
	| GRAD 
	;

grad_stmt 
	: grad_type '(' IDENTIFIER ')' ';' 
	;


%%

void yyerror(const char *s){

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
	if(argc > 1){
		if((yyin = fopen(argv[1],"r")) == NULL){
			perror(argv[1]);
			return 1;
		}
		filename = argv[1];
		printf("File name : %s\n", filename);
	}
	else{
		filename = "(stdin)";
	}
	yyparse();
	
	return 0;
}