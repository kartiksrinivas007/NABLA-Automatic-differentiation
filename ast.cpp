#include "ast.h"
// #include <llvm/ADT/APFloat.h>
// #include <llvm-c-14/llvm-c/Core.h>

Node::Node()
{
    std::cout << "Node::Node()" << std::endl;
}

// class Statement

Statement::Statement()
{
    std::cout << "Statement::Statement()" << std::endl;
}

void Statement::print()
{
    std::cout << "[Incomplete] Print statement\n";
}

BinaryStatement::BinaryStatement(std::unique_ptr<Statement> statement, std::unique_ptr<Declaration> declaration)
{
    this->statement = std::move(statement);
    this->declaration = std::move(declaration);
}

BinaryStatements::BinaryStatements(std::unique_ptr<BinaryStatement> bs, std::unique_ptr<BinaryStatements> bss)
{
    this->binary_statement = std::move(bs);
    this->binary_statements = std::move(bss);
}

CompStatement::CompStatement()
{
    // binary_statements = std::vector<std::unique_ptr<BinaryStatement>>();
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
    // this->binary_statements.push_back(std::move(binary_statement));
}

ExprStatement::ExprStatement(std::unique_ptr<Expr> expression)
{
    this->expression = std::move(expression);
}

SelecStatement::SelecStatement(std::unique_ptr<Statement> statement)
{
    this->statement = std::move(statement);
}

IfStatement::IfStatement(std::unique_ptr<Expr> expression, std::unique_ptr<Statement> statement) : SelecStatement(std::move(statement))
{
    this->expression = std::move(expression);
}

ElseStatement::ElseStatement(std::unique_ptr<Statement> statement) : SelecStatement(std::move(statement))
{
}

ElIfStatement::ElIfStatement(std::unique_ptr<Expr> expression, std::unique_ptr<Statement> statement) : SelecStatement(std::move(statement))
{
    this->expression = std::move(expression);
}

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

AssignmentExp::AssignmentExp(std::unique_ptr<ConditionalExp> conditional_expression, std::unique_ptr<AssignmentExp> assignment_expression, std::unique_ptr<UnaryExp> unary_expression, std::unique_ptr<AssignmentOperator> assignment_operator, std::unique_ptr<Expr> expr_expr, std::unique_ptr<AssignmentExp> assignment_expression2) : Expr(std::move(expr_expr), std::move(assignment_expression2))
{
    this->conditional_expression = std::move(conditional_expression);
    this->assignment_expression = std::move(assignment_expression);
    this->unary_expression = std::move(unary_expression);
    this->assignment_operator = std::move(assignment_operator);
}

ConditionalExp::ConditionalExp(std::unique_ptr<LogicalOrExp> logical_or_expression, std::unique_ptr<Expr> expression, std::unique_ptr<ConditionalExp> conditional_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expression) : Expr(std::move(expr), std::move(assignment_expression))
{
    this->logical_or_expression = std::move(logical_or_expression);
    this->expression = std::move(expression);
    this->conditional_expression = std::move(conditional_expression);
}

LogicalOrExp::LogicalOrExp(std::unique_ptr<LogicalAndExp> logical_and_expression, std::unique_ptr<LogicalOrExp> logical_or_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->logical_and_expression = std::move(logical_and_expression);
    this->logical_or_expression = std::move(logical_or_expression);
}

LogicalAndExp::LogicalAndExp(std::unique_ptr<InclusiveOrExp> inclusive_or_expression, std::unique_ptr<LogicalAndExp> logical_and_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->inclusive_or_expression = std::move(inclusive_or_expression);
    this->logical_and_expression = std::move(logical_and_expression);
}

InclusiveOrExp::InclusiveOrExp(std::unique_ptr<ExclusiveOrExp> exclusive_or_expression, std::unique_ptr<InclusiveOrExp> inclusive_or_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->exclusive_or_expression = std::move(exclusive_or_expression);
    this->inclusive_or_expression = std::move(inclusive_or_expression);
}

