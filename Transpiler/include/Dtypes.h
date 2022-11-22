#ifndef DTYPE_H
#define DTYPE_H

#include "Node.h"
#include <string>
#include <vector>

namespace nb{
class Variable: public Node{
    public:
    Variable(int m , int n, std::vector<std::vector<double>> vals, int count, std::string name){
        data = Tensor(m, n, vals);
        gradient = Tensor(m, n);
        this->count = count;
        this->name = name;
    }
};

class Constant: public Node{
    public:
    Constant(int m, int n, std::vector<std::vector<double>> vals, int count, std::string name){
        data = Tensor(m, n, vals);
        this->count = count;
        this->name = name;
    }

};

class Scalar: public Node{
    public:
    Scalar(double d_data, int count){
        // this->ddata = d_data;
        // scl_count = count;
        // name = "Scalar:" + std::to_string(scl_count);
    }
};

class Scalar_Variable : public Scalar{
    public:
    Scalar_Variable(std::string name, double d_data, int count): Scalar(d_data, count){
        this->ddata = d_data;
        this->count = count;
        is_scalar = true;
        this->name = name;
    }
};

class Scalar_Constant : public Scalar{
    public:
    Scalar_Constant(std::string name, double d_data, int count): Scalar(d_data, count){
        this->ddata  = d_data; //this innitializes a scalar constant
        this->count = count;
        is_scalar = true;
        this->name = name;
    }
};

};
#endif