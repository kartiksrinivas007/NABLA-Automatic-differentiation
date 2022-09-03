```bash
cd Lexer-Parser
yacc -d grammar.y && lex lexer.l && gcc lex.yy.c y.tab.c -o exec 
./exec < Test\ Cases/tmp.txt   
```