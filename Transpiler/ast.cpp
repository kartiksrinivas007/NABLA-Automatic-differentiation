#include "ast.h"
#include <algorithm>
// #include <iostream>

template <typename S>
std::ostream &operator<<(std::ostream &os, const std::vector<S> &vector)
{
    // Printing all the elements
    // using <<
    for (auto element : vector)
    {
        os << element << " ";
    }
    os << "\n";
    return os;
}

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
    this->dimensions = std::vector<int>();
}

void Expr::printExpression() {}

void Expr::initialize_expression_node_info(std::unordered_map<std::string, SymTabItem> *symbolTable) {}

BinaryExpr::BinaryExpr(Expr *lhs, Expr *rhs, char op)
{
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = op;
}

std::string optofunc(char c)
{
    switch (c)
    {
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

void BinaryExpr::initialize_expression_node_info(std::unordered_map<std::string, SymTabItem> *symbolTable)
{
    std::cout << "BinaryExpr::initialize_expression_node_info: " << this->op << std::endl;

    std::cout << "lhs: " << std::endl;
    this->lhs->initialize_expression_node_info(symbolTable);
    std::cout << "rhs: " << std::endl;
    this->rhs->initialize_expression_node_info(symbolTable);

    if (this->op == '+' || this->op == '-')
    {
        // First check if both are tensors
        // Then if one is a tensor and other is a scalar
        // Then if both are int or float
        if (this->lhs->DataType == TypeSpecifier::TENSOR && this->rhs->DataType == TypeSpecifier::TENSOR)
        {
            // both are tensors
            if (this->lhs->dimensions != this->rhs->dimensions)
            {
                std::cout << "Fatal: Dimensions of operands for '+/-' do not match" << std::endl;
                exit(1);
            }
            this->DataType = TypeSpecifier::TENSOR;
            this->dimensions = this->lhs->dimensions;
        }
        else if ((this->lhs->DataType == TypeSpecifier::TENSOR && this->rhs->DataType != TypeSpecifier::TENSOR) || (this->lhs->DataType != TypeSpecifier::TENSOR && this->rhs->DataType == TypeSpecifier::TENSOR))
        {
            // one is a tensor and other is a int or float
            std::cout << "Fatal: Tensor and scalar addition/subtraction not supported" << std::endl;
            exit(1);
        }
        else
        {
            // if both are int or float

            // if both ints then int otherwise float
            if (this->lhs->DataType == TypeSpecifier::INT && this->rhs->DataType == TypeSpecifier::INT)
            {
                this->DataType = TypeSpecifier::INT;
            }
            else
            {
                this->DataType = TypeSpecifier::FLOAT;
            }
        }
    }
    else if (this->op == '*')
    {
        // if both ints then int otherwise float
        if (this->lhs->DataType == TypeSpecifier::INT && this->rhs->DataType == TypeSpecifier::INT)
        {
            this->DataType = TypeSpecifier::INT;
        }
        else if (this->lhs->DataType == TypeSpecifier::FLOAT && this->rhs->DataType == TypeSpecifier::FLOAT)
        {
            this->DataType = TypeSpecifier::FLOAT;
        }
        else if (this->lhs->DataType == TypeSpecifier::TENSOR && this->rhs->DataType == TypeSpecifier::TENSOR)
        {
            this->DataType = TypeSpecifier::TENSOR;
            if (this->lhs->dimensions != this->rhs->dimensions)
            {
                std::cout << "Fatal: Dimensions of operands for '*' do not match" << std::endl;
                exit(1);
            }
            this->dimensions = this->lhs->dimensions;
        }
        else if (this->lhs->DataType == TypeSpecifier::TENSOR && this->rhs->DataType != TypeSpecifier::TENSOR)
        {
            this->DataType = TypeSpecifier::TENSOR;
            this->dimensions = this->lhs->dimensions;
        }
        else if (this->lhs->DataType != TypeSpecifier::TENSOR && this->rhs->DataType == TypeSpecifier::TENSOR)
        {
            this->DataType = TypeSpecifier::TENSOR;
            this->dimensions = this->rhs->dimensions;
        }
        else if (this->lhs->DataType == TypeSpecifier::FLOAT || this->rhs->DataType != TypeSpecifier::FLOAT)
        {
            this->DataType = TypeSpecifier::FLOAT;
        }
        else
        {
            std::cout << "Fatal: Invalid operands for '*'" << std::endl;
            exit(1);
        }
    }
    else if (this->op == '/')
    {
        if (this->rhs->DataType == TypeSpecifier::TENSOR)
        {
            std::cout << "Fatal: Anything divided by Tensor is not supported" << std::endl;
            exit(1);
        }

        if (this->lhs->DataType == TypeSpecifier::TENSOR)
        {
            this->DataType = TypeSpecifier::TENSOR;
            this->dimensions = this->lhs->dimensions;
        }
        else if (this->lhs->DataType == TypeSpecifier::INT && this->rhs->DataType == TypeSpecifier::INT)
        {
            this->DataType = TypeSpecifier::INT;
        }
        else
        {
            this->DataType = TypeSpecifier::FLOAT;
        }
    }
    else if (this->op == '@')
    {
        if (this->lhs->DataType != TypeSpecifier::TENSOR || this->rhs->DataType != TypeSpecifier::TENSOR)
        {
            std::cout << "Fatal: @ only supported on Tensors" << std::endl;
            exit(1);
        }
        else
        {
            if (this->lhs->dimensions.back() != this->rhs->dimensions.front())
            {
                std::cout << "Fatal: Dimensions of operands for '@' do not match" << std::endl;
                exit(1);
            }
            this->DataType = TypeSpecifier::TENSOR;
            for (int i = 0; i < this->lhs->dimensions.size() - 1; i++)
            {
                this->dimensions.push_back(this->lhs->dimensions[i]);
            }
            for (int i = 1; i < this->rhs->dimensions.size(); i++)
            {
                this->dimensions.push_back(this->rhs->dimensions[i]);
            }
        }
    }
    else
    {
        std::cout << "Fatal: Unknown operator(Shouldn't have reached here)" << std::endl;
        exit(1);
    }
}

void BinaryExpr::transpile(std::ostream &out, int tab) const
{
    out << " " << optofunc(this->op);
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

void UnaryExpr::initialize_expression_node_info(std::unordered_map<std::string, SymTabItem> *symbolTable)
{
    std::cout << "Entered unary expr" << std::endl;
    if (this->identifier != "")
    {
        // Identifier
        std::cout << "Identifier: " << this->identifier << std::endl;
        SymTabItem *symTabItem = search(symbolTable, this->identifier);
        std::cout << "Here\n";
        if (symTabItem == NULL)
        {
            std::cout << "Fatal: Variable " << this->identifier << " not found" << std::endl;
            exit(0);
        }

        if (symTabItem->dataType == "Tensor")
        {
            this->DataType = TypeSpecifier::TENSOR;
            this->dimensions = symTabItem->Dims;
        }
        else if (symTabItem->dataType == "int")
        {
            this->DataType = TypeSpecifier::INT;
        }
        else if (symTabItem->dataType == "float")
        {
            this->DataType = TypeSpecifier::FLOAT;
        }
        else
        {
            std::cout << "Fatal: Unknown datatype of " << this->identifier << std::endl;
            exit(1);
        }
    }
    else if (this->cvalue != nullptr)
    {
        // Constant
        if (this->cvalue->isInt)
        {
            this->DataType = TypeSpecifier::INT;
        }
        else
        {
            this->DataType = TypeSpecifier::FLOAT;
        }
    }
    else
    {
        // libfunc
        this->expr->initialize_expression_node_info(symbolTable);
        this->DataType = this->expr->DataType;
        if (this->libfunc == LibFuncs::TRANSPOSE)
        {
            this->dimensions = this->expr->dimensions;
            std::reverse(this->dimensions.begin(), this->dimensions.end());
        }
        else
        {
            // SIN COS....
            this->dimensions = this->expr->dimensions;
        }
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
            case LibFuncs::TRANSPOSE:
                out << "_g._trans";
                break;
            case LibFuncs::EXP:
                out << "_g._exp";
                break;
            case LibFuncs::LOG:
                out << "_g._log";
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

void Start::transpile(std::ostream &out, int tab) const
{
    out << "#include <iostream>" << std::endl;
    out << "#include \"../include/Graph.h\"" << std::endl
        << std::endl;
    out << "using namespace std;" << std::endl
        << std::endl;
    out << "int main() {" << std::endl;
    out << std::string("\t", tab + 1) << "Graph _g;" << std::endl;

    for (auto i : *this->DeclList)
    {
        i->transpile(out, tab + 1);
    }

    for (auto i : *this->AssgnStmtList)
    {
        i->transpile(out, tab + 1);
    }

    for (auto i : *this->GradStmtList)
    {
        i->transpile(out, tab + 1);
    }

    out << std::string("\t", tab + 1) << "return 0;" << std::endl;
    out << "}" << std::endl;
}

void Decl::transpile(std::ostream &out, int tab) const
{
    out << std::string("\t", tab)
        << "Node* " << this->InitDeclaratorList->declarator->name
        << " = "
        << "_g.";

    switch (this->GradType)
    {
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

void InitDeclarator::transpile(std::ostream &out, int tab) const
{
    if (this->declarator != nullptr)
    {
        this->declarator->transpile(out, tab);
    }

    if (this->initializer != nullptr)
    {
        out << ", ";
        this->initializer->transpile(out, tab);
    }
}

void Declarator::transpile(std::ostream &out, int tab) const
{
    // out << this->name;
    if (!this->Dimensions.empty())
    {

        for (int i = 0; i < this->Dimensions.size(); i++)
        {
            out << this->Dimensions[i];
            if (i != this->Dimensions.size() - 1)
            {
                out << ", ";
            }
        }
    }
}

void Initializer::transpile(std::ostream &out, int tab) const
{
    if (this->isScalar)
    {
        if (this->val.cvalue->isInt)
        {
            out << this->val.cvalue->value.int_val;
        }
        else
        {
            out << this->val.cvalue->value.float_val;
        }
    }
    else
    {
        out << "{";
        for (auto i : *this->val.InitializerList)
        {
            i->transpile(out, tab);
            if (i != this->val.InitializerList->back())
            {
                out << ", ";
            }
            // out << ",";
        }
        out << "}";
    }
}

void AssgnStmt::transpile(std::ostream &out, int tab) const
{
    out << std::string("\t", tab);

    if (op.has_value())
    {
        out << this->name << " " << AssignmentOperatorMapCpp[op.value()] << " ";
    }
    else
    {
        out << this->name << " = ";
    }

    this->expr->transpile(out, tab);
    // this->lhs->transpile(out, tab);
    // out << " = ";
    // this->rhs->transpile(out, tab);

    out << ";" << std::endl;
}

void Expr::transpile(std::ostream &out, int tab) const
{
}

void GradStmt::transpile(std::ostream &out, int tab) const
{
    if (this->grad_type == GradType::GRAD)
    {
        out << std::string("\t", tab) << this->name << "->gradient.print();" << std::endl;
    }
    else
    {
        out << std::string("\t", tab) << "_g." << GradTypeMapCpp[this->grad_type] << "(" << this->name << ");" << std::endl;
    }
}

// int main()
// {
//     return 0;
// }
