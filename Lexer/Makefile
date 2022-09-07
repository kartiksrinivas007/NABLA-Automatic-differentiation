TEST-DIR := Test

.PHONY : test clean

.SILENT:
test :
		@echo "\e[1mTesting Lexer\e[0m"
		cd $(TEST-DIR);\
		make test


clean:
	rm *.yy.c ||:
	rm *.out ||:
	cd $(TEST-DIR); make clean

