#include "ast.h"
// #include <iostream>

Node::Node()
{
    // std::cout << "Node::Node()" << std::endl;
}

Start::Start(std::vector<class Decl *> *DeclList, std::vector<class AssgnStmt *> *AssgnStmtList, std::vector<class GradStmt *> *GradStmtList, std::unordered_map<std::string, SymTabItem> *symbolTable)
{
    this->DeclList = DeclList;
    this->AssgnStmtList = AssgnStmtList;
    this->GradStmtList = GradStmtList;
    this->symbolTable = symbolTable;
}

Decl::Decl(GradSpecifier GradType, TypeSpecifier DataType, InitDeclarator *InitDeclaratorList)
{
    this->GradType = GradType;
    this->DataType = DataType;
    this->InitDeclaratorList = InitDeclaratorList;
}

InitDeclarator::InitDeclarator(Declarator *declarator, Initializer *initializer = NULL)
{
    this->declarator = declarator;
    this->initializer = initializer;
}

Declarator::Declarator(std::string name)
{
    this->name = name;
    // this->Dimensions = Dimensions;
}

ConstValue::ConstValue(int value)
{
    this->isInt = true;
    this->value.int_val = value;
}

ConstValue::ConstValue(float value)
{
    this->isInt = false;
    this->value.float_val = value;
}

Initializer::Initializer(ConstValue *value)
{
    this->val.cvalue = value;
    this->isScalar = true;
}

Initializer::Initializer(std::vector<Initializer *> *InitializerList)
{
    this->val.InitializerList = InitializerList;
    this->isScalar = false;
}

void Initializer::printInitializerList()
{
    // std::cout << "InitializerList" << std::endl;
    if (this->isScalar)
    {
        std::cout << "Scalar ";
        if (this->val.cvalue->isInt)
        {
            std::cout << this->val.cvalue->value.int_val << std::endl;
        }
        else
        {
            std::cout << this->val.cvalue->value.float_val << std::endl;
        }
    }
    else
    {
        std::cout << "Array" << std::endl;
        for (auto i : *this->val.InitializerList)
        {
            i->printInitializerList();
        }
    }
}

// void print_init_list_tree(Initializer *obj)
// {
// }

AssgnStmt::AssgnStmt(std::string name, std::optional<AssignmentOperator> op, Expr *expr)
{
    this->name = name;
    this->op = op;
    this->expr = expr;
}

Expr::Expr()
{
}

void Expr::printExpression() {}

BinaryExpr::BinaryExpr(Expr *lhs, Expr *rhs, char op)
{
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = op;
}

std::string optofunc(char c){
    switch(c){
        case '+':
            return "_g._add";
        case '-':
            return "_g._sub";
        case '*':
            return "_g._mul";
        case '/':
            return "_g._div";
        case '^':
            return "_g._pow";
        case '@':
            return "_g._matmul";
        default:
            return "_g._error";
    }
}

void BinaryExpr::printExpression()
{
    std::cout << "(";
    this->lhs->printExpression();
    std::cout << " " << this->op << " ";
    this->rhs->printExpression();
    std::cout << ")";
}

void BinaryExpr::transpile(std::ostream &out, int tab) const
{
    out << " " << optofunc( this->op );
    out << "(";
    this->lhs->transpile(out, tab);
    out << ",";
    this->rhs->transpile(out, tab);
    out << ")";
}

UnaryExpr::UnaryExpr(Expr *expr, std::optional<LibFuncs> libfunc, std::string identifier, ConstValue *cvalue)
{
    this->expr = expr;
    this->libfunc = libfunc;
    this->identifier = identifier;
    this->cvalue = cvalue;
}

void UnaryExpr::printExpression()
{
    if (this->identifier != "")
    {
        std::cout << this->identifier;
    }
    else if (this->cvalue != nullptr)
    {
        if (this->cvalue->isInt)
        {
            std::cout << this->cvalue->value.int_val;
        }
        else
        {
            std::cout << this->cvalue->value.float_val;
        }
    }
    else
    {
        std::cout << "(";
        switch (this->libfunc.value())
        {
            {
            case LibFuncs::SIN:
                std::cout << "sin(";
                break;
            case LibFuncs::COS:
                std::cout << "cos(";
                break;

            default:
                std::cout << "Invalid libfunc";
                break;
            }
        }
        this->expr->printExpression();
        std::cout << ")";
    }
}

void UnaryExpr::transpile(std::ostream &out, int tab) const
{
    if (this->identifier != "")
    {
        out << this->identifier;
    }
    else if (this->cvalue != nullptr)
    {
        if (this->cvalue->isInt)
        {
            out << this->cvalue->value.int_val;
        }
        else
        {
            out << this->cvalue->value.float_val;
        }
    }
    else
    {
        // out << "(";
        switch (this->libfunc.value())
        {
            {
            case LibFuncs::SIN:
                out << "_g._sin";
                break;
            case LibFuncs::COS:
                out << "-g._cos";
                break;

            default:
                out << "Invalid libfunc";
                break;
            }
        }
        out << "(";
        this->expr->transpile(out, tab);
        out << ")";
        // out << ")";
    }
}

