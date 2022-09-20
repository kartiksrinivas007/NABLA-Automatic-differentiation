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
