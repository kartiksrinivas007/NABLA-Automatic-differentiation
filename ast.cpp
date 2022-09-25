#include "ast.h"
#include <llvm/ADT/APFloat.h>
#include <llvm-c-14/llvm-c/Core.h>

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