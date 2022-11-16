#include "traversal.h"

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

void traverse_declarations(std::vector<Decl *> *DeclList)
{
    for (auto decl : *DeclList)
    {
        switch (decl->DataType)
        {
        case TypeSpecifier::INT:
            break;

        case TypeSpecifier::BOOL:
            break;

        case TypeSpecifier::FLOAT:
            break;

        case TypeSpecifier::TENSOR:
            break;

        default:
            std::cout << "Fatal: Type not found" << std::endl;
            break;
        }
    }

    for (auto decl : *DeclList)
    {
        if (decl->DataType == TypeSpecifier::INT)
        {
            if (decl->InitDeclaratorList->initializer->val.cvalue != NULL)
            {
                if (decl->InitDeclaratorList->initializer->val.cvalue->isInt == false)
                {
                    std::cout << "Fatal: Type mismatch int being assigned a float value" << std::endl;
                }
            }
        }
    }
}

int isUniformSize(Initializer *init, std::vector<int> &vec)
{
    if (init->isScalar == true)
    {
        return 1;
    }
    int size = init->val.InitializerList->size();
    std::vector<int> tmp;
    for (auto i : *init->val.InitializerList)
    {
        tmp.push_back(isUniformSize(i, vec));
    }
    if (std::all_of(tmp.begin(), tmp.end(), [&](int i)
                    { return i == tmp[0]; }))
    {
        // vec.push_back(size);
        return size;
    }
    else
    {
        // std::cout << "Fatal: Tensor dimensions not uniform" << std::endl;
        return -1;
    }
}

void ShapeTensor(Initializer *init, std::vector<int> &vec)
{
    if (init->isScalar == true)
    {
        return;
    }
    int size = init->val.InitializerList->size();
    vec.push_back(size);
    ShapeTensor(init->val.InitializerList->at(0), vec);
    return;
}

