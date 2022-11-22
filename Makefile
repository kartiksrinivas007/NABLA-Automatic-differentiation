LEXERDIR = Lexer
PARSERDIR = Parser
SEMANTICDIR = Semantic
TRANSPILERDIR = Transpiler

.PHONY: all lexer parser semantic transpiler test clean

all: lexer parser semantic transpiler

lexer:
	$(MAKE) --no-print-directory -C $(LEXERDIR)

parser:
	$(MAKE) --no-print-directory -C $(PARSERDIR)

semantic:
	$(MAKE) --no-print-directory -C $(SEMANTICDIR)

transpiler:
	$(MAKE) --no-print-directory -C $(TRANSPILERDIR)

test: lexer parser semantic transpiler
	$(MAKE) --no-print-directory -C $(LEXERDIR) test
	$(MAKE) --no-print-directory -C $(PARSERDIR) test
	$(MAKE) --no-print-directory -C $(SEMANTICDIR) test
	# $(MAKE) --no-print-directory -C $(TRANSPILERDIR) test

clean:
	$(MAKE) --no-print-directory -C $(LEXERDIR) clean
	$(MAKE) --no-print-directory -C $(PARSERDIR) clean
	$(MAKE) --no-print-directory -C $(SEMANTICDIR) clean
	$(MAKE) --no-print-directory -C $(TRANSPILERDIR) clean
	rm -f **/*.o **/*.out **/*.exe **/.*.cpp
