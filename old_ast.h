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
class BinaryStatements;
class ExprStatement;
class CompStatement; // Singleton class
class SelecStatement;
class IfStatement;
class ElseStatement;
class ElifStatement;
class IterStatement;

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

class TypeSpecifier;

class Node
{
public:
    Node();
    virtual ~Node() = default;
    // virtual void print() = 0;
    // int row_num, col_num;
    // add codegen() function from llvm for IR gen
};

class Statement : public Node
{
public:
    Statement();
    virtual ~Statement() = default;
    // virtual void print();
};

// Statement obj = new ExpressionStatement();
class BinaryStatement : public Node
{
private:
    std::unique_ptr<Statement> statement;
    std::unique_ptr<Declaration> declaration;

public:
    BinaryStatement(std::unique_ptr<Statement>, std::unique_ptr<Declaration>);
    virtual ~BinaryStatement() = default;
};

class BinaryStatements : public Node
{
private:
    std::unique_ptr<BinaryStatement> binary_statement;
    std::unique_ptr<BinaryStatements> binary_statements;

public:
    BinaryStatements(std::unique_ptr<BinaryStatement>, std::unique_ptr<BinaryStatements>);
};

class ExprStatement : public Statement
{
private:
    std::unique_ptr<Expr> expression;

public:
    ExprStatement(std::unique_ptr<Expr>);
    virtual ~ExprStatement() = default;
};

// class CompStatement : public Statement
// // This will be a singleton class because there can be only one compound statement which
// // will be a bunch of binary_statement enclosed in {}.
// // Singleton will ease some of out work as during bottom up parsing, whenever yacc wants
// // to "pass" a declaration or statement to the compound statement, we can just pass it
// // to the singleton instance of CompStatement without worrying whether the instance
// // was created or not.
// {
// private:
//     std::unique_ptr<BinaryStatements> binary_statements;

//     // private constructor to make prevent instantiation
//     // CompStatement();

//     // The singleton instance. static because we only want to have one instance of CompStatement and we want to access it from static function get_instance()
//     static CompStatement *singleton_instance; // ToDo: can we make use of unique_ptr here?

// public:
//     virtual ~CompStatement() = default;
//     CompStatement();
//     // Function to get the singleton instance. static because we want to call it without an instance
//     static CompStatement *get_instance(); // ToDo: can we make use of unique_ptr here?

//     void add_binary_statement(std::unique_ptr<BinaryStatement>); // append the binary statement to the vector
// };

class CompStatement : public Statement
{
private:
    std::unique_ptr<BinaryStatements> binary_statements;

public:
    CompStatement(std::unique_ptr<BinaryStatements>);
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
    // Consteuctor for IfStatement class
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
private:
    std::unique_ptr<InclusiveOrExp> inclusive_or_expression;
    std::unique_ptr<LogicalAndExp> logical_and_expression;
    // LogicalAndExp *logical_and_exp;
    // InclusiveOrExp *inclusive_or_exp;
public:
    LogicalAndExp(std::unique_ptr<InclusiveOrExp>, std::unique_ptr<LogicalAndExp>, std::unique_ptr<Expr>, std::unique_ptr<AssignmentExp>);
    virtual ~LogicalAndExp() = default;
};

class InclusiveOrExp : public Expr
{
private:
    std::unique_ptr<ExclusiveOrExp> exclusive_or_expression;
    std::unique_ptr<InclusiveOrExp> inclusive_or_expression;
    // InclusiveOrExp *inclusive_or_exp;
    // ExclusiveOrExp *exclusive_or_exp;
public:
    InclusiveOrExp(std::unique_ptr<ExclusiveOrExp>, std::unique_ptr<InclusiveOrExp>, std::unique_ptr<Expr>, std::unique_ptr<AssignmentExp>);
    virtual ~InclusiveOrExp() = default;
};

class ExclusiveOrExp : public Expr
{
private:
    std::unique_ptr<AndExp> and_expression;
    std::unique_ptr<ExclusiveOrExp> exclusive_or_expression;
    // ExclusiveOrExp *exclusive_or_exp;
    // AndExp *and_exp;
public:
    ExclusiveOrExp(std::unique_ptr<AndExp>, std::unique_ptr<ExclusiveOrExp>, std::unique_ptr<Expr>, std::unique_ptr<AssignmentExp>);
    virtual ~ExclusiveOrExp() = default;
};

class AndExp : public Expr
{
private:
    std::unique_ptr<EqualityExp> equality_expression;
    std::unique_ptr<AndExp> and_expression;
    // AndExp *and_exp;
    // EqualityExp *equality_exp;
public:
    AndExp(std::unique_ptr<EqualityExp>, std::unique_ptr<AndExp>, std::unique_ptr<Expr>, std::unique_ptr<AssignmentExp>);
    virtual ~AndExp() = default;
};