ExclusiveOrExp::ExclusiveOrExp(std::unique_ptr<AndExp> and_expression, std::unique_ptr<ExclusiveOrExp> exclusive_or_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->and_expression = std::move(and_expression);
    this->exclusive_or_expression = std::move(exclusive_or_expression);
}

AndExp::AndExp(std::unique_ptr<EqualityExp> equality_expression, std::unique_ptr<AndExp> and_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->equality_expression = std::move(equality_expression);
    this->and_expression = std::move(and_expression);
}

EqualityExp::EqualityExp(std::unique_ptr<RelationalExp> relational_expression, std::unique_ptr<EqualityExp> equality_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr, bool isEqualOp = false) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->relational_expression = std::move(relational_expression);
    this->equality_expression = std::move(equality_expression);
    this->isEqualOp = isEqualOp;
}

RelationalExp::RelationalExp(std::unique_ptr<ShiftExp> shift_expression, std::unique_ptr<RelationalExp> relational_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr, std::unique_ptr<RelationalOp> relational_op) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->shift_expression = std::move(shift_expression);
    this->relational_expression = std::move(relational_expression);
    this->relational_op = std::move(relational_op);
}

ShiftExp::ShiftExp(std::unique_ptr<AdditiveExp> additive_expression, std::unique_ptr<ShiftExp> shift_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr, bool isLeftShift = false) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->additive_expression = std::move(additive_expression);
    this->shift_expression = std::move(shift_expression);
    this->isLeftShift = isLeftShift;
}

AdditiveExp::AdditiveExp(std::unique_ptr<MultiplicativeExp> multiplicative_expression, std::unique_ptr<AdditiveExp> additive_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr, bool isAddition = false) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->multiplicative_expression = std::move(multiplicative_expression);
    this->additive_expression = std::move(additive_expression);
    this->isAddition = isAddition;
}

MultiplicativeExp::MultiplicativeExp(std::unique_ptr<CastExp> cast_expression, std::unique_ptr<MultiplicativeExp> multiplicative_expression, std::unique_ptr<Expr> expr, std::unique_ptr<AssignmentExp> assignment_expr, std::unique_ptr<MultiplicativeOp> multiplicative_op) : Expr(std::move(expr), std::move(assignment_expr))
{
    this->cast_expression = std::move(cast_expression);
    this->multiplicative_expression = std::move(multiplicative_expression);
    this->multiplicative_op = std::move(multiplicative_op);
}

// // // //

DeclarationType::DeclarationType(std::unique_ptr<TypeSpecifier> ts, std::unique_ptr<GradSpecifier> gs = nullptr)
{
    this->type_specifier = std::move(ts);
    this->grad_specifier = std::move(gs);
}

Declarator::Declarator(std::string s, std::unique_ptr<ConditionalExp> ce = nullptr, std::unique_ptr<Declarator> d = nullptr)
{
    this->identifier = s;
    this->conditional_exp = std::move(ce);
    this->declarator = std::move(d);
}

InitDeclarators::InitDeclarators(std::unique_ptr<InitDeclarators> ids, std::unique_ptr<InitDeclarator> id)
{
    this->init_declarator = std::move(id);
    this->init_declarators = std::move(ids);
}

InitDeclarator::InitDeclarator(std::unique_ptr<Declarator> d, std::unique_ptr<Initializer> i = nullptr)
{
    this->declarator = std::move(d);
    this->initializer = std::move(i);
}

Declaration::Declaration(std::unique_ptr<DeclarationType> dt, std::unique_ptr<InitDeclarators> ids)
{
    this->declaration_type = std::move(dt);
    this->init_declarators = std::move(ids);
}

TypeSpecifier::TypeSpecifier(std::string s)
{
    this->data_type = s;
}

int main()
{
    return 0;
}

void TypeSpecifier::print()
{
    std::cout << data_type << std::endl;
}