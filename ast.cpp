#include "ast.h"

extern "C"
{
    void Parent::func()
    {
        std::cout << "Parent::func()" << std::endl;
    }

    void Child::func()
    {
        std::cout << "Child::func()" << std::endl;
    }
}