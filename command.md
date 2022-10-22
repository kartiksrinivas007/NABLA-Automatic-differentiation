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

```For mac
/opt/homebrew/Cellar/bison/3.8.2/bin/yacc -d Parser/grammar.y && lex Lexer/lexer.l && clang++ -std=c++14 lex.yy.c y.tab.c ast.cpp -o exec
```

```bash
yacc -d Parser/grammar.y && lex Lexer/lexer.l
clang++ -I/usr/include/llvm-14 -I/usr/include/llvm-c-14 -c $(llvm-config --cxxflags) lex.yy.c y.tab.c ast.cpp
clang++ ast.o lex.yy.o y.tab.o $(llvm-config --ldflags --libs) -lpthread -o exec
```

```bison
bison -d Parser/grammar.y && lex Lexer/lexer.l && clang++ lex.yy.c grammar.tab.c  ast/ast.cpp -o exec
```

```Makefile
make build
```

```Makefile
make debug
```

```Command
cd Parser/ && bison -d grammar.y; cd ../Lexer/ &&lex lexer.l ;cd .. && g++ -o exec Parser/grammar.tab.c Lexer/lex.yy.c ast/ast.cpp
```
