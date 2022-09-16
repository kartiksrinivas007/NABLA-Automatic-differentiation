#!/bin/bash

DIR=.
file=$1
yacc -d Parser/grammar.y
lex Lexer/lexer.l
gcc -o exec lex.yy.c y.tab.c
inpdir=$(dirname "$1")
./exec $DIR/$file > $DIR/"parser-op.txt"