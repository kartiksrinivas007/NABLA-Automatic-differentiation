#include "Graph.h"
#include<algorithm>

namespace nb{
Graph::Graph(){
    operators.clear();
    constants.clear();
    variables.clear();
    scalars.clear();
    count = 0;
     //maybe , maybe not ?
}

Node* Graph::_matmul(Node* a, Node* b){
    Multiply *mul = new Multiply(a, b, count); 
    operators.push_back(mul);
    count++;
    return mul; //return the Tensor that is calculated as an output    
}

Node* Graph::_add(Node* a , Node* b){
    //create an add node
    Add *add = new Add(a, b, count);
    operators.push_back(add);
    count++;
    return add;
}

Node* Graph::_sub(Node* a , Node* b){
    //create an add node
    Sub *sub = new Sub(a, b, count);
    operators.push_back(sub);
    count++;
    return sub;
}

Node* Graph::_mul(Node* a, Node* b){
    Mul *mul = new Mul(a, b, count);
    operators.push_back(mul);
    count++;
    return mul;
}

Node* Graph::_mul(Node* a, double b){
    Mul *mul = new Mul(a, _scalar_constant(b), count);
    operators.push_back(mul);
    count++;
    return mul;
}

Node* Graph::_mul(double a, Node* b){
    Mul *mul = new Mul(_scalar_constant(a), b, count);
    operators.push_back(mul);
    count++;
    return mul;
}

Node* Graph::_mul(double a, double b){
    Mul *mul = new Mul(_scalar_constant(a), _scalar_constant(b), count);
    operators.push_back(mul);
    count++;
    return mul;
}

Node* Graph::_div(double a, double b){
    Division *div = new Division(_scalar_constant(a), _scalar_constant(b), count);
    operators.push_back(div);
    count++;
    return div;
}

Node* Graph::_trans(Node* a){
    Transpose* t = new Transpose(a, count);
    operators.push_back(t);
    count++;
    return t;
}

Node* Graph::_neg(Node* a){
    Negative* n = new Negative(a, count);
    operators.push_back(n);
    count++;
    return n;
}

Node* Graph::_exp(Node* a){
    Exponential* e = new Exponential(a, count);
    operators.push_back(e);
    count++;
    return e;
}

Node* Graph::_variable(int m, int n, std::vector<std::vector<double>> vals){
    //create a variable node
    Variable *var = new Variable(m, n, vals, count);
    variables.push_back(var);
    count++;
    return var;
}
Node* Graph::_variable(int m, int n){
    //create a variable node
    std::vector<std::vector<double>> vals(m, std::vector<double>(n, 0.0));
    Variable *var = new Variable(m, n, vals, count);
    variables.push_back(var);
    count++;
    return var;
}

Node* Graph::_constant(int m, int n, std::vector<std::vector<double>> vals){
    //create a constant node
    Constant *con = new Constant(m, n, vals, count);
    constants.push_back(con);
    count++;
    return con;
}

 Node* Graph::_scalar_variable(double data){
    Scalar_Variable *scl_v = new Scalar_Variable(data, count);
    scalars.push_back(scl_v);
    count++;
    return scl_v;
}

Node* Graph::_scalar_constant(double data){
    Scalar_Constant *scl_c = new Scalar_Constant(data, count);
    scalars.push_back(scl_c);
    count++;
    return scl_c;
}

std::vector<Node*> Graph::topological_sort(){
    std::vector<Node*>ordering;
    ordering.resize(0);
    return ordering;
}

void Graph::backward(Node* f){
    if(!f->is_scalar)
    {
        if(f->data.m!=1 || f->data.n!=1){
            std::cout << "Output should be a 1x1 Tensor or a scalar !" << std::endl;
            return;
        }
        // std::cout<<"Output is a 1x1 Tensor"<<std::endl;
        f->gradient = Tensor(1, 1, {{1}});
        // std::cout<<"Address of f "<< f <<std::endl; 
        reverse(operators.begin(), operators.end());
        for(auto& op : operators){ //DURING THE DEBUG PHASE , F AND THE FINAL OP ARE POINTING TO DIFFERENT LOCATIONS!!!!!
            // std::cout<<"Address of op : "<<op<<std::endl;
            if(!op->is_visited){
                op->backward();
                op->is_visited = true;
            }
        }
        reverse(operators.begin(), operators.end());
    }
    else{
        f->scalar_gradient = 1;
        reverse(operators.begin(), operators.end());
        for(auto& op : operators){
            if(!op->is_visited){
                op->backward();
                op->is_visited = true;
            }
        }
        reverse(operators.begin(), operators.end());
    }
}

};