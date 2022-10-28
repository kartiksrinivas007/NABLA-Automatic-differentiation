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

// Abstract Node class
class Node;

// Start Class
class Start;

// Classes that are a part of the Declare Section
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
class ConstValue;
class InitDeclarator;
class Declarator;
class Initializer;

// Expressions class
class AssgnStmt;

// Gradient class
class GradStmt;
enum class GradType
{
    GRAD,
    BACKWARD
};

class Node
{
public:
    Node();
    virtual ~Node() = default;
    // virtual void print() = 0;
    // int row_num, col_num;
    // add codegen() function from llvm for IR gen
};

// Start Class stores the pointers to all the declarations, expressions and gradients
class Start : public Node
{
public:
    std::vector<class Decl *> *DeclList;
    std::vector<class AssgnStmt *> *AssgnStmtList;
    std::vector<class GradStmt *> *GradStmtList;
    Start(std::vector<class Decl *> *DeclList, std::vector<class AssgnStmt *> *AssgnStmtList, std::vector<class GradStmt *> *GradStmtList);
    virtual ~Start() = default;
};

// Decl Class stores the declarations of a variable such as their gradient specifier, data type and the pointer to the initializer
// It also stores the initial value of the variable if it is initialized
class Decl : public Node
{
public:
    GradSpecifier GradType;
    TypeSpecifier DataType;
    InitDeclarator *InitDeclaratorList;
    Decl(GradSpecifier, TypeSpecifier, InitDeclarator*);
    virtual ~Decl() = default;
};

// InitDeclarator Class stores the pointer to the declarator and the pointer to the initializer
class InitDeclarator : public Node
{
public:
    Declarator *declarator;
    Initializer *initializer;
    InitDeclarator(Declarator*, Initializer*);
    virtual ~InitDeclarator() = default;
};
// Declarator Class stores the name of the variable and the dimensions of the variable
class Declarator : public Node
{
public:
    std::string name;
    std::vector<int> Dimensions;
    Declarator(std::string);
    virtual ~Declarator() = default;
};

class ConstValue : public Node
{
public:
    union inbuilt_type
    {
        int int_val;
        float float_val;
    };
    inbuilt_type value;
    bool isInt;
    // ConstValue(int value, bool isInt = false);
    ConstValue(int value);
    ConstValue(float value);
    virtual ~ConstValue() = default;
};

// Initializer Class stores the value of the variable using a union structure
// It also stores the pointers to the initializers of the elements of the variable if it is an array
class Initializer : public Node
{
    public:
    union type_value{
        ConstValue *cvalue;
        std::vector<Initializer*> *InitializerList;
        constexpr type_value() : cvalue(nullptr) {}
        ~type_value() {}
    };
    type_value val;
    bool isScalar;
    Initializer(ConstValue* value);
    Initializer(std::vector<Initializer*> *InitializerList);
    // Initializer(ConstValue*, std::vector<Initializer*>);

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
    GradType grad_type;
    std::string name;
    GradStmt(GradType, std::string);
    virtual ~GradStmt() = default;
};

#endif