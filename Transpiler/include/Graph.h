#ifndef GRAPH_H
#define GRAPH_H

#include "Operators.h" 
#include "Dtypes.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>



namespace nb{

class Graph{
    public:
    int count; //making this static causes issues
    std::vector<Node*> operators, constants, variables, scalars;
    Graph();
    Node* _variable(std::string name, int m, int n, std::vector<std::vector<double>> vals);
    Node* _variable(std::string name, int m, int n);
    Node* _constant(std::string name, int m, int n, std::vector<std::vector<double>> vals);
    Node* _scalar_variable(std::string name, double data=0);
    Node* _scalar_constant(std::string name, double data);

    Node* _add(Node* a, Node* b);
    Node* _add(Node* a, double b);
    Node* _add(double a, Node* b);
    Node* _add(double a, double b);

    Node* _sub(Node* a, Node* b);
    Node* _sub(Node* a, double b);
    Node* _sub(double a, Node* b);
    Node* _sub(double a, double b);
    
    Node* _matmul(Node* a, Node* b);
    Node* _trans(Node* a);
    
    Node* _neg(Node* a);
    Node* _exp(Node* a);
    Node* _sin(Node* a);
    Node* _cos(Node* a);
    Node* _tan(Node* a);

    Node* _mul(Node*a , Node*b);
    Node* _mul(double a, Node*b);
    Node* _mul(Node*a , double b);
    Node* _mul(double a, double b);

    Node* _div(double a, double b);


    std::vector<Node*> topological_sort();
    void backward(Node* f);
    void generate_graph(Node* f);
    void DFS(std::ostream& out, Node* f);
};

};

#endif