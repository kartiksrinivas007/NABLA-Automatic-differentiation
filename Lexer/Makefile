TEST-DIR := Test
EXEC = lexer.out
LEX = flex
CC = gcc

.PHONY : test clean build
.SILENT : test

$(EXEC) : lexer.l
	$(LEX) lexer.l
	$(CC) -o $(EXEC) lex.yy.c

test : $(EXEC)
	cp $(EXEC) $(TEST-DIR)
	@echo "\e[1mTesting Lexer\e[0m"
	@echo "----------------"
	${MAKE} --no-print-directory -C $(TEST-DIR) test

clean:
	rm -f *.yy.c
	rm -f *.out
	rm -f $(TEST-DIR)/*.out


