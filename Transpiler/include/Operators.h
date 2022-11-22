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
    Add(Node* a , Node* b, int count);
    Node* forward(const Node* a, const Node* b);
};

class Sub : public Operator{
    public:
    void backward();
    int sub_count;
    Sub(Node* a , Node* b, int count);
    Node* forward(const Node* a, const Node* b);
};

class Multiply : public Operator{
    public:
    void backward();
    int mul_count;
    Multiply(Node* a , Node* b , int count);
    Node* forward(const Node* a, const Node* b);
    // std::pair<Tensor, Tensor> backward(Tensor dout);
};

class Mul : public Operator{
    public:
    void backward();
    int mul_count;
    Mul(Node* a , Node* b , int count);
    Node* forward(const Node* a, const Node* b);
    // std::pair<Tensor, Tensor> backward(Tensor dout);
};

class Transpose : public Operator{
    public:
    void backward();
    int trans_count;
    Transpose(Node* a, int count);
    Node* forward(const Node* a);
};

class Negative : public Operator{
    public:
    void backward();
    int neg_count;
    Negative(Node* a, int count);
    Node* forward(const Node* a);
};

class Exponential : public Operator{
    public:
    void backward();
    int exp_count;
    Exponential(Node* a, int count);
    Node* forward(const Node* a);
};

class Division : public Operator{
    public:
    void backward();
    int div_count;
    Division(Node* a, Node* b, int count);
    Node* forward(const Node* a, const Node* b);
};

class Norm : public Operator{
    public:
    void backward();
    int norm_count;
    Norm(Node* a, int count);
    Node* forward(const Node* a);
};

#endif