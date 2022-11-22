#ifndef OP_H
#define OP_H
#include "Node.h"
#include <vector>

namespace nb{

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
    Add(Node* a , Node* b, int count);
    Node* forward(const Node* a, const Node* b);
};

class Sub : public Operator{
    public:
    void backward();
    Sub(Node* a , Node* b, int count);
    Node* forward(const Node* a, const Node* b);
};

class Multiply : public Operator{
    public:
    void backward();
    Multiply(Node* a , Node* b , int count);
    Node* forward(const Node* a, const Node* b);
    // std::pair<Tensor, Tensor> backward(Tensor dout);
};

class Mul : public Operator{
    public:
    void backward();
    Mul(Node* a , Node* b , int count);
    Node* forward(const Node* a, const Node* b);
    // std::pair<Tensor, Tensor> backward(Tensor dout);
};

class Transpose : public Operator{
    public:
    void backward();
    Transpose(Node* a, int count);
    Node* forward(const Node* a);
};

class Negative : public Operator{
    public:
    void backward();
    Negative(Node* a, int count);
    Node* forward(const Node* a);
};

class Exponential : public Operator{
    public:
    void backward();
    Exponential(Node* a, int count);
    Node* forward(const Node* a);
};

class Division : public Operator{
    public:
    void backward();
    Division(Node* a, Node* b, int count);
    Node* forward(const Node* a, const Node* b);
};

class Sin : public Operator{
    public:
    void backward();
    Sin(Node* a, int count);
    Node* forward(const Node* a);
};

class Cos : public Operator{
    public:
    void backward();
    Cos(Node* a, int count);
    Node* forward(const Node* a);
};

class Tan : public Operator{
    public:
    void backward();
    Tan(Node* a, int count);
    Node* forward(const Node* a);
};

};

#endif