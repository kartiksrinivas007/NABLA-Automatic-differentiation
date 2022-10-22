EXEC = parser.out
BISON = bison

.SILENT: test run ../Lexer/lex.yy.c
.PHONY: clean build test run

$(EXEC) : grammar.y ../Lexer/lex.yy.c
	bison -d grammar.y
	g++ grammar.tab.c ../Lexer/lex.yy.c ../ast/ast.cpp -o $(EXEC) 

../Lexer/lex.yy.c: ../Lexer/lexer.l
	cd ../Lexer && make build && cp lex.yy.c ../Parser

build : grammar.y
	$(BISON) -d grammar.y

test: ${EXEC}
	cd Test && ./test.sh

run: $(EXEC)
	./$(EXEC) < Test/tmp.txt

clean: 
	rm -f *.out
	rm -f *.tab.*