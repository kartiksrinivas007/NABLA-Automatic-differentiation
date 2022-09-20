#ifndef AST_H
#define AST_H

#include <iostream>
#include <cstring>

class Parent
{
public:
    void func();
};

class Child : public Parent
{
public:
    void func();
};

#endif