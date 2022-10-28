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

Decl::Decl(GradSpecifier GradType, TypeSpecifier DataType, InitDeclarator *InitDeclaratorList)
{
    this->GradType = GradType;
    this->DataType = DataType;
    this->InitDeclaratorList = InitDeclaratorList;
}

InitDeclarator::InitDeclarator(Declarator *declarator, Initializer *initializer = NULL)
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

Initializer::Initializer(ConstValue *value)
{
    this->val.cvalue = value;
    this->isScalar = true;
}

Initializer::Initializer(std::vector<Initializer *> *InitializerList)
{
    this->val.InitializerList = InitializerList;
    this->isScalar = false;
}

// void print_init_list_tree(Initializer *obj)
// {
// }

AssgnStmt::AssgnStmt(std::string name, std::optional<AssignmentOperator> op, Expr *expr)
{
    this->name = name;
    this->op = op;
    this->expr = expr;
}

Expr::Expr()
{
}

void Expr::printExpression() {}

BinaryExpr::BinaryExpr(Expr *lhs, Expr *rhs, char op)
{
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = op;
}

void BinaryExpr::printExpression()
{
    std::cout << "(";
    this->lhs->printExpression();
    std::cout << " " << this->op << " ";
    this->rhs->printExpression();
    std::cout << ")";
}

UnaryExpr::UnaryExpr(Expr *expr, std::optional<LibFuncs> libfunc, std::string identifier, ConstValue *cvalue)
{
    this->expr = expr;
    this->libfunc = libfunc;
    this->identifier = identifier;
    this->cvalue = cvalue;
}

void UnaryExpr::printExpression()
{
    if (this->identifier != "")
    {
        std::cout << this->identifier;
    }
    else if (this->cvalue != nullptr)
    {
        if (this->cvalue->isInt)
        {
            std::cout << this->cvalue->value.int_val;
        }
        else
        {
            std::cout << this->cvalue->value.float_val;
        }
    }
    else
    {
        std::cout << "(";
        switch (this->libfunc.value())
        {
            {
            case LibFuncs::SIN:
                std::cout << "sin(";
                break;
            case LibFuncs::COS:
                std::cout << "cos(";
                break;

            default:
                std::cout << "Invalid libfunc";
                break;
            }
        }
        this->expr->printExpression();
        std::cout << ")";
    }
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