```bash
yacc -d Parser/grammar.y && lex Lexer/lexer.l && gcc lex.yy.c y.tab.c -o exec 
./exec < Parser/Test/tmp.txt   
```