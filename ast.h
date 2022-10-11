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
class CompStatement; // Singleton class

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
    ExprStatement(std::unique_ptr<Expr>);
    virtual ~ExprStatement() = default;
};

class CompStatement : public Statement
// This will be a singleton class because there can be only one compound statement which
// will be a bunch of binary_statement enclosed in {}.
// Singleton will ease some of out work as during bottom up parsing, whenever yacc wants
// to "pass" a declaration or statement to the compound statement, we can just pass it
// to the singleton instance of CompStatement without worrying whether the instance
// was created or not.
{
private:
    std::vector<std::unique_ptr<BinaryStatement>> binary_statements; // ToDo: Can vector also be changed to unique_ptr?

    // private constructor to make prevent instantiation
    CompStatement();

    // The singleton instance. static because we only want to have one instance of CompStatement and we want to access it from static function get_instance()
    static CompStatement *singleton_instance; // ToDo: can we make use of unique_ptr here?

public:
    virtual ~CompStatement() = default;

    // Function to get the singleton instance. static because we want to call it without an instance
    static CompStatement *get_instance(); // ToDo: can we make use of unique_ptr here?

    void add_binary_statement(std::unique_ptr<BinaryStatement>); // append the binary statement to the vector
};

class SelecStatement : public Statement
{
private:
    // Since statement is common in if, elif and else, we create a comman Statement* at the parent itself
    std::unique_ptr<Statement> statement;

public:
    SelecStatement(std::unique_ptr<Statement>);
    virtual ~SelecStatement() = default;
};

// IfStatement inherits from SelectStatement
class IfStatement : public SelecStatement
{
private:
    std::unique_ptr<Expr> expression;
public:
    //Consteuctor for IfStatement class
    IfStatement(std::unique_ptr<Expr>, std::unique_ptr<Statement> statement);
    virtual ~IfStatement() = default;
};

// ElseStatement inherits from SelectStatement
class ElseStatement : public SelecStatement
{
public:
    ElseStatement(std::unique_ptr<Statement> statement);
    virtual ~ElseStatement() = default;
};

// ElifStatement inherits from SelectStatement
class ElIfStatement : public SelecStatement
{
private:
    std::unique_ptr<Expr> expression;

public:
    ElIfStatement(std::unique_ptr<Expr>, std::unique_ptr<Statement> statement);
    virtual ~ElIfStatement() = default;
};

// IterationStatement inherits from Statement
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
    IterStatement(std::unique_ptr<Declaration>, std::unique_ptr<ExprStatement>, std::unique_ptr<ExprStatement>, std::unique_ptr<Expr>, std::unique_ptr<Statement>);
    virtual ~IterStatement() = default;
};

// Expressions class hierarchy

class Expr : public Node
{
private:
    std::unique_ptr<Expr> expression;
    std::unique_ptr<AssignmentExp> assignment_expression;

public:
    Expr(std::unique_ptr<Expr>, std::unique_ptr<AssignmentExp>);
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
private:
    std::unique_ptr<ConditionalExp> conditional_expression;
    std::unique_ptr<AssignmentExp> assignment_expression;
    std::unique_ptr<UnaryExp> unary_expression;
    std::unique_ptr<AssignmentOperator> assignment_operator;
    // ConditionalExp *conditional_expression;
    // AssignmentExp *assignment_expression;
    // UnaryExp *unary_expression;
    // AssignmentOperator assignment_operator;
public:
    AssignmentExp(std::unique_ptr<ConditionalExp>, std::unique_ptr<AssignmentExp>, std::unique_ptr<UnaryExp>, std::unique_ptr<AssignmentOperator>, std::unique_ptr<Expr>, std::unique_ptr<AssignmentExp>);
    virtual ~AssignmentExp() = default;
};

class ConditionalExp : public Expr
{
private:
    std::unique_ptr<LogicalOrExp> logical_or_expression;
    std::unique_ptr<Expr> expression;
    std::unique_ptr<ConditionalExp> conditional_expression;

    // Expr *logical_or_expression;
    // Expr *expression;
    // Expr *conditional_expression;
public: 
    ConditionalExp(std::unique_ptr<LogicalOrExp>, std::unique_ptr<Expr>, std::unique_ptr<ConditionalExp>, std::unique_ptr<Expr>, std::unique_ptr<AssignmentExp>);
    virtual ~ConditionalExp() = default;
};
class LogicalOrExp : public Expr
{
private:
    std::unique_ptr<LogicalAndExp> logical_and_expression;
    std::unique_ptr<LogicalOrExp> logical_or_expression;
    // LogicalOrExp *logical_or_exp;
    // LogicalAndExp *logical_and_exp;
public:
    LogicalOrExp(std::unique_ptr<LogicalAndExp>, std::unique_ptr<LogicalOrExp>, std::unique_ptr<Expr>, std::unique_ptr<AssignmentExp>);
    virtual ~LogicalOrExp() = default;
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