GradStmt::GradStmt(GradType grad_type, std::string name)
{
    this->grad_type = grad_type;
    this->name = name;
}

// Transpiler
std::map<GradSpecifier, std::string> GradSpecifierMap = {
    {GradSpecifier::CNS, "CNS"},
    {GradSpecifier::VAR, "VAR"}};

std::map<GradSpecifier, std::string> GradSpecifierMapCpp = {
    {GradSpecifier::CNS, "const"},
    {GradSpecifier::VAR, ""}};

std::map<TypeSpecifier, std::string> TypeSpecifierMap = {
    {TypeSpecifier::CHAR, "CHAR"},
    {TypeSpecifier::INT, "INT"},
    {TypeSpecifier::FLOAT, "FLOAT"},
    {TypeSpecifier::BOOL, "BOOL"},
    {TypeSpecifier::TENSOR, "TENSOR"}};

std::map<TypeSpecifier, std::string> TypeSpecifierMapCpp = {
    {TypeSpecifier::CHAR, "char"},
    {TypeSpecifier::INT, "int"},
    {TypeSpecifier::FLOAT, "float"},
    {TypeSpecifier::BOOL, "bool"},
    {TypeSpecifier::TENSOR, "Tensor"}};

std::map<GradType, std::string> GradTypeMapCpp = {
    {GradType::GRAD, "grad"},
    {GradType::BACKWARD, "backward"}};

std::map<AssignmentOperator, std::string> AssignmentOperatorMapCpp = {
    {AssignmentOperator::AST_ASSIGN, "="},
    {AssignmentOperator::AST_ADD_ASSIGN, "+="},
    {AssignmentOperator::AST_SUB_ASSIGN, "-="},
    {AssignmentOperator::AST_MUL_ASSIGN, "*="},
    {AssignmentOperator::AST_DIV_ASSIGN, "/="},
    {AssignmentOperator::AST_AT_ASSIGN, "@="}};

void Start::transpile(std::ostream &out, int tab ) const {
    out << "#include <iostream>" << std::endl;
    out << "#include \"Graph.h\"" << std::endl << std::endl;
    out << "using namespace std;" << std::endl << std::endl;
    out << "int main() {" << std::endl;
    out << std::string("\t",tab+1) <<"Graph _g;" << std::endl;

    for (auto i : *this->DeclList) {
        i->transpile(out, tab + 1);
    }

    for(auto i : *this->AssgnStmtList) {
        i->transpile(out, tab + 1);
    }

    for(auto i : *this->GradStmtList) {
        i->transpile(out, tab + 1);
    }

    out << std::string("\t",tab+1) <<"return 0;" << std::endl;
    out << "}" << std::endl;

}

void Decl::transpile(std::ostream &out, int tab) const {
    out << std::string("\t", tab)
        << "Node& " << this->InitDeclaratorList->declarator->name
        << " = "
        << "_g.";

        switch( this->GradType){
            case GradSpecifier::CNS:
                out << "_constant";
                break;
            case GradSpecifier::VAR:
                out << "_variable";
                break;
        }

        out << "(";

        this->InitDeclaratorList->transpile(out, tab);

        out << ");" << std::endl;
}

void InitDeclarator::transpile(std::ostream &out, int tab) const {
    if( this->declarator != nullptr ) {
        this->declarator->transpile(out, tab);
    }

    if( this->initializer != nullptr ) {
        out << ", ";
        this->initializer->transpile(out, tab);
    }

}


void Declarator::transpile(std::ostream &out, int tab) const {
    // out << this->name;
    if( !this->Dimensions.empty()){

        for(int i = 0; i < this->Dimensions.size(); i++) {
            out << this->Dimensions[i];
            if( i != this->Dimensions.size() - 1 ) {
                out << ", ";
            }
        }

    }

}

void Initializer::transpile(std::ostream &out, int tab) const {
    if (this->isScalar)
    {
        if( this->val.cvalue->isInt ) {
            out << this->val.cvalue->value.int_val;
        } else {
            out << this->val.cvalue->value.float_val;
        }
    }
    else
    {
        out << "{";
        for( auto i : *this->val.InitializerList ) {
            i->transpile(out, tab);
            if( i != this->val.InitializerList->back() ) {
                out << ", ";
            }
            // out << ",";
        }
        out << "}";
    }
}


void AssgnStmt::transpile(std::ostream &out, int tab) const {
    out << std::string("\t", tab);

    if(op.has_value()){
        out << this->name << " " << AssignmentOperatorMapCpp[op.value()] << " ";
    }
    else{
        out << this->name << " = ";
    }

    this->expr->transpile(out, tab);
    // this->lhs->transpile(out, tab);
    // out << " = ";
    // this->rhs->transpile(out, tab);
    
    out << ";" << std::endl;

}

void Expr::transpile(std::ostream &out, int tab) const {

}

void GradStmt::transpile(std::ostream &out, int tab) const {
    out << std::string("\t", tab)<< "_g." << GradTypeMapCpp[this->grad_type] << "(" << this->name << ");" << std::endl;
}

// int main()
// {
//     return 0;
// }
