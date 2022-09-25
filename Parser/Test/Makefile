# prog-tests := $(wildcard *.nb)
prog-test := $(addsuffix .nb.test, $(basename $(wildcard *.nb)))
EXEC = ../parser.out

.PHONY: test 

.SILENT:
test: $(prog-test) $(EXEC)
	echo "\n"
	# @echo "\e[32mAll test cases passed succesfully\e[0m"
$(EXEC) : 
	cd .. && make


%.nb.test : %.nb
	./$(EXEC)  $<


# test :%.nb $(EXEC)
# 		echo 


