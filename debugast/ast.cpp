#include "ast.h"
// #include <iostream>

Node::Node()
{
    // std::cout << "Node::Node()" << std::endl;
}


Start::Start(std::vector<class Decl *> *DeclList, std::vector<class AssgnStmt *> *AssgnStmtList, std::vector<class GradStmt *> *GradStmtList)
{
    this->DeclList = DeclList;
    this->AssgnStmtList = AssgnStmtList;
    this->GradStmtList = GradStmtList;
}

Decl::Decl(GradSpecifier GradType, TypeSpecifier DataType, InitDeclarator* InitDeclaratorList)
{
    this->GradType = GradType;
    this->DataType = DataType;
    this->InitDeclaratorList = InitDeclaratorList;
}

InitDeclarator::InitDeclarator(Declarator* declarator, Initializer* initializer = NULL)
{
    this->declarator = declarator;
    this->initializer = initializer;
}

Declarator::Declarator(std::string name)
{
    this->name = name;
    // this->Dimensions = Dimensions;
}

ConstValue::ConstValue(int value)
{
    this->isInt = true;
    this->value.int_val = value;
}

ConstValue::ConstValue(float value)
{
    this->isInt = false;
    this->value.float_val = value;
}

Initializer::Initializer(ConstValue* value, std::vector<Initializer*> InitializerList)
{
    this->value = value;
    this->InitializerList = InitializerList;
}

AssgnStmt::AssgnStmt()
{

}

GradStmt::GradStmt()
{

}

// int main()
// {
//     return 0;
// }