#include <iostream>
#include <vector>

#include "Tensor.h"

using namespace std;

using namespace nb;

int main() {
	const Tensor b({2, 2});
	const Tensor d({2, 2});
	 Tensor c({2, 2});
	c =  add(b,d);
	backward(c);
	grad(b);
	return 0;
}
