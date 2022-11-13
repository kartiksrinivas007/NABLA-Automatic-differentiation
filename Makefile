LEXERDIR = Lexer/
PARSERDIR = Parser/
LEXERFILE = $(LEXERDIR)lexer.l
PARSERFILE = $(PARSERDIR)grammar.y
BUILDDIR = Build/
TARGET = exec
CPPFLAGS = -std=c++17

.PHONY: build run test_lexer test_parser clean

$(TARGET): $(LEXERFILE) $(PARSERFILE)
	mkdir -p $(BUILDDIR)
	cd $(BUILDDIR);\
	bison -d ../$(PARSERFILE); lex ../$(LEXERFILE);\
	clang++ ${CPPFLAGS} grammar.tab.c lex.yy.c -o $(TARGET)
	cp $(BUILDDIR)$(TARGET) $(TARGET)

build: Parser/grammar.tab.c Lexer/lex.yy.c ast/ast.cpp ast/ast.h
	mkdir -p $(BUILDDIR)
	clang++ ${CPPFLAGS} -o ${TARGET} Parser/grammar.tab.c Lexer/lex.yy.c ast/ast.cpp
	cp ${TARGET} $(BUILDDIR)${TARGET}

debug: Parser/grammar.tab.c Lexer/lex.yy.c ast/ast.cpp ast/ast.h copy_all
	mkdir -p $(BUILDDIR)
	cd $(BUILDDIR) &&\
	g++ -g -o ${TARGET} grammar.tab.c lex.yy.c ast.cpp ${CPPFLAGS} -o debug.out

copy_all:
	cp  Lexer/*.l $(BUILDDIR)
	cp  Lexer/*.c $(BUILDDIR)
	cp  Parser/*.h $(BUILDDIR)
	cp  Parser/*.c $(BUILDDIR)
	cp  Parser/*.y $(BUILDDIR)
	cp  ast/*.h $(BUILDDIR)
	cp  ast/*.cpp $(BUILDDIR)
	cp  test.nb $(BUILDDIR)

Parser/grammar.tab.c: Parser/grammar.y
	cd Parser && make build

Lexer/lex.yy.c: Lexer/lexer.l Parser/grammar.tab.h
	cd Lexer && make build

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
	rm -f $(BUILDDIR)/*

our_build: ${LEXERFILE} ${PARSERFILE} ast/ast.cpp ast/ast.h
	flex ${LEXERFILE} && mv lex.yy.c Lexer/lex.yy.c
	bison -d ${PARSERFILE} && mv grammar.tab.c Parser/grammar.tab.c && mv grammar.tab.h Parser/grammar.tab.h
	clang++ ${CPPFLAGS} Parser/grammar.tab.c Lexer/lex.yy.c ast/ast.cpp -o ${TARGET} 

our_build2: ${LEXERFILE} ${PARSERFILE} ast/ast.cpp ast/ast.h
	flex ${LEXERFILE} && mv lex.yy.c Lexer/lex.yy.c
	bison -d ${PARSERFILE} && mv grammar.tab.c Parser/grammar.tab.c && mv grammar.tab.h Parser/grammar.tab.h
	clang++ ${CPPFLAGS} Parser/grammar.tab.c Lexer/lex.yy.c ast/ast.cpp semantic/traversals.cpp -o ${TARGET} 
