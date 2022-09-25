#ifndef AST_H
#define AST_H

#include <iostream>
#include <cstring>
#include <vector>

// Naming scheme:
// class MUST have names starting with Capital [A-Z]

class Parent
{
public:
    void func();
    class Parent *child;
};

class Child : public Parent
{
public:
    void func();
};

class Node;
class Statement;
class BinaryStatement;
class ExprStatement;
class CompStatement;

// Incomplete
class Expr;
class Declaration;

class Node
{
public:
    virtual ~Node() {}
    virtual void print() = 0;
    int row_num, col_num;
    // add codegen() function from llvm for IR gen
};

class Statement : public Node
{
public:
    virtual void print();
};

class BinaryStatement : public Node
{
public:
    Statement *statement;
    Declaration *declaration;
    BinaryStatement();
};

class ExprStatement : public Statement
{
public:
    Expr *expression;
};

class CompStatement : public Statement
{
public:
    std::vector<BinaryStatement *> binary_statements;
};

class SelecStatement : public Statement
{
public:
    // Since statement is common in if, elif and else, we create a comman Statement* at the parent itself
    Statement *statement;
};

class IfStatement : public SelecStatement
{
public:
    Expr *expression;
};

class ElseStatement : public SelecStatement
{
public:
};

class ElIfStatement : public SelecStatement
{
public:
    Expr *expression;
};

class IterStatement : public Statement
{
    // LOOP contains either of the ones below.
    // Exactly one of the two of them below have to be nullptr
    Declaration *declaration;
    ExprStatement *init_expression;

    ExprStatement *test_statement;
    Expr *update_expreession; // The increment/decreement expression
    Statement *loop_statement;
};

// class DoubleExprAST : public BaseAST
// {
// public:
//     DoubleExprAST(double val) : val(val) {}
//     virtual void print()
//     {
//         std::cout << val << std::endl;
//     }

// private:
//     double val;
// };

// class IntExpreAST : public BaseAST
// {
// public:
//     IntExpreAST(int val) : val(val) {}
//     virtual void print()
//     {
//         std::cout << val << std::endl;
//     }

// private:
//     int val;
// };

// class VariableExprAST : public BaseAST
// {
// public:
//     VariableExprAST(const std::string &name) : name(name) {}
//     virtual void print()
//     {
//         std::cout << name << std::endl;
//     }

// private:
//     std::string name;
// };

#endif