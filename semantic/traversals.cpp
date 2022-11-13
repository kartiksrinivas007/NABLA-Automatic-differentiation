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

    for(auto decl : *DeclList) {
        if(decl->DataType == TypeSpecifier::INT)
        {
            if(decl->InitDeclaratorList->initializer->val.cvalue != NULL)
            {
                if(decl->InitDeclaratorList->initializer->val.cvalue->isInt == false)
                {
                    std::cout << "Fatal: Type mismatch int being assigned a float value" << std::endl;
                }
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
    if(anyError == false)
    {
        std::map<std::string, GradType> gradMap;
        for(auto gradStmt: *GradStmtList)
        {
            std::string gradStmt_name = gradStmt->name;
            GradType gradStmt_type = gradStmt->grad_type;
            auto it = gradMap.find(gradStmt_name);
            if(it == gradMap.end())
            {
                if(gradStmt_type == GradType::GRAD)
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