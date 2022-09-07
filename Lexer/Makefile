TEST-DIR := Test

.PHONY : test all 

p1:
	lex lexer.l
	gcc -o lexer.out lex.yy.c

.SILENT:
test :
		@echo "\e[1mTesting Lexer\e[0m"
		cd $(TEST-DIR);\
		make test


clean:
	rm *.yy.c ||:
	rm *.out ||:
	cd $(TEST-DIR); make clean

