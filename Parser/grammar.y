%code requires
{
	#include "../ast/ast.h"
	#include "../semantic/traversal.h"
}
%{
#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fstream>
#include <stdarg.h>
#include <string.h>
#include <iomanip>
#include <vector>
extern int yylex();
extern FILE* yyin;
void yyerror(const char *);


#ifndef DEBUG 
	#define SHOW printf
#else
	#define SHOW
#endif

class Start* root;

extern int yylineno;
extern int yycolumn;
extern char yytext[];
extern char linebuf[];
extern const char* filename;
// extern void yyerror(char *);
struct YYLTYPE;
extern void lyyerror(struct YYLTYPE t,char *s,...);
extern void warning(const char*);

struct SymTabItem{
	std::string name;
	std::string dataType;
	std::string type;
	std::vector<int> Dims;
	int rowNum;
	int colNum;
	struct SymTabItem* next;
};
#define SYM_SIZE 353
struct SymTabItem* symbolTable[SYM_SIZE]; 

void init_symtable(){
	for(int i=0;i<SYM_SIZE;i++)
		symbolTable[i]=NULL;
}
int hash(std::string name){
	int val = 0;
	for(char c: name){
		val+=c;
	}
	return val % SYM_SIZE;
}
char dataType[10];
char type[10];
void insertToSymbTab(char);
SymTabItem* search(std::string);
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
	class Start * AstStart;

// Declaration classes
	class Decl* AstDecl;
	GradSpecifier AstGradSpecifier;
	TypeSpecifier AstTypeSpecifier;
	GradType AstGradType;
	class InitDeclarator* AstInitDeclarator;
	class Declarator* AstDeclarator;
	class Initializer* AstInitializer;
	class ConstValue * AstConstValue;

// Operations
	class AssgnStmt* AstAssgnStmt;
	LibFuncs AstLibFuncs;
	AssignmentOperator AstAssignmentOperator;
	class Expr* AstExpr;
	class BinaryExpr* AstBinaryExpr;

// Gradient
	class GradStmt* AstGradStmt;


	std::vector<Decl*> * AstDeclList;
	std::vector<Initializer*> * AstInitializerList;
	std::vector<GradStmt*> * AstGradStmtList;
	std::vector<AssgnStmt*> * AstAssgnStmtList;


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
%type<AstDecl> decl
%type<AstGradSpecifier> grad_specifier
%type<AstTypeSpecifier> type_specifier
%type<AstInitDeclarator> init_declarator
%type<AstDeclarator> declarator
%type<AstInitializer> initializer
%type<AstConstValue> constant
%type<AstStart> start
%type<AstDeclList> decl_list
%type<AstDeclList> declarations
%type<AstInitializerList> initializer_list

%type<AstAssgnStmt> assign_stmt
%type<AstAssgnStmtList> assign_stmt_list
%type<AstAssgnStmtList> operations
%type<AstLibFuncs> lib_funcs
%type<AstAssignmentOperator> assign_op
%type<AstExpr> exp
%type<AstExpr> additive_exp
%type<AstExpr> multiplicative_exp
%type<AstExpr> lib_exp

%type<AstGradStmtList> grad_stmt_list
%type<AstGradStmt> grad_stmt
%type<AstGradStmtList> gradient
%type<AstGradType> grad_type
%start start

// %define parse.error verbose 
%locations
%%

start : declarations operations gradient {$$ = new Start($1,$2,$3); root = $$;}
	;


// Declaration
declarations : DECLARE '{'decl_list'}' {$$ = $3;}
	;	

decl_list 
	: decl_list decl {$1->push_back($2); $$ = $1;}
	| decl {$$ = new std::vector<Decl*>(); $$->push_back($1);}
	;

decl : grad_specifier type_specifier init_declarator ';' {$$ = new Decl($1, $2, $3);}
	;

grad_specifier 
	: CNS {$$ = GradSpecifier::CNS;strcpy(type, $1);}
	| VAR {$$ = GradSpecifier::VAR;strcpy(type, $1);}
	;

type_specifier : CHAR {$$ = TypeSpecifier::CHAR;strcpy(dataType, $1);}
	| INT	{$$ = TypeSpecifier::INT;strcpy(dataType, $1);}
	| FLOAT	{$$ = TypeSpecifier::FLOAT;strcpy(dataType, $1);}
	| BOOL	{$$ = TypeSpecifier::BOOL;strcpy(dataType, $1);}
	| TENSOR	{$$ = TypeSpecifier::TENSOR;strcpy(dataType, $1);}
	;

init_declarator 
	: declarator {$$ = new InitDeclarator($1, NULL);}
	| declarator '=' initializer {$$ = new InitDeclarator($1, $3);}
	;

declarator 
	: IDENTIFIER {$$ = new Declarator($1);insertToSymbTab('V');std::cout << "Decl: " << $$->name << std::endl;}
	| declarator'[' INT_CONST ']' {$$->Dimensions.push_back($3);search($$->name)->Dims.push_back($3);  std::cout <<"Wassup: " << $3 << " " << $$->Dimensions.size() << std::endl;}
	;

initializer
	: constant {$$ = new Initializer($1); /*std::cout << "Initializer: " << $1->value.int_val << std::endl;*/}
	| '['initializer_list ']' {$$ = new Initializer($2); $$->printInitializerList();} 
	;
	// | '[' initializer_list ','initializer ']'

initializer_list 
	: initializer {$$ = new std::vector<Initializer*>(); $$->push_back($1);}
	| initializer_list ',' initializer {$$ = $1; $$->push_back($3);}
	;

/* const_exp 
	: constant '+' constant
	| constant
	; */

constant 
	: INT_CONST {$$ = new ConstValue($1);}
	| FLOAT_CONST {$$ = new ConstValue($1);}
	;


// Operations
operations 
	: OPERATIONS '{' assign_stmt_list '}' {$$ = $3;}
	;

assign_stmt_list 
	: assign_stmt {$$ = new std::vector<AssgnStmt*>(); $$->push_back($1);}
	| assign_stmt_list assign_stmt {$$ = $1; $$->push_back($2);}
	;

assign_stmt 	
	: IDENTIFIER assign_op exp ';' {$$ = new AssgnStmt($1, $2, $3);}
	| ';' {$$ = new AssgnStmt(nullptr, std::nullopt, nullptr);}
	;

assign_op 
	: ADD_ASSIGN {$$ = AssignmentOperator::AST_ADD_ASSIGN;}
	| '=' {$$ = AssignmentOperator::AST_ASSIGN;}
	| MUL_ASSIGN {$$ = AssignmentOperator::AST_MUL_ASSIGN;}
	| DIV_ASSIGN {$$ = AssignmentOperator::AST_DIV_ASSIGN;}
	| SUB_ASSIGN {$$ = AssignmentOperator::AST_SUB_ASSIGN;}
	| AT_ASSIGN {$$ = AssignmentOperator::AST_AT_ASSIGN;}
	;


exp : additive_exp {$$ = $1; $$->printExpression(); std::cout << std::endl;}
	;

additive_exp 
	: additive_exp  '+' multiplicative_exp {$$ = new BinaryExpr($1, $3, '+');}
	| additive_exp '-' multiplicative_exp {$$ = new BinaryExpr($1, $3, '-');}
	| multiplicative_exp {$$ = $1;}
	;
	
multiplicative_exp 
	: multiplicative_exp '*' lib_exp {$$ = new BinaryExpr($1, $3, '*');}
	| multiplicative_exp '/' lib_exp {$$ = new BinaryExpr($1, $3, '/');}
	| multiplicative_exp '@' lib_exp {$$ = new BinaryExpr($1, $3, '@');}
	| lib_exp {$$ = $1;}
	;

lib_exp 
	: IDENTIFIER {$$ = new UnaryExpr(nullptr, std::nullopt, $1, nullptr);}
	| lib_funcs '(' exp ')' {$$ = new UnaryExpr($3, $1, "", nullptr);}
	| constant {$$ = new UnaryExpr(nullptr, std::nullopt, "", $1);}
	;

lib_funcs 
	: SIN {$$ = LibFuncs::SIN;}
	| COS {$$ = LibFuncs::COS;}
	| LOG {$$ = LibFuncs::LOG;}
	| EXP {$$ = LibFuncs::EXP;}
	;

// Gradient
gradient 
	: GRADIENT '{' grad_stmt_list '}' {$$ = $3;}
	;

grad_stmt_list 
	: grad_stmt {$$ = new std::vector<GradStmt*>(); $$->push_back($1);}
	| grad_stmt_list grad_stmt {$1->push_back($2); $$ = $1;}
	;

grad_type
	: BACKWARD {$$ = GradType::BACKWARD;}
	| GRAD {$$ = GradType::GRAD;}
	;

grad_stmt 
	: grad_type '(' IDENTIFIER ')' ';' {$$ = new GradStmt($1, $3);}
	;


%%

void insertToSymbTab(char c) {
  SymTabItem* q=search(yytext);
  
  if(q==NULL) {
	q=(SymTabItem*)malloc(sizeof(SymTabItem));
 	if(c == 'V') {
      q->name=strdup(yytext);
      q->dataType=strdup(dataType);
      q->rowNum=yylineno;    
      q->colNum=yycolumn;
      q->type=strdup(type); 
	  int val=hash(q->name);
	  q->next=symbolTable[val];
	  symbolTable[val]=q;
     }
  }
  else{
	yyerror("Repeat declaration found");
  }
}

SymTabItem* search(std::string name) { 
	int val = hash(name);
	SymTabItem* ptr = symbolTable[val];
	while ((ptr != NULL) && (name != ptr->name)) ptr = ptr->next;
	return ptr;
}

void yyerror(const char *s)
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
		/* filename = basename(argv[1]); */
		printf("File name : %s\n", filename);
	}
	else{
		filename = "(stdin)";
	}
	init_symtable();
	yyparse();
	printf("Name\tDatatype\tVartype\tRow\tCol\n");
	for(int i = 0; i<SYM_SIZE; i++){
		SymTabItem* ptr=symbolTable[i];
		while(ptr!=NULL){
			/* printf("%s\t%s\t\t%s\t%d\t%d\n", 
			ptr->name, ptr->dataType, ptr->type, ptr->rowNum, ptr->colNum); */
			std::cout<<ptr->name<<std::setw(10)<<ptr->dataType<<std::setw(10)<<ptr->type<<std::setw(10)<<ptr->rowNum<<std::setw(10)<<ptr->colNum;
			std::cout<<"    [";
			for(int x: ptr->Dims){
				std::cout<<x<<",";
			}
			std::cout<<"]\n";
			ptr=ptr->next;
		}
	}
	std::ofstream out("output.cpp");


	root->transpile(out);
	traverse_gradient(root->GradStmtList);
	
	return 0;
}
