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

Statement::Statement()
{
    std::cout << "Statement::Statement()" << std::endl;
}

Node::Node()
{
    std::cout << "Node::Node()" << std::endl;
}

// class Statement

// class BinaryStatement
BinaryStatement::BinaryStatement(std::unique_ptr<Statement> statement, std::unique_ptr<Declaration> declaration)
{
    this->statement = std::move(statement);
    this->declaration = std::move(declaration);
}

// class CompStatement
CompStatement::CompStatement()
{
    binary_statements = std::vector<std::unique_ptr<BinaryStatement>>();
}

CompStatement *CompStatement::singleton_instance = nullptr;
CompStatement *CompStatement::get_instance()
{
    if (singleton_instance == nullptr)
    {
        singleton_instance = new CompStatement();
    }
    return singleton_instance;
}

void CompStatement::add_binary_statement(std::unique_ptr<BinaryStatement> binary_statement)
{
    this->binary_statements.push_back(std::move(binary_statement));
}

// class ExprStatement
ExprStatement::ExprStatement(std::unique_ptr<Expr> expression)
{
    this->expression = std::move(expression);
}

// class SelecStatement
SelecStatement::SelecStatement(std::unique_ptr<Statement> statement)
{
    this->statement = std::move(statement);
}

// class IfStatement
IfStatement::IfStatement(std::unique_ptr<Expr> expression, std::unique_ptr<Statement> statement) : SelecStatement(std::move(statement))
{
    this->expression = std::move(expression);
}

// class ElseStatement
ElseStatement::ElseStatement(std::unique_ptr<Statement> statement) : SelecStatement(std::move(statement))
{
}

// class ElIfStatement
ElIfStatement::ElIfStatement(std::unique_ptr<Expr> expression, std::unique_ptr<Statement> statement) : SelecStatement(std::move(statement))
{
    this->expression = std::move(expression);
}

// class IterStatement
IterStatement::IterStatement(std::unique_ptr<Declaration> declaration, std::unique_ptr<ExprStatement> init_expression, std::unique_ptr<ExprStatement> test_statement, std::unique_ptr<Expr> update_expreession, std::unique_ptr<Statement> loop_statement)
{
    this->declaration = std::move(declaration);
    this->init_expression = std::move(init_expression);
    this->test_statement = std::move(test_statement);
    this->update_expreession = std::move(update_expreession);
    this->loop_statement = std::move(loop_statement);
}

// class Expr
Expr::Expr(std::unique_ptr<Expr> expression, std::unique_ptr<AssignmentExp> assignment_expression)
{
    this->expression = std::move(expression);
    this->assignment_expression = std::move(assignment_expression);
}

// class AssignmentExp
AssignmentExp::AssignmentExp(std::unique_ptr<ConditionalExp> conditional_expression, std::unique_ptr<AssignmentExp> assignment_expression, std::unique_ptr<UnaryExp> unary_expression, std::unique_ptr<AssignmentOperator> assignment_operator, std::unique_ptr<Expr> expr_expr, std::unique_ptr<AssignmentExp> assignment_expression2) : Expr(std::move(expr_expr), std::move(assignment_expression2))
{
    this->conditional_expression = std::move(conditional_expression);
    this->assignment_expression = std::move(assignment_expression);
    this->unary_expression = std::move(unary_expression);
    this->assignment_operator = std::move(assignment_operator);
}

// class ConditionalExp
ConditionalExp::ConditionalExp(std::unique_ptr<LogicalOrExp> logical_or_expression, std::unique_ptr<Expr> expression, std::unique_ptr<ConditionalExp> conditional_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expression) : Expr(std::move(expr), std::move(assignment_expression))
{
    this->logical_or_expression = std::move(logical_or_expression);
    this->expression = std::move(expression);
    this->conditional_expression = std::move(conditional_expression);
}

// class LogicalOrExp
LogicalOrExp::LogicalOrExp(std::unique_ptr<LogicalAndExp> logical_and_expression, std::unique_ptr<LogicalOrExp> logical_or_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->logical_and_expression = std::move(logical_and_expression);
    this->logical_or_expression = std::move(logical_or_expression);
}

// class LogicalAndExp
LogicalAndExp::LogicalAndExp(std::unique_ptr<InclusiveOrExp> inclusive_or_expression, std::unique_ptr<LogicalAndExp> logical_and_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->inclusive_or_expression = std::move(inclusive_or_expression);
    this->logical_and_expression = std::move(logical_and_expression);
}

// class InclusiveOrExp
InclusiveOrExp::InclusiveOrExp(std::unique_ptr<ExclusiveOrExp> exclusive_or_expression, std::unique_ptr<InclusiveOrExp> inclusive_or_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->exclusive_or_expression = std::move(exclusive_or_expression);
    this->inclusive_or_expression = std::move(inclusive_or_expression);
}

// class ExclusiveOrExp
ExclusiveOrExp::ExclusiveOrExp(std::unique_ptr<AndExp> and_expression, std::unique_ptr<ExclusiveOrExp> exclusive_or_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->and_expression = std::move(and_expression);
    this->exclusive_or_expression = std::move(exclusive_or_expression);
}

// class AndExp
AndExp::AndExp(std::unique_ptr<EqualityExp> equality_expression, std::unique_ptr<AndExp> and_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->equality_expression = std::move(equality_expression);
    this->and_expression = std::move(and_expression);
}

// class EqualityExp
EqualityExp::EqualityExp(std::unique_ptr<RelationalExp> relational_expression, std::unique_ptr<EqualityExp> equality_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr, bool isEqualOp = false) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->relational_expression = std::move(relational_expression);
    this->equality_expression = std::move(equality_expression);
    this->isEqualOp = isEqualOp;
}

// class RelationalExp
RelationalExp::RelationalExp(std::unique_ptr<ShiftExp> shift_expression, std::unique_ptr<RelationalExp> relational_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr, std::unique_ptr<RelationalOp> relational_op) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->shift_expression = std::move(shift_expression);
    this->relational_expression = std::move(relational_expression);
    this->relational_op = std::move(relational_op);
}

// class ShiftExp
ShiftExp::ShiftExp(std::unique_ptr<AdditiveExp> additive_expression, std::unique_ptr<ShiftExp> shift_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr, bool isLeftShift = false) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->additive_expression = std::move(additive_expression);
    this->shift_expression = std::move(shift_expression);
    this->isLeftShift = isLeftShift;
}

// class AdditiveExp
AdditiveExp::AdditiveExp(std::unique_ptr<MultiplicativeExp> multiplicative_expression, std::unique_ptr<AdditiveExp> additive_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr, bool isAddition = false) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->multiplicative_expression = std::move(multiplicative_expression);
    this->additive_expression = std::move(additive_expression);
    this->isAddition = isAddition;
}

// class MultiplicativeExp
MultiplicativeExp::MultiplicativeExp(std::unique_ptr<CastExp> cast_expression, std::unique_ptr<MultiplicativeExp> multiplicative_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr, std::unique_ptr<MultiplicativeOp> multiplicative_op) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->cast_expression = std::move(cast_expression);
    this->multiplicative_expression = std::move(multiplicative_expression);
    this->multiplicative_op = std::move(multiplicative_op);
}