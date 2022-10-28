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

Initializer::Initializer(ConstValue* value)
{
    this->val.cvalue = value;
    this->isScalar = true;
}

Initializer::Initializer(std::vector<Initializer*> *InitializerList)
{
    this->val.InitializerList = InitializerList;
    this->isScalar = false;
}

AssgnStmt::AssgnStmt()
{

}

GradStmt::GradStmt(GradType grad_type, std::string name)
{
    this->grad_type = grad_type;
    this->name = name;
}

// int main()
// {
//     return 0;
// }