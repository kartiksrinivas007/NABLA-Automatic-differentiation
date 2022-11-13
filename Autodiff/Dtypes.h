#ifndef DTYPE_H
#define DTYPE_H

#include "Node.h"
#include <string>
#include <vector>

class Variable: public Node{
    public:
    int var_count;
    Variable(int m , int n, std::vector<std::vector<double>> vals, int count ){
        data = Tensor(m, n, vals);
        gradient = Tensor(m, n);
        var_count = count;
        name = "Var:" + std::to_string(var_count);
    }
};

class Constant: public Node{
    public:
    int cns_count;
    Constant(int m, int n, std::vector<std::vector<double>> vals, int count){
        data = Tensor(m, n, vals);
        cns_count = count;
        name = "Const:" + std::to_string(cns_count);
    }

};

class Scalar: public Node{
    public:
    double ddata;
    int scl_count;
    Scalar(double d_data, int count){
        this->ddata = d_data;
        scl_count = count;
        name = "Scalar:" + std::to_string(scl_count);
    }
};

#endif