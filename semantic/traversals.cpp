#include "traversal.h"

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
                    std::cout << "Fatal: Type mismatch, tensor value being assigned to scalar: " << decl->InitDeclaratorList->declarator->name << std::endl;
                }
            }
            //Checks if the scalar variable declared is of scalar type and not tensor
            if(decl->InitDeclaratorList->declarator->Dimensions.size() != 0)
            {
                std::cout << "Fatal: Type mismatch, scalar variable is not arrayable(tensor) type: " << decl->InitDeclaratorList->declarator->name << std::endl;
            }
        }
        //Check if float value is being assigned to non float type variable
        if (decl->DataType == TypeSpecifier::INT || decl->DataType == TypeSpecifier::BOOL)
        {
            if (decl->InitDeclaratorList->initializer != NULL)
            {
                if (decl->InitDeclaratorList->initializer->val.cvalue != NULL)
                {
                    if (decl->InitDeclaratorList->initializer->isScalar == true && decl->InitDeclaratorList->initializer->val.cvalue->isInt == false)
                    {
                        std::cout << "Fatal: Type mismatch, float value being assigned to non float variable: " << decl->InitDeclaratorList->declarator->name << std::endl;
                    }
                }
            }
        }

        //Check for tensors
        if(decl->DataType == TypeSpecifier::TENSOR)
        {
            if(decl->InitDeclaratorList->declarator->Dimensions.size() == 0)
            {
                std::cout << "Fatal: Tensor variable shape not declared: " << decl->InitDeclaratorList->declarator->name << std::endl;
            }
            else if(decl->InitDeclaratorList->initializer != NULL)
            {
                std::vector<int> tensor_shape = decl->InitDeclaratorList->declarator->Dimensions;
            }
            else{}
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

void traverse_gradient2(Start *root)
{
    std::vector<GradStmt *> *GradStmtList = root->GradStmtList;
    bool anyError = false;
    std::cout << "Gradient Semantic Analysis" << std::endl;
    // checking if gradient is being done for vars only
    for (auto gradStmt : *GradStmtList)
    {
        std::string gradName = gradStmt->name;
        // find(name in symbol table and get its type as cns or var)
        //  if not found then throw error
        SymTabItem *symTabItem = search(root->symbolTable, gradName);
        if (symTabItem == NULL)
        {
            std::cout << "Fatal: Variable " << gradName << " not found" << std::endl;
            // anyError = true;
        }
        else
        {
            if (symTabItem->type == "cns")
            {
                std::cout << "Fatal: Cannot take gradient of a constant: " << gradName << std::endl;
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
                    std::cout << "Fatal: Cannot take gradient of: '" << gradStmt_name << "' without taking backward first" << std::endl;
                    anyError = true;
                }
                else
                {
                    gradMap.insert(std::pair<std::string, GradType>(gradStmt_name, gradStmt_type));
                }
            }
        }
    }
    std::cout << "Gradient Semantic Analysis Over" << std::endl;
}
