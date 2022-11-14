#include<iostream>
#include "Graph.h"
using namespace std;

int main ()
{
    Graph _g;
    
    Node& k = _g._scalar_variable(1.25);
    cout<<"K:"<<k.ddata<<endl;
    // to do b = k*a , and c = j@b
    Node &a = _g._variable(2, 1, {{1}, {2}});
    Node &j = _g._variable(1, 2, {{2, 3}});
    Node &c = _g._variable(1, 1);
    cout<<"a"<<endl;
    a.data.print();
    cout<<"j"<<endl;
    j.data.print();
    Node &b = _g._mul(k, a); 
    cout<<"b"<<endl;
    b.data.print();
    c = _g._matmul(j, b);
    cout<<"c"<<endl;
    c.data.print();
for (auto &it: _g.operators){
        cout<<it->name<<endl;
        for(auto &it2: it->inputs){
            cout<<"Operand :"<<endl;
            // it2.print();
            cout << it2 << endl;

        }
    }
    _g.backward(c);
    // cout<<"Grad of a"<<endl;
    // a.gradient.print();
    // cout<<"Grad of j"<<endl;
    // j.gradient.print();
    // cout<<"Grad of b"<<endl;
    // b.gradient.print();
    // cout<<"Grad of c"<<endl;
    // c.gradient.print();
    // cout<<"Grad of k"<<endl;

    // Node& z = _g._matmul(_g._add(x2, y2), _g._add(x1, y1));
    // z.print();
    // cout << "# of vars = " << _g.variables.size() << endl;
    // for (auto i : _g.variables)
    // {
    //     cout<<i->name<<endl;
    // }
    // cout << "End of vars" << endl;
    // for (auto &it: _g.operators){
    //     cout<<it->name<<endl;
    //     for(auto &it2: it->inputs){
    //         cout<<"Operand :"<<endl;
    //         // it2.print();
    //         cout << it2 << endl;

    //     }
    // }
    // for (auto i : _g.variables)
    // {
    //     cout<<i->name<<endl;
    //     cout << "Val:" << endl;
    //     i->data.print();
    //     cout << "Grad:" << endl;
    //     i->gradient.print();
    // }
    return 0;
}
