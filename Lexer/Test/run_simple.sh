#!/bin/bash

DIR=.
file=$1
lex test_lexer.l
gcc -o test_lexer.out lex.yy.c
./test_lexer.out $DIR/$file
