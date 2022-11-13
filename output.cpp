#include <iostream>
#include "Graph.h"

using namespace std;

int main() {
	Graph _g;
	Node& c = _g._constant(1, 2, {{1, 2, 3}});
	Node& b = _g._variable(2, 3, {{1, 1, 1}, {2, 4, 5}, {7, 8, 9}});
	c =  _g._add( _g._add(a,b),d);
	_g.backward(c);
	_g.grad(b);
	return 0;
}