// Semantic Analysis for Declarations part of the program
void traverse_declarations2(Start *root)
{
    std::vector<Decl *> *DeclList = root->DeclList;

    // Check for invalid data type declarations
    for (auto decl : *DeclList)
    {
        switch (decl->DataType)
        {
        case TypeSpecifier::INT:
            break;

        case TypeSpecifier::BOOL:
            break;

        case TypeSpecifier::FLOAT:
            break;

        case TypeSpecifier::TENSOR:
            break;

        default:
            std::cout << "Fatal: Type not found" << std::endl;
            break;
        }
    }

    // Check if correct values are being assigned to variables or not
    for (auto decl : *DeclList)
    {
        // std::cout << decl->InitDeclaratorList->declarator->name << " " << decl->InitDeclaratorList->declarator->Dimensions.size() << std::endl;
        if (decl->DataType != TypeSpecifier::TENSOR)
        {
            // Check if tensor value is being assigned to a scalar variable or not
            if (decl->InitDeclaratorList->initializer != NULL)
            {
                if (decl->InitDeclaratorList->initializer->isScalar == false)
                {
                    // Find the row and col from symbol table
                    SymTabItem *symTabItem = search(root->symbolTable, decl->InitDeclaratorList->declarator->name);
                    std::cout << "Fatal: Type mismatch at " << symTabItem->rowNum << ":" << symTabItem->colNum << "\n\tTensor value being assigned to scalar: " << decl->InitDeclaratorList->declarator->name << std::endl;
                }
            }
            //  Checks if the scalar variable declared is of scalar type and not tensor
            if (decl->InitDeclaratorList->declarator->Dimensions.size() != 0)
            {
                SymTabItem *symTabItem = search(root->symbolTable, decl->InitDeclaratorList->declarator->name);
                std::cout << "Fatal: Type mismatch at " << symTabItem->rowNum << ":" << symTabItem->colNum << "\n\tScalar variable is not arrayable(tensor) type: " << decl->InitDeclaratorList->declarator->name << std::endl;
            }
        }
        //  Check if float value is being assigned to non float type variable
        if (decl->DataType == TypeSpecifier::INT || decl->DataType == TypeSpecifier::BOOL)
        {
            if (decl->InitDeclaratorList->initializer != NULL)
            {
                if (decl->InitDeclaratorList->initializer->val.cvalue != NULL)
                {
                    if (decl->InitDeclaratorList->initializer->isScalar == true && decl->InitDeclaratorList->initializer->val.cvalue->isInt == false)
                    {
                        SymTabItem *symTabItem = search(root->symbolTable, decl->InitDeclaratorList->declarator->name);
                        std::cout << "Fatal: Type mismatch at " << symTabItem->rowNum << ":" << symTabItem->colNum << "\n\tFloat value being assigned to non float variable: " << decl->InitDeclaratorList->declarator->name << std::endl;
                    }
                }
            }
        }

        //  Check for tensors
        if (decl->DataType == TypeSpecifier::TENSOR)
        {
            // Check if tensor shape is declared
            if (decl->InitDeclaratorList->declarator->Dimensions.size() == 0)
            {
                SymTabItem *symTabItem = search(root->symbolTable, decl->InitDeclaratorList->declarator->name);
                std::cout << "Fatal: Type mismatch at " << symTabItem->rowNum << ":" << symTabItem->colNum << "\n\tTensor variable shape not declared: " << decl->InitDeclaratorList->declarator->name << std::endl;
            }
            else if (decl->InitDeclaratorList->initializer != NULL)
            {
                // Check if Tensor shape is uniform
                std::vector<int> tensor_declare_shape = decl->InitDeclaratorList->declarator->Dimensions;
                std::vector<int> tensor_init_shape;
                int size = isUniformSize(decl->InitDeclaratorList->initializer, tensor_init_shape);
                if (size == -1)
                {
                    SymTabItem *symTabItem = search(root->symbolTable, decl->InitDeclaratorList->declarator->name);
                    std::cout << "Fatal: Type mismatch at " << symTabItem->rowNum << ":" << symTabItem->colNum << "\n\tTensor dimensions not uniform: " << decl->InitDeclaratorList->declarator->name << std::endl;
                }
                std::cout << decl->InitDeclaratorList->declarator->name << " " << size << std::endl;
                if (size != -1)
                {
                    ShapeTensor(decl->InitDeclaratorList->initializer, tensor_init_shape);

                    for (int i = 0; i < tensor_init_shape.size(); i++)
                    {
                        std::cout << tensor_init_shape[i] << " ";
                    }
                    std::cout << "\n";
                }
                // TODO: if it is nx1 output is still n
                //  TODO: check tensor_init_shape with tensor_declare_shape
            }
            else
            {
            }
        }
    }
}

void traverse_operations(Start *root)
{
    std::vector<AssgnStmt *> *AssgnStmtList = root->AssgnStmtList;
    // should be equal to number of assignment statements
    std::cout << "Traversing operations: " << AssgnStmtList->size() << std::endl;
    for (auto assgn_stmt : *AssgnStmtList)
    {
        std::string var_name = assgn_stmt->name;
        SymTabItem *symTabItem = search(root->symbolTable, var_name);
        if (symTabItem == NULL)
        {
            std::cout << "Fatal: Variable " << var_name << " not found" << std::endl;
            exit(0);
        }
        assgn_stmt->expr->initialize_expression_node_info(root->symbolTable);

        std::cout << "Dimensions of expr " << assgn_stmt->expr->dimensions << std::endl;
        if (symTabItem->dataType == "int")
        {
            if (assgn_stmt->expr->DataType != TypeSpecifier::INT)
            {
                std::cout << "Fatal: RHS and LHS not of same type\n";
                std::cout << "LHS of type: " << symTabItem->dataType << "\n";
                exit(0);
                // TODO: handle shorthand
                // TODO: add errors for bool and char
            }
        }
        else if (symTabItem->dataType == "float")
        {
            if (assgn_stmt->expr->DataType == TypeSpecifier::TENSOR)
            {
                std::cout << "Fatal: RHS and LHS not of same type\n";
                std::cout << "LHS of type: " << symTabItem->dataType << "\n";
                exit(0);
            }
        }
        else if (symTabItem->dataType == "Tensor")
        {
            if (assgn_stmt->expr->DataType != TypeSpecifier::TENSOR)
            {
                std::cout << "Fatal: RHS and LHS not of same type\n";
                std::cout << "LHS of type: " << symTabItem->dataType << "\n";
                exit(0);
            }

            if (assgn_stmt->expr->dimensions != symTabItem->Dims)
            {
                std::cout << "Fatal: RHS and LHS not of same dimensions\n";
                exit(0);
            }
        }
    }
}

