all-tests := $(addsuffix .test, $(basename $(wildcard *.test-in)))
EXEC := test_lexer.out

.PHONY : test all %.test

$(EXEC) :
		lex test_lexer.l
		gcc -o $(EXEC) lex.yy.c -lfl

test : $(EXEC) $(all-tests) 
		@echo "\e[32mSuccess, all tests passed.\e[0m"

%.test : %.test-in %.test-exp $(EXEC)
		
		./$(EXEC) $< | diff -q $(word 2, $?) - >/dev/null || \
    (@echo "\e[31mTest $@ failed\e[0m" && exit 1)	


clean:
	rm *.yy.c ||:
	rm *.out ||: