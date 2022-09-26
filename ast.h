#ifndef AST_H
#define AST_H

#include <iostream>
#include <cstring>
#include <vector>
#include <memory>

// Naming scheme:
// 1. class MUST have names starting with Capital [A-Z]
// 2. Follow the scheme class BinaryStatement for creation of unique_ptr as class member
//   and initialization in constructor

// Search for ToDo

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

// Statements
class Node;
class Statement;
class BinaryStatement;
class ExprStatement;
class CompStatement;

// Expressions
class Expr;
class AssignmentExp;
class ConditionalExp;
class LogicalOrExp;
class LogicalAndExp;
class InclusiveOrExp;
class ExclusiveOrExp;
class AndExp;
class EqualityExp;
class RelationalExp;
class ShiftExp;
class AdditiveExp;
class MultiplicativeExp;
class CastExp;
class UnaryExp;
class PostfixExp;
class PrimaryExp;

// Declarations
class Declaration;
class DeclarationType;
class InitDeclarators;
class InitDeclarator;
class Declarator;
class Initializer;
class InitializerList;

class Node
{
public:
    Node();
    virtual ~Node() = default;
    virtual void print() = 0;
    // int row_num, col_num;
    // add codegen() function from llvm for IR gen
};

class Statement : public Node
{
public:
    Statement();
    virtual ~Statement() = default;
    virtual void print();
};

class BinaryStatement : public Node
{
private:
    std::unique_ptr<Statement> statement;
    std::unique_ptr<Declaration> declaration;

public:
    BinaryStatement(std::unique_ptr<Statement>, std::unique_ptr<Declaration>);
    virtual ~BinaryStatement() = default;
};

class ExprStatement : public Statement
{
private:
    std::unique_ptr<Expr> expression;

public:
    virtual ~ExprStatement() = default;
};

class CompStatement : public Statement
{
private:
    std::vector<std::unique_ptr<BinaryStatement>> binary_statements;

public:
    virtual ~CompStatement() = default;
};

class SelecStatement : public Statement
{
private:
    // Since statement is common in if, elif and else, we create a comman Statement* at the parent itself
    std::unique_ptr<Statement> statement;

public:
    virtual ~SelecStatement() = default;
};

class IfStatement : public SelecStatement
{
public:
    std::unique_ptr<Expr> expression;
};

class ElseStatement : public SelecStatement
{
public:
    virtual ~ElseStatement() = default;
};

class ElIfStatement : public SelecStatement
{
private:
    std::unique_ptr<Expr> expression;

public:
    virtual ~ElIfStatement() = default;
};

class IterStatement : public Statement
{
private:
    // LOOP contains either of the ones below.
    // Exactly one of the two of them below have to be nullptr
    std::unique_ptr<Declaration> declaration;
    std::unique_ptr<ExprStatement> init_expression;

    std::unique_ptr<ExprStatement> test_statement;
    std::unique_ptr<Expr> update_expreession; // The increment/decreement expression
    std::unique_ptr<Statement> loop_statement;

public:
    virtual ~IterStatement() = default;
};

// Expressions class hierarchy

class Expr : public Node
{
private:
    std::unique_ptr<Expr> expression;
    std::unique_ptr<AssignmentExp> assignment_expression;

public:
    virtual ~Expr() = default;
};

enum class AssignmentOperator
{
    ASSIGN,
    MUL_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN,
    ADD_ASSIGN,
    SUB_ASSIGN,
    LEFT_ASSIGN,
    RIGHT_ASSIGN,
    AND_ASSIGN,
    XOR_ASSIGN,
    OR_ASSIGN,
    AT_ASSIGN
};

class AssignmentExp : public Expr
{
public:
    ConditionalExp *conditional_expression;
    AssignmentExp *assignment_expression;
    UnaryExp *unary_expression;
    AssignmentOperator assignment_operator;
};

class ConditionalExp : public Expr
{
public:
    Expr *logical_or_expression;
    Expr *expression;
    Expr *conditional_expression;
};
class LogicalOrExp : public Expr
{
public:
    LogicalOrExp *logical_or_exp;
    LogicalAndExp *logical_and_exp;
};

