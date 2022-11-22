#ifndef DTYPE_H
#define DTYPE_H

#include "Node.h"
#include <string>
#include <vector>

namespace nb{
class Variable: public Node{
    public:
    Variable(int m , int n, std::vector<std::vector<double>> vals, int count ){
        data = Tensor(m, n, vals);
        gradient = Tensor(m, n);
        this->count = count;
        name = "Var";
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
    int scl_count;
    Scalar(double d_data, int count){
        // this->ddata = d_data;
        // scl_count = count;
        // name = "Scalar:" + std::to_string(scl_count);
    }
};

class Scalar_Variable : public Scalar{
    public:
    int scl_var_count;
    Scalar_Variable(double d_data, int count): Scalar(d_data, count){
        this->ddata = d_data;
        scl_var_count = count;
        is_scalar = true;
        name = "Scalar_Var:" + std::to_string(scl_var_count);
    }
};

class Scalar_Constant : public Scalar{
    public:
    int scl_cns_count;
    Scalar_Constant(double d_data, int count): Scalar(d_data, count){
        this->ddata  = d_data; //this innitializes a scalar constant
        scl_cns_count = count;
        is_scalar = true;
        name = "Scalar_Const:" + std::to_string(scl_cns_count);
    }
};

};
#endif