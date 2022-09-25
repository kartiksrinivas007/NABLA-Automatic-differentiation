#ifndef AST_H
#define AST_H

#include <iostream>
#include <cstring>

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

class BaseAST
{
public:
    virtual ~BaseAST() {}
    virtual void print() = 0;
    int row_num, col_num;
};

class DoubleExprAST : public BaseAST
{
public:
    DoubleExprAST(double val) : val(val) {}
    virtual void print()
    {
        std::cout << val << std::endl;
    }

private:
    double val;
};

class IntExpreAST : public BaseAST
{
public:
    IntExpreAST(int val) : val(val) {}
    virtual void print()
    {
        std::cout << val << std::endl;
    }
private :
    int val;
};

class VariableExprAST : public BaseAST
{
public:
    VariableExprAST(const std::string &name) : name(name) {}
    virtual void print()
    {
        std::cout << name << std::endl;
    }
private:
    std::string name;
};

#endif