#include "sym.h"

void insertToSymbTab(std::unordered_map<std::string,SymTabItem>* symbolTable,char* yytext,int yylineno,int yycolumn,
std::string dataType,std::string type) {
    std::string t_name(yytext);
    SymTabItem* q=search(symbolTable,yytext);

    if(q==NULL){
        auto& ref=(*symbolTable);
        ref[t_name].name=t_name;
        ref[t_name].dataType=dataType;
        ref[t_name].rowNum=yylineno;
        ref[t_name].colNum=yycolumn;
        ref[t_name].type=type;
    }
    else{
        std::cerr<<"Repeat declaration found"<<std::endl;
    }
}

void Undeclaration_Error(std::unordered_map<std::string,SymTabItem>* symbolTable,std::string inp1){
	if(symbolTable->find(inp1)==symbolTable->end()){
		std::cerr<<"undeclared variable \""<<inp1<< "\" is used "<<std::endl;
	}
}

SymTabItem* search(std::unordered_map<std::string,SymTabItem>* symbolTable,std::string name) { 
	auto it=symbolTable->find(name);
	if(it==symbolTable->end()){
		return NULL;
	}
	return &(it->second);
}