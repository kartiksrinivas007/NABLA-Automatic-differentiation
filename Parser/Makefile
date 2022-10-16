EXEC = parser.out

.SILENT:
$(EXEC) : grammar.y ../Lexer/lexer.l
	yacc -d grammar.y
	lex ../Lexer/lexer.l
	clang++ y.tab.c lex.yy.c ../ast.cpp -I .. -o $(EXEC) 


test: ${EXEC}
	cd Test && ./test.sh


run: $(EXEC)
	./$(EXEC) < Test/tmp.txt

clean: 
	rm *.out