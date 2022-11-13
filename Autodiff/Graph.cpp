#include "Graph.h"
#include<algorithm>
Graph::Graph(){
    operators.clear();
    constants.clear();
    variables.clear();
    scalars.clear();
    count = 0;
     //maybe , maybe not ?
}

Node& Graph::_matmul(Node& a, Node& b){
    Multiply *mul = new Multiply(a, b, count); 
    operators.push_back(mul);
    count++;
    return *mul; //return the Tensor that is calculated as an output    
}

Node& Graph::_add(Node& a , Node& b){
    //create an add node
    Add *add = new Add(a, b, count);
    operators.push_back(add);
    count++;
    return *add;
}

Node& Graph::_variable(int m, int n, std::vector<std::vector<double>> vals){
    //create a variable node
    Variable *var = new Variable(m, n, vals, count);
    variables.push_back(var);
    count++;
    return *var;
}
Node& Graph::_variable(int m, int n){
    //create a variable node
    std::vector<std::vector<double>> vals(m, std::vector<double>(n, 0.0));
    Variable *var = new Variable(m, n, vals, count);
    variables.push_back(var);
    count++;
    return *var;
}

Node& Graph::_constant(int m, int n, std::vector<std::vector<double>> vals){
    //create a constant node
    Constant *con = new Constant(m, n, vals, count);
    constants.push_back(con);
    count++;
    return *con;
}

double Graph::_scalar(double data){
    //create a scalar node
    Scalar *scl = new Scalar(data, count);
    scalars.push_back(scl);
    count++;
    return scl->ddata;
}


std::vector<Node*> Graph::topological_sort(){
    std::vector<Node*>ordering;
    ordering.resize(0);

    return ordering;
}

void Graph::backward(Node& f){
    if(f.data.m!=1 || f.data.n!=1){
        std::cout << "Output should be 1x1!!!!!!!!!!!!!!!" << std::endl;
    }
    f.gradient = Tensor(1, 1, {{1}});
    reverse(operators.begin(), operators.end());
    for(auto& op : operators){
        if(!op->is_visited){
            op->backward();
            op->is_visited = true;
        }
    }
    reverse(operators.begin(), operators.end());
}