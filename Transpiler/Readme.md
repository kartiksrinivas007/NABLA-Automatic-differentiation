# Transpilation of Nabla to C++

This directory contains the transpiler from Nabla to C++.

## Uses

compile the transpiler with `make` and run it with `./nabla <input file>`.
The transpiler will generate a `output.cpp` file in the current directory.

compile the generated file with `g++ -std=c++11 output.cpp -o output.out -L lib -lnb` and run it with `./output.out`. 
