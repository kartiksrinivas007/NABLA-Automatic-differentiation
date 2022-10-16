TEST-DIR := Test
EXEC = lexer.out


.PHONY : test clean

$(EXEC) :
		lex test_lexer.l
		gcc -o $(EXEC) lex.yy.c 

.SILENT:
test :
		@echo "\e[1mTesting Lexer\e[0m"
		cd $(TEST-DIR);\
		make test


clean:
	rm *.yy.c ||:
	rm *.out ||:
	cd $(TEST-DIR); make clean

