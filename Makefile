LEXERDIR = Lexer/
PARSERDIR = Parser/
LEXERFILE = $(LEXERDIR)lexer.l
PARSERFILE = $(PARSERDIR)grammar.y
BUILDDIR = Build/
TARGET = parser.out

.PHONY: build run test_lexer test_parser clean

$(TARGET): $(LEXERFILE) $(PARSERFILE)
	mkdir -p $(BUILDDIR)
	cd $(BUILDDIR);\
	yacc -d ../$(PARSERFILE); lex ../$(LEXERFILE);\
	gcc y.tab.c lex.yy.c -o $(TARGET)
	cp $(BUILDDIR)$(TARGET) $(TARGET)

build: $(TARGET)

test_lexer: 
	cd $(LEXERDIR); make test

test_parser:
	cd $(PARSERDIR); make test

clean:
	rm -f *.out
	rm -f **/*.out
	rm -f *.tab.c
	rm -f **/*.tab.c
	rm -f *.tab.h
	rm -f **/*.tab.h
	rm -f *.yy.c
	rm -f **/*.yy.c