```bash
yacc -d Parser/grammar.y && lex Lexer/lexer.l && gcc lex.yy.c y.tab.c -o exec
./exec < Parser/Test/tmp.txt
```

```bash
yacc -d Parser/grammar.y && lex Lexer/lexer.l && g++ lex.yy.c y.tab.c ast.cpp -o exec
```

```bash
yacc -d Parser/grammar.y && lex Lexer/lexer.l && clang++ lex.yy.c y.tab.c ast.cpp -o exec
```


may be working

```bash
yacc -d Parser/grammar.y && lex Lexer/lexer.l
clang++ -I/usr/include/llvm-14 -I/usr/include/llvm-c-14 -c $(llvm-config --cxxflags) lex.yy.c y.tab.c ast.cpp 
clang++ ast.o lex.yy.o y.tab.o $(llvm-config --ldflags --libs) -lpthread -o exec  
```