void traverse_gradient(std::vector<GradStmt *> *GradStmtList)
{
    bool anyError = false;
    // checking if gradient is being done for vars only
    // for(auto gradStmt: *GradStmtList)
    // {
    //     std::string gradName = gradStmt->name;
    //     //find(name in symbol table and get its type as cns or var)
    //     // if not found then throw error
    //     GradSpecifier gradType = GradSpecifier::CNS;
    //     if(gradType == GradSpecifier::CNS)
    //     {
    //         std::cout << "Fatal: Cannot take gradient of a constant" << std::endl;
    //         anyError = true;
    //     }
    // }

    // checking if backward and then gradient is being done
    if (anyError == false)
    {
        std::map<std::string, GradType> gradMap;
        for (auto gradStmt : *GradStmtList)
        {
            std::string gradStmt_name = gradStmt->name;
            GradType gradStmt_type = gradStmt->grad_type;
            auto it = gradMap.find(gradStmt_name);
            if (it == gradMap.end())
            {
                if (gradStmt_type == GradType::GRAD)
                {
                    std::cout << "Fatal: Cannot take gradient of a variable without taking backward first" << std::endl;
                    anyError = true;
                }
                else
                {
                    gradMap.insert(std::pair<std::string, GradType>(gradStmt_name, gradStmt_type));
                }
            }
        }
    }
}

// Semantic Analysis for Gradient part of the program
void traverse_gradient2(Start *root)
{
    std::vector<GradStmt *> *GradStmtList = root->GradStmtList;
    bool anyError = false;
    // std::cout << "Gradient Semantic Analysis" << std::endl;
    // checking if gradient is being done for vars only
    for (auto gradStmt : *GradStmtList)
    {
        std::string gradName = gradStmt->name;
        // Checking if variable exists
        SymTabItem *symTabItem = search(root->symbolTable, gradName);
        if (symTabItem == NULL)
        {
            std::cout << "Fatal: Variable " << gradName << " not found" << std::endl;
            // anyError = true;
        }
        else
        {
            // Checking if variable is a constant as its gradient cannot be taken
            if (symTabItem->type == "cns")
            {
                SymTabItem *symTabItem = search(root->symbolTable, gradStmt->name);
                std::cout << "Fatal: Type mismatch at " << symTabItem->rowNum << ":" << symTabItem->colNum << "\n\tCannot take gradient of a constant: " << gradName << std::endl;
                // anyError = true;
            }
        }
    }

    // checking if backward and then gradient is being done
    // std::cout << "Gradient Semantic Analysis" << std::endl;
    if (anyError == false)
    {
        std::map<std::string, GradType> gradMap;
        for (auto gradStmt : *GradStmtList)
        {
            std::string gradStmt_name = gradStmt->name;
            GradType gradStmt_type = gradStmt->grad_type;
            auto it = gradMap.find(gradStmt_name);
            // std::cout <<"Here\n";
            if (it == gradMap.end())
            {
                if (gradStmt_type == GradType::GRAD)
                {
                    SymTabItem *symTabItem = search(root->symbolTable, gradStmt->name);
                    std::cout << "Fatal: Type mismatch at " << symTabItem->rowNum << ":" << symTabItem->colNum << "\n\tCannot take gradient of: '" << gradStmt_name << "' without taking backward first" << std::endl;
                    anyError = true;
                }
                else
                {
                    gradMap.insert(std::pair<std::string, GradType>(gradStmt_name, gradStmt_type));
                }
            }
        }
    }
    // std::cout << "Gradient Semantic Analysis Over" << std::endl;
}
