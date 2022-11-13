#include<iostream>
#include "Graph.h"
using namespace std;

int main ()
{
    Graph _g;
    
    // Node& x = (_g._variable(2, 2, {{1, 2}, {3, 4}}));
    // cout << "X:" << &x  << endl;
    // x.print();
    // Node& y = (_g._variable(2, 2, {{5, 0}, {1, 2}}));
    // cout << "Y:" << &y  << endl;
    // y.print();
    // Node& z = (_g._variable(2, 2));
    // cout<<"x + x*y"<<endl;

    // z = _g._add(x, _g._matmul(x, y));
    
    // z.print();

    Node& x1 = (_g._variable(2, 1, {{1}, {2}}));
    Node& y1 = (_g._variable(2, 1, {{3}, {5}}));
    Node& x2 = (_g._variable(1, 2, {{2, 4}}));
    Node& y2 = (_g._variable(1, 2, {{5, 1}}));
    Node& z = _g._matmul(_g._add(x2, y2), _g._add(x1, y1));
    z.print();
    cout << "# of vars = " << _g.variables.size() << endl;
    for (auto i : _g.variables)
    {
        cout<<i->name<<endl;
    }
    cout << "End of vars" << endl;
    for (auto &it: _g.operators){
        cout<<it->name<<endl;
        for(auto &it2: it->inputs){
            cout<<"Operand :"<<endl;
            // it2.print();
            cout << it2 << endl;

        }
    }
    _g.backward(z);
    for (auto i : _g.variables)
    {
        cout<<i->name<<endl;
        cout << "Val:" << endl;
        i->data.print();
        cout << "Grad:" << endl;
        i->gradient.print();
    }
}
