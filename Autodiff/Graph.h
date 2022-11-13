#ifndef GRAPH_H
#define GRAPH_H

#include "Operators.h" 
#include "Dtypes.h"
#include <string>

class Graph{
    public:
    int count; //making this static causes issues
    std::vector<Node*> operators, constants, variables, scalars;
    Graph();
    Node& _variable(int m, int n, std::vector<std::vector<double>> vals);
    Node& _variable(int m, int n);
    Node& _constant(int m, int n, std::vector<std::vector<double>> vals);
    double _scalar(double data);
    Node& _add(Node& a, Node& b);
    Node& _matmul(Node& a, Node& b);

    std::vector<Node*> topological_sort();
    void backward(Node& f);
};

#endif