class LogicalAndExp : public Expr
{
public:
    LogicalAndExp *logical_and_exp;
    InclusiveOrExp *inclusive_or_exp;
};

class InclusiveOrExp : public Expr
{
public:
    InclusiveOrExp *inclusive_or_exp;
    ExclusiveOrExp *exclusive_or_exp;
};

class ExclusiveOrExp : public Expr
{
public:
    ExclusiveOrExp *exclusive_or_exp;
    AndExp *and_exp;
};

class AndExp : public Expr
{
public:
    AndExp *and_exp;
    EqualityExp *equality_exp;
};

class EqualityExp : public Expr
{
public:
    EqualityExp *equality_exp;
    RelationalExp *relational_exp;
    bool isEqualOp;
};

enum class RelationalOp
{
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_EQUAL,
    GREATER_THAN_EQUAL
};

class RelationalExp : public Expr
{
public:
    RelationalExp *relational_exp;
    ShiftExp *shift_exp;
    RelationalOp relational_op;
};

class ShiftExp : public Expr
{
public:
    ShiftExp *shift_exp;
    AdditiveExp *additive_exp;
    bool isLeftShift;
};

class AdditiveExp : public Expr
{
public:
    AdditiveExp *additive_exp;
    MultiplicativeExp *multiplicative_exp;
    bool isAddition;
};

enum class MultiplicativeOp
{
    MULTIPLY,
    DIVIDE,
    MODULO,
    AT
};

class MultiplicativeExp : public Expr
{
public:
    MultiplicativeExp *multiplicative_exp;
    CastExp *cast_exp;
    MultiplicativeOp multiplicative_op;
};

enum class TypeSpecifier
{
    CHAR,
    INT,
    LONG,
    FLOAT,
    BOOL,
    TENSOR
};

class CastExp : public Expr
{
public:
    CastExp *cast_exp;
    UnaryExp *unary_exp;
    TypeSpecifier type_specifier;
};

enum class LibFuncs
{
    GRAD,
    COS,
    SIN,
    EXP,
    LOG,
    BACKWARD,
    SIZEOF,
    PRINT
};

enum class UnaryOp
{
    AND,
    PLUS,
    MINUS,
    TILDA,
    EXCLAMATION,
    AT
};
class UnaryExp : public Expr
{
public:
    UnaryExp *unary_exp;
    PostfixExp *postfix_exp;
    CastExp *cast_exp;
    AdditiveExp *additive_exp;
    bool isIncrement;
    UnaryOp unary_op;
    LibFuncs lib_funcs;
};

class PostfixExp : public Expr
{
public:
    PostfixExp *postfix_exp;
    PrimaryExp *primary_exp;
    Expr *expression;
    bool isIncrement;
};

enum class ConstantType
{
    INT,
    FLOAT,
    CHAR,
};
class PrimaryExp : public Expr
{
public:
    ConstantType type;
    Expr *expression;
    std::string identifier;
    std::string constant_literal;
};

// Declaration
class Declaration : public Node
{
public:
    DeclarationType *declaration_type;
    InitDeclarators *init_declarators;
};

enum class GradSpecifier
{
    CNS,
    VAR
};

class DeclarationType : public Node
{
public:
    TypeSpecifier type_specifier;
    GradSpecifier grad_specifier;
};

class InitDeclarator : public Node
{
public:
    Declarator *declarator;
    Initializer *initializer;
};

class InitDeclarators : public Node
{
public:
    std::vector<InitDeclarator *> init_declarators;
};

class Declarator : public Node
{
public:
    std::string identifier;
    ConditionalExp *conditional_exp;
    Declarator *declarator;
};

class Initializer : public Node
{
public:
    AssignmentExp *assignment_exp;
    InitializerList *initializer_list;
    // ToDo: Add error checkiing for semantically incorrect grammar
    bool isComma;
};

class InitializerList : public Node
{
public:
    std::vector<Initializer *> initializer_list;
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