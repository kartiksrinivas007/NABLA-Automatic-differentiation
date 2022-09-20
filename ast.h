#ifndef AST_H
#define AST_H

#include <iostream>
#include <cstring>

extern "C"
{
class Parent
{
public:
    void func()
    {
        std::cout << "Parent::func()" << std::endl;
    }
};

class Child : public Parent
{
public:
    void func()
    {
        std::cout << "Child::func()" << std::endl;
    }
};
}

#endif