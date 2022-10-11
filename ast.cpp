#include "ast.h"
// #include <llvm/ADT/APFloat.h>
// #include <llvm-c-14/llvm-c/Core.h>

void Parent::func()
{
    std::cout << "Parent::func()" << std::endl;
}

void Child::func()
{
    std::cout << "Child::func()" << std::endl;
}

void Statement::print()
{
    std::cout << "[Incomplete] Print statement\n";
}

Statement::Statement()
{
    std::cout << "Statement::Statement()" << std::endl;
}

Node::Node()
{
    std::cout << "Node::Node()" << std::endl;
}

// class Statement

// class BinaryStatement
BinaryStatement::BinaryStatement(std::unique_ptr<Statement> statement, std::unique_ptr<Declaration> declaration)
{
    this->statement = std::move(statement);
    this->declaration = std::move(declaration);
}

// class CompStatement
CompStatement::CompStatement()
{
    binary_statements = std::vector<std::unique_ptr<BinaryStatement>>();
}

CompStatement *CompStatement::singleton_instance = nullptr;
CompStatement *CompStatement::get_instance()
{
    if (singleton_instance == nullptr)
    {
        singleton_instance = new CompStatement();
    }
    return singleton_instance;
}

void CompStatement::add_binary_statement(std::unique_ptr<BinaryStatement> binary_statement)
{
    this->binary_statements.push_back(std::move(binary_statement));
}

// class ExprStatement
ExprStatement::ExprStatement(std::unique_ptr<Expr> expression)
{
    this->expression = std::move(expression);
}