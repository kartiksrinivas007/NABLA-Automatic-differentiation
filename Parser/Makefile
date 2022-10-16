EXEC = parser.out

.SILENT:
$(EXEC) : grammar.y ../Lexer/lexer.l
	yacc -d grammar.y
	lex ../Lexer/lexer.l
	gcc y.tab.c lex.yy.c -o $(EXEC) 


test: ${EXEC}
	cd Test && ./test.sh


run: $(EXEC)
	./$(EXEC) < Test/tmp.txt

clean: 
	rm *.out