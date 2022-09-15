#!/bin/bash

DIR=.
file=$1
lex Lexer/lexer.l
yacc -d Parser/grammar.y
gcc -o exec lex.yy.c y.tab.c
./exec $DIR/$file