%{
#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include <string.h>
#include "ast.h"
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
CompStatement *root;

typedef struct node {
	char *token;
	// unsigned n_children;
	// struct node** children;
	struct node* child1;
	struct node* child2;
	struct node* child3;
} node;

void insert_node(char* token, node*, node*, node*); // Create a node with these children

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
	class Expression *expr;

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
%start start

// %define parse.error verbose 
%locations
%%

start : compound_statement {SHOW("parsing complete! \n");}
	   ;

// Statements

compound_statement 
	: '{' '}' {printf("compound_statment is empty\n"); /*root = new CompStatement();*/}
	| '{' binary_ds_list '}' {printf("cmp_stmt -> binary_ds_list\n");} 
  | '{' error '}' {yyerrok;}
	;
	// | '{' declaration_list statement_list '}'  {printf("comp_stmt -> decl_stmt  + stmt_list\n");}
	// | '{' statement_list '}'  {printf("comp_stmt -> stmt_list \n");}
	// | '{' declaration_list '}' {printf("comp_stmt -> declaration_list\n");}
	// ;
binary_ds_list
	: binary_ds_list binary_statement {SHOW ("binary_ds_list -> binary_ds_list + binary_statement\n");}
	| binary_statement {SHOW ("binary_ds_list -> binary_statement\n");}
	;

binary_statement 
	: declaration {SHOW ("binary_statment -> declaration\n");}
	| statement {SHOW("binary_statement -> statement");}
	;
statement 
	: expression_statement {SHOW("stmt -> exp_stmt\n");}
	| compound_statement {SHOW("stmt -> comp_stmt\n");}
	| selection_statement {SHOW("stmt -> selection_stmt\n");}// TODO: Add this in pdf, was missing there
	| iteration_statement {SHOW("stmt -> iter_statementist\n");}
	;

selection_statement 
	: 
	/*|*/ if_section else_section ENDIF  {SHOW("selection_stmt -> if else endif\n");}
	| if_section elif_section ENDIF	{SHOW("selection_stmt -> if elif endif\n");}
	| if_section elif_section else_section ENDIF {SHOW("selection_stmt -> if elif else endif\n");}
	| if_section ENDIF {SHOW("selection_stmt -> if endif\n");}
 	;

// declaration_list 
// 	: declaration_list declaration {SHOW("decl_list -> decl_list decl\n");}
// 	| declaration {SHOW("decl_list -> decl\n");}
// 	;

// statement_list 
// 	: statement {SHOW("state_list -> stmt\n");}
// 	| statement_list statement {SHOW("state_list -> state_list stmt\n");}
// 	;

expression_statement 
	: ';' {SHOW("exp_stmt -> ;\n");}
	| exp ';' {SHOW("exp_stmt -> exp ;\n");}
	;

iteration_statement 
	: LOOP '(' expression_statement expression_statement exp ')' statement {SHOW("iter_stmt -> %s (exp_stmt exp_stmt exp) stmt\n", $1);}
	| LOOP '(' declaration expression_statement exp ')' statement {SHOW("iter_stmt -> %s (decl exp_stmt exp) stmt\n", $1);}
	;

if_section 
	: IF '(' exp ')' statement {SHOW("if_sec -> %s (exp) stmt\n", $1);}
	;

else_section 
	: ELSE statement {SHOW("else_sec -> %s stmt\n", $1);}
	;

elif_section 
	: ELIF '(' exp ')' statement {SHOW("elif_sec -> %s (exp) stmt\n", $1);}
	| elif_section ELIF '(' exp ')' statement {SHOW("elif_sec -> elif_sec %s (exp) stmt\n", $2);}
	;

// Declarations 
declaration 
	: declaration_type init_declarators ';' {SHOW("decl -> decl_type init_decls\n");}
	| error ';' {yyerrok;}
	;

declaration_type
	: grad_specifier type_specifier {SHOW("decl_type -> grad_spec type_spec\n");}
	| type_specifier {SHOW("decl_type -> type_spec\n");}
	;

grad_specifier
	: CNS {SHOW("grad_spec -> %s\n", $1);}
	| VAR {SHOW("grad_spec -> %s\n", $1);}
	;

type_specifier
	: CHAR {SHOW("type_spec -> %s\n", $1);}
	| INT {SHOW("type_spec -> %s\n", $1);}
	| FLOAT {SHOW("type_spec -> %s\n", $1);}
	| BOOL {SHOW("type_spec -> %s\n", $1);}
	| TENSOR {SHOW("type_spec -> %s\n", $1);}
	;

init_declarators
	: init_declarator {SHOW("init_decls -> init_decl\n");}
	| init_declarators ',' init_declarator {SHOW("init_decls -> init_decls init_decl\n");}
	;

init_declarator 
	: declarator {SHOW("init_decl -> decl\n");}
	| declarator '=' initializer {SHOW("int_decl -> decl = initializer\n");}
	;

declarator
	: IDENTIFIER {SHOW("decl -> %s\n", $1);}
	| '('declarator')' {SHOW("decl -> (decl)\n");}
	| declarator '[' conditional_exp ']' {SHOW("decl -> decl [conditional_exp]\n");}
	;

initializer
	: assignment_exp {SHOW("initializer -> assign_exp\n");}
	| '[' initializer_list ']' {SHOW("initializer -> [initializer_list]\n");}
	| '[' initializer_list ',' ']' {SHOW("initializer -> [initializer_list,]\n");}
	;

initializer_list
	: initializer {SHOW("initializer_list -> initializer\n");}
	| initializer_list ',' initializer {SHOW("initializer_list -> initializer_list, initializer\n");}
	;

// Expressions
exp
	: assignment_exp {SHOW("exp -> assign_exp\n");}
	| exp ',' assignment_exp {SHOW("exp -> exp, assign_exp\n");}
	;

assignment_exp
	: conditional_exp {SHOW("assign_exp -> cond_exp\n");}
	| unary_exp assignment_operator assignment_exp {SHOW("assign_exp -> unary_exp assign_op assign_exp\n");}
	;

assignment_operator
	: '=' {SHOW("assign_op -> =\n");}
	| MUL_ASSIGN {SHOW("assign_op -> %s\n", $1);}
	| DIV_ASSIGN {SHOW("assign_op -> %s\n", $1);}
	| MOD_ASSIGN {SHOW("assign_op -> %s\n", $1);}
	| ADD_ASSIGN {SHOW("assign_op -> %s\n", $1);}
	| SUB_ASSIGN {SHOW("assign_op -> %s\n", $1);}
	| LEFT_ASSIGN	{SHOW("assign_op -> %s\n", $1);}
	| RIGHT_ASSIGN {SHOW("assign_op -> %s\n", $1);}
	| AND_ASSIGN {SHOW("assign_op -> %s\n", $1);}
	| XOR_ASSIGN {SHOW("assign_op -> %s\n", $1);}
	| OR_ASSIGN {SHOW("assign_op -> %s\n", $1);}
	| AT_ASSIGN {SHOW("assign_op -> %s\n", $1);}
	;

conditional_exp
	: logical_or_exp {SHOW("cond_exp -> log_or_exp\n");}
	| logical_or_exp '?' exp ':' conditional_exp {SHOW("cond_exp -> log_or_exp ? exp : cond_exp\n");}
	;
/* const_exp
	: conditional_exp  {SHOW("const_exp -> cond_exp\n");}
	// TODO: Add this in pdf, was missing there 
	; */
logical_or_exp
	: logical_and_exp {SHOW("log_or_exp -> log_and_exp\n");}
	| logical_or_exp OR_OP logical_and_exp {SHOW("log_or_exp -> log_or_exp %s log_and_exp\n", $2);}
	;

logical_and_exp
	: inclusive_or_exp {SHOW("log_and_exp -> incl_or_exp\n");}
	| logical_and_exp AND_OP inclusive_or_exp {SHOW("log_and_exp -> log_and_exp %s incl_or_exp\n", $2);}
	;

inclusive_or_exp
	: exclusive_or_exp {SHOW("incl_or_exp -> excl_or_exp\n");}
	| inclusive_or_exp '|' exclusive_or_exp {SHOW("incl_or_exp -> incl_or_exp | excl_or_exp\n");}
	;

exclusive_or_exp
	: and_exp {SHOW("excl_or_exp -> and_exp\n");}
	| exclusive_or_exp '^' and_exp 	{SHOW("excl_or_exp -> excl_or_exp ^ and_exp\n");}
	;

and_exp
	: equality_exp {SHOW("and_exp -> equ_exp\n");}
	| and_exp '&' equality_exp {SHOW("and_exp -> and_exp & equ_exp\n");}
	;

equality_exp
	: relational_exp {SHOW("equ_exp -> rel_exp\n");}
	| equality_exp EQ_OP relational_exp {SHOW("equ_exp -> equ_exp %s rel_exp\n", $2);}
	| equality_exp NE_OP relational_exp {SHOW("equ_exp -> equ_exp %s rel_exp\n", $2);}
	;

relational_exp
	: shift_exp {SHOW("rel_exp -> shift_exp\n");}
	| relational_exp '<' shift_exp {SHOW("rel_exp -> rel_exp < shift_exp\n");}
	| relational_exp '>' shift_exp {SHOW("rel_exp -> rel_exp > shift_exp\n");}
	| relational_exp LE_OP shift_exp {SHOW("rel_exp -> rel_exp %s shift_exp\n", $2);}
	| relational_exp GE_OP shift_exp {SHOW("rel_exp -> rel_exp %s shift_exp\n", $2);}
	; 

shift_exp
	: additive_exp {SHOW("shift_exp -> add_exp\n");}
	| shift_exp LEFT_OP additive_exp {SHOW("shift_exp -> shift_exp %s add_exp\n", $2);}
	| shift_exp RIGHT_OP additive_exp {SHOW("shift_exp -> shift_exp %s add_exp\n", $2);}
	;

additive_exp
	: multiplicative_exp {SHOW("add_exp -> mult_exp\n");}
	| additive_exp '+' multiplicative_exp {SHOW("add_exp -> add_exp + mult_exp\n");}
	| additive_exp '-' multiplicative_exp {SHOW("add_exp -> add_exp - mult_exp\n");}
	;

multiplicative_exp
	: cast_exp {SHOW("mult_exp -> cast_exp\n");}
	| multiplicative_exp '*' cast_exp {SHOW("mult_exp -> mult_exp * cast_exp\n");}
	| multiplicative_exp '/' cast_exp {SHOW("mult_exp -> mult_exp / cast_exp\n");}
	| multiplicative_exp '%' cast_exp {SHOW("mult_exp -> mult_exp modulo cast_exp\n");}
	| multiplicative_exp '@' cast_exp {SHOW("mult_exp -> mult_exp matmul cast_exp\n");}
	;

cast_exp
	: unary_exp {SHOW("cast_exp -> unary_exp\n");}
	| '(' type_specifier ')' cast_exp  {SHOW("cast_exp -> (type_specifier) cast_exp\n");}// TODO: Make change in grammar pdf
	;

unary_exp
	: postfix_exp {SHOW("unary_exp -> postfix_exp\n");}
	| INC_OP unary_exp {SHOW("unary_exp -> %s unary_exp\n", $1);}
	| DEC_OP unary_exp {SHOW("unary_exp -> %s unary_exp\n", $1);}
	| unary_operator cast_exp {SHOW("unary_exp -> unary_op cast_exp\n");}
	//| '('type_specifier')'cast_exp // TODO: Make change in grammar pdf
	| lib_funcs '(' conditional_exp ')' {SHOW("unary_exp -> lib_funcs (additive_exp)\n");}
	;

// TODO: Make change in grammar pdf 
lib_funcs
	: GRAD {SHOW("lib_funcs -> %s\n", $1);}
	| COS {SHOW("lib_funcs -> %s\n", $1);}
	| SIN {SHOW("lib_funcs -> %s\n", $1);}
	| EXP	{SHOW("lib_funcs -> %s\n", $1);}
	| LOG	{SHOW("lib_funcs -> %s\n", $1);}
	| BACKWARD	{SHOW("lib_funcs -> %s\n", $1);}
	| SIZEOF {SHOW("lib_funcs -> %s\n", $1);}
	| PRINT {SHOW("lib_funcs -> %s\n", $1);}
	;

unary_operator
	: '&' {SHOW("unary_op -> &\n");}
	//| '*' {SHOW("unary_op -> *\n");}
	| '+' {SHOW("unary_op -> +\n");}
	| '-' {SHOW("unary_op -> -\n");}
	| '~' {SHOW("unary_op -> ~\n");}
	| '!' {SHOW("unary_op -> !\n");}
	| AT_OP {SHOW("unary_op -> %s\n", $1);}
	;

postfix_exp
	: primary_exp {SHOW("postfix_exp -> primary_exp\n");}
	| postfix_exp '[' exp ']' {SHOW("postfix_exp -> postfix_exp [ exp ]\n");}
	| postfix_exp INC_OP {SHOW("postfix_exp -> postfix_exp %s\n", $2);}
	| postfix_exp DEC_OP {SHOW("postfix_exp -> postfix_exp %s\n", $2);}
	;

primary_exp
	: IDENTIFIER {SHOW("primary_exp -> %s\n", $1);}
	| constant {SHOW("primary_exp -> constant\n");}
	| '(' exp ')' {SHOW("primary_exp -> ( exp )\n");}
	| '(' error ')' {yyerrok; lyyerror(@1,"error"); yyerrok; }
	;

// Constants
constant
	: INT_CONST {SHOW("constant -> %d\n", $1);}
	| CHAR_CONST {SHOW("constant -> %s\n", $1);}
	| FLOAT_CONST {SHOW("constant -> %f\n", $1);}
	| CONSTANT {SHOW("constant -> %s\n", $1);}
	| STRING_LITERAL
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