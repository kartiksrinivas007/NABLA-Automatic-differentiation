#ifndef AST_H
#define AST_H

#include <iostream>
#include <cstring>
#include <vector>
#include <memory>

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

// Abstract class
class Node;

// Start Class
class Start;

// Declaration classes
class Decl;
enum class GradSpecifier
{
    CNS,
    VAR
};
enum class TypeSpecifier
{
    CHAR,
    INT,
    FLOAT,
    BOOL,
    TENSOR
};
class InitDeclarator;
class Declarator;
class Initializer;

// Expressions class
class AssgnStmt;

// Gradient class
class GradStmt;

class Node
{
public:
    Node();
    virtual ~Node() = default;
    // virtual void print() = 0;
    // int row_num, col_num;
    // add codegen() function from llvm for IR gen
};

class Start : public Node
{
public:
    std::vector<class Decl *> *DeclList;
    std::vector<class AssgnStmt *> *AssgnStmtList;
    std::vector<class GradStmt *> *GradStmtList;
    Start(std::vector<class Decl *> *DeclList, std::vector<class AssgnStmt *> *AssgnStmtList, std::vector<class GradStmt *> *GradStmtList);
    virtual ~Start() = default;
};

class Decl : public Node
{
public:
    GradSpecifier GradType;
    TypeSpecifier DataType;
    InitDeclarator *InitDeclaratorList;
    Decl(GradSpecifier, TypeSpecifier, InitDeclarator*);
    virtual ~Decl() = default;
};

class InitDeclarator : public Node
{
public:
    Declarator *declarator;
    Initializer *initializer;
    InitDeclarator(Declarator*, Initializer*);
    virtual ~InitDeclarator() = default;
};

class Declarator : public Node
{
public:
    std::string name;
    std::vector<int> Dimensions;
    Declarator(std::string);
    virtual ~Declarator() = default;
};

class Initializer : public Node
{
    public:
    union inbuilt_type
    {
        int int_val;
        float float_val;
    };
    inbuilt_type value;
    std::vector<Initializer*> InitializerList;
    Initializer(inbuilt_type, std::vector<Initializer*>);
    virtual ~Initializer() = default;
};



class AssgnStmt : public Node
{
public:
    // Declarator *declarator;
    // Initializer *initializer;
    // AssgnStmt(Declarator*, Initializer*);
    AssgnStmt();
    virtual ~AssgnStmt() = default;
};

class GradStmt : public Node
{
public:
    // Declarator *declarator;
    // Initializer *initializer;
    // GradStmt(Declarator*, Initializer*);
    GradStmt();
    virtual ~GradStmt() = default;
};

#endif