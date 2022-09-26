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

// Statement functions

// BinaryStatement class
BinaryStatement::BinaryStatement(std::unique_ptr<Statement> statement, std::unique_ptr<Declaration> declaration)
    : statement(std::move(statement)), declaration(std::move(declaration))
{
}