#ifndef NODE_H
#define NODE_H

#include<string>
#include"Tensor.h"

class Node{
    public:
    std::string name;
    void print();
    bool is_visited = false;
    std::vector<Node*> inputs;
    Tensor data;
    Tensor gradient;
    Node();
    Node(Tensor& data);
    Node& forward(const Node& a, const Node& b);
    virtual void backward(){};
};

#endif