class EqualityExp : public Expr
{
private:
    std::unique_ptr<RelationalExp> relational_expression;
    std::unique_ptr<EqualityExp> equality_expression;
    // EqualityExp *equality_exp;
    // RelationalExp *relational_exp;
    bool isEqualOp;

public:
    EqualityExp(std::unique_ptr<RelationalExp>, std::unique_ptr<EqualityExp>, std::unique_ptr<Expr>, std::unique_ptr<AssignmentExp>, bool);
    virtual ~EqualityExp() = default;
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
private:
    std::unique_ptr<ShiftExp> shift_expression;
    std::unique_ptr<RelationalExp> relational_expression;
    // RelationalExp *relational_exp;
    // ShiftExp *shift_exp;
    std::unique_ptr<RelationalOp> relational_op;
    // RelationalOp relational_op;
public:
    RelationalExp(std::unique_ptr<ShiftExp>, std::unique_ptr<RelationalExp>, std::unique_ptr<Expr>, std::unique_ptr<AssignmentExp>, std::unique_ptr<RelationalOp>);
    virtual ~RelationalExp() = default;
};

class ShiftExp : public Expr
{
private:
    std::unique_ptr<AdditiveExp> additive_expression;
    std::unique_ptr<ShiftExp> shift_expression;
    // ShiftExp *shift_exp;
    // AdditiveExp *additive_exp;
    bool isLeftShift;

public:
    ShiftExp(std::unique_ptr<AdditiveExp>, std::unique_ptr<ShiftExp>, std::unique_ptr<Expr>, std::unique_ptr<AssignmentExp>, bool);
    virtual ~ShiftExp() = default;
};

class AdditiveExp : public Expr
{
private:
    std::unique_ptr<MultiplicativeExp> multiplicative_expression;
    std::unique_ptr<AdditiveExp> additive_expression;
    // AdditiveExp *additive_exp;
    // MultiplicativeExp *multiplicative_exp;
    bool isAddition;

public:
    AdditiveExp(std::unique_ptr<MultiplicativeExp>, std::unique_ptr<AdditiveExp>, std::unique_ptr<Expr>, std::unique_ptr<AssignmentExp>, bool);
    virtual ~AdditiveExp() = default;
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
private:
    std::unique_ptr<CastExp> cast_expression;
    std::unique_ptr<MultiplicativeExp> multiplicative_expression;
    std::unique_ptr<MultiplicativeOp> multiplicative_op;
    // MultiplicativeExp *multiplicative_exp;
    // CastExp *cast_exp;
    // MultiplicativeOp multiplicative_op;
public:
    MultiplicativeExp(std::unique_ptr<CastExp>, std::unique_ptr<MultiplicativeExp>, std::unique_ptr<Expr>, std::unique_ptr<AssignmentExp>, std::unique_ptr<MultiplicativeOp>);
    virtual ~MultiplicativeExp() = default;
};

// enum class TypeSpecifier
// {
//     _CHAR,
//     _INT,
//     _LONG,
//     _FLOAT,
//     _BOOL,
//     _TENSOR
// };

class TypeSpecifier : public Node
{
private:
    std::string data_type;

public:
    TypeSpecifier(std::string);
    // void print();
};

class CastExp : public Expr
{
public:
    CastExp *cast_exp;
    UnaryExp *unary_exp;
    TypeSpecifier type_specifier;
};

// enum class LibFuncs
// {
//     GRAD,
//     COS,
//     SIN,
//     EXP,
//     LOG,
//     BACKWARD,
//     SIZEOF,
//     PRINT
// };

class LibFuncs : public Node
{
private:
    std::string lib_func;

public:
    LibFuncs(std::string);
    // void print();
};

// enum class UnaryOp
// {
//     AND,
//     PLUS,
//     MINUS,
//     TILDA,
//     EXCLAMATION,
//     AT
// };

class UnaryOp : public Node
{
private:
    std::string unary_op;

public:
    UnaryOp(std::string);
    // void print();
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

// enum class ConstantType
// {
//     INT,
//     FLOAT,
//     CHAR,
// };

class ConstantType : public Node
{
private:
    std::string constant_type;

public:
    ConstantType(std::string);
    // void print();
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
    std::unique_ptr<DeclarationType> declaration_type;
    std::unique_ptr<InitDeclarators> init_declarators;
    Declaration(std::unique_ptr<DeclarationType>, std::unique_ptr<InitDeclarators>);
};

// enum class GradSpecifier
// {
//     CNS,
//     VAR
// };

class GradSpecifier : public Node
{
private:
    std::string grad_specifier;

public:
    GradSpecifier(std::string);
    // void print();
};

class DeclarationType : public Node
{
public:
    std::unique_ptr<TypeSpecifier> type_specifier;
    std::unique_ptr<GradSpecifier> grad_specifier;
    DeclarationType(std::unique_ptr<TypeSpecifier>, std::unique_ptr<GradSpecifier>);
};

class InitDeclarator : public Node
{
public:
    // Declarator *declarator;
    // Initializer *initializer;
    std::unique_ptr<Declarator> declarator;
    std::unique_ptr<Initializer> initializer;
    InitDeclarator(std::unique_ptr<Declarator>, std::unique_ptr<Initializer>);
};

class InitDeclarators : public Node
{
public:
    std::unique_ptr<InitDeclarators> init_declarators;
    std::unique_ptr<InitDeclarator> init_declarator;

    InitDeclarators(std::unique_ptr<InitDeclarators>, std::unique_ptr<InitDeclarator>);
};

class Declarator : public Node
{
public:
    std::string identifier;
    std::unique_ptr<ConditionalExp> conditional_exp;
    std::unique_ptr<Declarator> declarator;
    Declarator(std::string, std::unique_ptr<ConditionalExp>, std::unique_ptr<Declarator>);
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