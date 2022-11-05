TEST-DIR := Test
EXEC = lexer.out


.PHONY : test clean build
.SILENT : test ../Parser/grammar.tab.c 

$(EXEC) :
	lex test_lexer.l
	gcc -o $(EXEC) lex.yy.c 

build : ../Parser/grammar.tab.h lexer.l
	lex lexer.l

../Parser/grammar.tab.h : 
	cd ../Parser && make build

test :
		@echo "\e[1mTesting Lexer\e[0m"
		@cd $(TEST-DIR);\
		make test

clean:
	rm *.yy.c ||:
	rm *.out ||:
	@cd $(TEST-DIR); make clean

