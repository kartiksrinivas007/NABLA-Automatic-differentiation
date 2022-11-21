# Nabla

[![Lexer Build Status](https://github.com/IITH-COMPILERS2/compilers-2-project-team-9-aug22/actions/workflows/lexer.yml/badge.svg)](https://github.com/IITH-COMPILERS2/compilers-2-project-team-9-aug22/actions/workflows/lexer.yml) [![Parser Build Status](https://github.com/IITH-COMPILERS2/compilers-2-project-team-9-aug22/actions/workflows/parser.yml/badge.svg)](https://github.com/IITH-COMPILERS2/compilers-2-project-team-9-aug22/actions/workflows/parser.yml) [![Semantic Build Status](https://github.com/IITH-COMPILERS2/compilers-2-project-team-9-aug22/actions/workflows/semantic.yml/badge.svg)](https://github.com/IITH-COMPILERS2/compilers-2-project-team-9-aug22/actions/workflows/semantic.yml)

![alt text](Whitepaper/images/nabla.png)
<br>

Nabla is a Domain specific langauge built for the purpose of Tensor Operations and Automatic differentiation
<br>
It uses dynamically built computational graphs for back-propagation.
Take a look at the code below

```python
declare{
    cns int a = 6;
    var int c;
}
operations{
    c = sin(a+cos(a));  
    d = exp(c);
}
gradient{
    backward(d);
    grad(d);
    grad(c);
}
```

The code will be converted into a computational graph(internally) of the form:-
After this we will be able to use the chain rule to calculate the gradients of the Final variable in terms of the beginning variables

![alt text](Whitepaper/images/comp_graph2.png)

## Documentation

To generate the documentation for the project, run the following command in the root directory of the project

```bash
doxygen
```

The documentation for the project can be found [here](https://ganesh-rb.github.io/Nabla-documentation/)

## The Class Hierarchy

---


![alt text](Whitepaper/images/class_hierarchy.png)

To see the class Hierarchy please switch to the branch `AST` and see files `ast.h` and `ast.cpp`

## Testing

---

Go inside the lexer folder and run these commands to test lexer
```console
make 
make test
```

Go inside the Parser folder and run these commands to test Parser
```console
make 
make test
```

Go inside the Semantic folder and run these commands to test semantic analysis
```console
make 
make test
```

