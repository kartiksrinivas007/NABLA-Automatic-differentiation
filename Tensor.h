#include <vector>
#include <iostream>
#include <string>
#include <initializer_list>

namespace nb{

class Tensor
{
private:
    /* data */

public:
    // Tensor(std::vector<int> dimensions);
    Tensor(std::initializer_list<size_t> dimensions);
    Tensor& operator=(std::initializer_list<std::initializer_list<double>> values);
    ~Tensor();
};

Tensor::Tensor(std::initializer_list<size_t>)
{
}

Tensor::~Tensor()
{
}

Tensor add(Tensor a, Tensor b){
    return Tensor({2, 2});
}

Tensor mul(Tensor a, Tensor b){
    return Tensor({2, 2});
}

Tensor sin(Tensor a){
    return Tensor({2, 2});
}

Tensor cos(Tensor a){
    return Tensor({2, 2});
}

void backward(Tensor a){
}

void grad(Tensor a){
}


template < int n1, int ... args >
class dummy{
public:
    dummy(){
        std::cout << n1 << std::endl;
    }
};

} // namespace nb
