#ifndef OP_H
#define OP_H
#include "Node.h"
#include <vector>


class Operator: public Node{
    public:
    virtual void backward(){};
    Operator(){
        inputs.resize(0);
    }
};

class Add : public Operator{
    public:
    void backward();
    int add_count;
    Add(Node& a , Node& b, int count);
    Node& forward(const Node& a, const Node& b);
    // std::pair<Tensor, Tensor> backward(Tensor dout);
};

class Multiply : public Operator{
    public:
    void backward();
    int mul_count;
    Multiply(Node& a , Node& b , int count);
    Node& forward(const Node& a, const Node& b);
    // std::pair<Tensor, Tensor> backward(Tensor dout);
};

class Mul : public Operator{
    public:
    void backward();
    int mul_count;
    Mul(Node& a , Node& b , int count);
    Node& forward(const Node& a, const Node& b);
    // std::pair<Tensor, Tensor> backward(Tensor dout);
};

#endif