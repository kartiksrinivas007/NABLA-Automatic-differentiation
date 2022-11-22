#include "Graph.h"


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

Node* Graph::_add(Node* a , double b){
    //create an add node
    if(a->is_scalar){
        Add *add = new Add(a, _scalar_constant(std::to_string(b), b), count);
        operators.push_back(add);
        count++;
        return add;
    }
}

Node* Graph::_add(double a , double b){
    //create an add node
    Add *add = new Add(_scalar_constant(std::to_string(a), a), _scalar_constant(std::to_string(b), b), count);
    operators.push_back(add);
    count++;
    return add;
}

Node* Graph::_add(double a , Node* b){
    if(b->is_scalar){
        //create an add node
        Add *add = new Add(_scalar_constant(std::to_string(a), a), b, count);
        operators.push_back(add);
        count++;
        return add;
    }
}

Node* Graph::_sub(Node* a , Node* b){
    //create an add node
    Sub *sub = new Sub(a, b, count);
    operators.push_back(sub);
    count++;
    return sub;
}

Node* Graph::_sub(Node* a , double b){
    //create an add node
    if(a->is_scalar){
        Sub *sub = new Sub(a, _scalar_constant(std::to_string(b), b), count);
        operators.push_back(sub);
        count++;
        return sub;
    }
}

Node* Graph::_sub(double a , double b){
    //create an add node
    Sub *sub = new Sub(_scalar_constant(std::to_string(a), a), _scalar_constant(std::to_string(b), b), count);
    operators.push_back(sub);
    count++;
    return sub;
}

Node* Graph::_sub(double a , Node* b){
    if(b->is_scalar){
        //create an add node
        Sub *sub = new Sub(_scalar_constant(std::to_string(a), a), b, count);
        operators.push_back(sub);
        count++;
        return sub;
    }
}

Node* Graph::_mul(Node* a, Node* b){
    Mul *mul = new Mul(a, b, count);
    operators.push_back(mul);
    count++;
    return mul;
}

Node* Graph::_mul(Node* a, double b){
    Mul *mul = new Mul(a, _scalar_constant(std::to_string(b), b), count);
    operators.push_back(mul);
    count++;
    return mul;
}

Node* Graph::_mul(double a, Node* b){
    Mul *mul = new Mul(_scalar_constant(std::to_string(a), a), b, count);
    operators.push_back(mul);
    count++;
    return mul;
}

Node* Graph::_mul(double a, double b){
    Mul *mul = new Mul(_scalar_constant(std::to_string(a), a), _scalar_constant(std::to_string(b), b), count);
    operators.push_back(mul);
    count++;
    return mul;
}

Node* Graph::_div(double a, double b){
    Division *div = new Division(_scalar_constant(std::to_string(a), a), _scalar_constant(std::to_string(b), b), count);
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

Node* Graph::_sin(Node* a){
    Sin* s = new Sin(a, count);
    operators.push_back(s);
    count++;
    return s;
}

Node* Graph::_cos(Node* a){
    Cos* c = new Cos(a, count);
    operators.push_back(c);
    count++;
    return c;
}

Node* Graph::_tan(Node* a){
    Tan* t = new Tan(a, count);
    operators.push_back(t);
    count++;
    return t;
}

Node* Graph::_variable(std::string name, int m, int n, std::vector<std::vector<double>> vals){
    //create a variable node
    Variable *var = new Variable(m, n, vals, count, name);
    variables.push_back(var);
    count++;
    return var;
}
Node* Graph::_variable(std::string name, int m, int n){
    //create a variable node
    std::vector<std::vector<double>> vals(m, std::vector<double>(n, 0.0));
    Variable *var = new Variable(m, n, vals, count, name);
    variables.push_back(var);
    count++;
    return var;
}

Node* Graph::_constant(std::string name, int m, int n, std::vector<std::vector<double>> vals){
    //create a constant node
    Constant *con = new Constant(m, n, vals, count, name);
    constants.push_back(con);
    count++;
    return con;
}

 Node* Graph::_scalar_variable(std::string name, double data){
    Scalar_Variable *scl_v = new Scalar_Variable(name, data, count);
    scalars.push_back(scl_v);
    count++;
    return scl_v;
}

Node* Graph::_scalar_constant(std::string name, double data){
    Scalar_Constant *scl_c = new Scalar_Constant(name, data, count);
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
    generate_graph(f);
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

void Graph::DFS(std::ostream& out, Node* f){
    out << "n" << f->count << "[label=<" << f->name << ">]" << std::endl;
    for(auto& n : f->inputs){
        out << "n" << n->count << " -> "  << "n" << f->count << std::endl;
    }
    f->is_printed = true;
    for(auto& n : f->inputs){
        DFS(out, n);
    }
}

void Graph::generate_graph(Node* f){
    std::ofstream fout;
    fout.open("computational_graph.dot");
    
    fout << "digraph {" << std::endl;
    fout << "rankdir=\"LR\";" << std::endl;
    DFS(fout, f);
    fout << "}" << std::endl;

    fout.close();
}

};