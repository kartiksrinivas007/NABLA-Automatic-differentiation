#include "Operators.h"


Add::Add(Node& a , Node& b, int count){
        // ::count++;
        add_count = count;
        inputs.push_back(&a);
        inputs.push_back(&b);
        this->name = "Add:" + std::to_string(add_count);
        this->forward(a, b);
}

Node& Add::forward(const Node& a, const Node& b)
{
    Tensor* c = new Tensor(a.data.m, a.data.n);
    for(int i=0;i<a.data.m;i++){
        for(int j=0;j<a.data.n;j++){
            c->data[i][j] = a.data.data[i][j] + b.data.data[i][j];
        }
    }
    // Node* n_c = new Node(*c);
    this->data = *c;
    return *this;
}

void Add::backward()
{
    for(auto& x : inputs){
        x->gradient = this->gradient;
    }
}


Multiply::Multiply(Node& a , Node& b , int count){
        mul_count = count;
        inputs.push_back(&a);
        inputs.push_back(&b);
        this->name = "Mul:" + std::to_string(mul_count);
        this->forward(a, b);
}

Node& Multiply :: forward(const Node& a, const Node& b)
{
    Tensor* c = new Tensor(a.data.m, b.data.n);
    for (int i = 0; i< a.data.m ; i++){
        for (int j = 0; j < b.data.n; j++){
            (c->data)[i][j] = 0;
            for (int k = 0; k < a.data.n; k++){
                (c->data)[i][j] += (a.data.data)[i][k] * (b.data.data)[k][j];
            }
        }
    }
    this->data = *c;
    return *this;
}

void Multiply :: backward(){
    this->inputs[0]->gradient = matmul(this->gradient, inputs[1]->data.transpose());
    this->inputs[1]->gradient = matmul(inputs[0]->data.transpose(), this->gradient);
}
