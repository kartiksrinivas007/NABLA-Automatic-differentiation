#ifndef NODE_H
#define NODE_H

#include<string>
#include"Tensor.h"

namespace nb{

class Node{
    public:
    std::string name;
    void print();
    int count;
    bool is_visited = false;
    bool is_printed = false;
    bool is_scalar = false;
    std::vector<Node*> inputs;
    Tensor data;
    double ddata; //this stores the value of a scalar
    Tensor gradient;
    double scalar_gradient = 0; //this stores the gradient with respect to a scalar
    Node();
    Node(Tensor& data);
    Node& forward(const Node& a, const Node& b);
    virtual void backward(){};
};

};

#endif