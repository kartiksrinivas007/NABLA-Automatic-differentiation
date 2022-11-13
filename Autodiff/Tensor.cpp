#include"Tensor.h"

Tensor::Tensor (int m, int n){
    this->m = m;
    this->n = n;
    data.resize(m);
    for(int i=0;i<m;i++){
        data[i].resize(n);
    }
    for (int i=0;i<m;i++){
        for (int j=0;j<n;j++){
            data[i][j] = 0;
        }
    }
}

Tensor::Tensor (int m, int n, std::vector<std::vector<double>> vals){
    this->m = m;
    this->n = n;
    data.resize(m);
    for(int i=0;i<m;i++){
        data[i].resize(n);
    }
    for (int i=0;i<m;i++){
        for (int j=0;j<n;j++){
            data[i][j] = vals[i][j];
        }
    }
}

Tensor::Tensor(){
        m = 0;
        n = 0;
}

void Tensor::print(){
        for (int i=0;i<m;i++){
            for (int j=0;j<n;j++){
                std::cout<<data[i][j]<<" ";
            }
            std::cout<<std::endl;
        }

}

std::pair<int, int> Tensor:: shape(){
        std::cout<<"Shape: ("<<m<<","<<n<<")"<<std::endl;
        return std::make_pair(m, n);
}


Tensor Tensor::transpose(){
    Tensor tr = Tensor(n, m);
    for (int i=0;i<m;i++){
        for (int j=0;j<n;j++){
            tr.data[j][i] = data[i][j];
        }
    }
    return tr;
}

Tensor matmul(Tensor a, Tensor b){
    Tensor* c = new Tensor(a.m, b.n);
    for (int i = 0; i< a.m ; i++){
        for (int j = 0; j < b.n; j++){
            (c->data)[i][j] = 0;
            for (int k = 0; k < a.n; k++){
                (c->data)[i][j] += (a.data)[i][k] * (b.data)[k][j];
            }
        }
    }
    return *c;
}

Tensor add(Tensor a, Tensor b){
    Tensor* c = new Tensor(a.m, a.n);
    for(int i=0;i<a.m;i++){
        for(int j=0;j<a.n;j++){
            c->data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }
    return *c;
}