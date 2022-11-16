all-tests := $(addsuffix .test, $(basename $(wildcard *.test-in)))
prog-tests := $(addsuffix .nb.test, $(basename $(wildcard *.nb)))
EXEC := lexer.out

.PHONY : test all %.test

test : $(EXEC) $(all-tests) $(prog-tests)
		(grep 1 .command.status > .command.null &&\
		echo "\e[1;31m\nSome test case Failed.\e[0m" && rm .command.*)||\
		(echo "\e[1;32m\nSuccess, all tests passed.\e[0m" && rm .command.* && exit 0) ||\
		exit 1

.SILENT:
%.test : %.test-in %.test-exp $(EXEC)
		(./$(EXEC) $< | diff $(word 2, $?) - >> .command.result &&\
		(echo "\e[32m\t [ PASSED ] \e[0m  $@")) && echo 0 >> .command.status ||\
    (echo "\e[31m\t [ FAILED ] \e[0m  $@" && echo 1 >> .command.status );

.SILENT:
%.nb.test : %.nb %.nb.exp $(EXEC)
		(./$(EXEC) $< | diff $(word 2, $?) - >> .command.result &&\
		(echo "\e[32m\t [ PASSED ] \e[0m  $<")) && echo 0 >> .command.status ||\
    (echo "\e[31m\t [ FAILED ] \e[0m  $<" && echo 1 >> .command.status );


clean:
	rm *.yy.c ||:
	rm *.out ||: