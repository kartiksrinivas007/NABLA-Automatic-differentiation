#include "Node.h"

void Node::print(){
    this->data.print();
}

Node::Node(Tensor& t){
    inputs.resize(0);
    data = t;
}

Node::Node(){
    inputs.resize(0);
}