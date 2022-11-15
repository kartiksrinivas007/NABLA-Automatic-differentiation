EXEC = parser.out
BUILDDIR = Build
BISON = bison
CC = clang++
CFLAGS = -g -std=c++17

OBJECTS = $(BUILDDIR)/grammar.tab.o $(BUILDDIR)/lex.yy.o

.SILENT: test run
.PHONY: clean build test run

$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) ${OBJECTS} $(CFLAGS)

debug: $(OBJECTS)
	$(CC) -o debug.out ${OBJECTS} $(CFLAGS)

grammar.tab.c grammar.tab.h: grammar.y
	$(BISON) -d grammar.y
	
lex.yy.c: lexer.l grammar.tab.h
	lex lexer.l

$(BUILDDIR)/%.o: %.c
	mkdir -p $(BUILDDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

test: ${EXEC}
	cd Test && ./test.sh

clean:
	rm -f *.yy.c
	rm -f *.tab.c
	rm -f *.tab.h
	rm -f *.out
	rm -f $(BUILDDIR)/*.o
	rm -f $(BUILDDIR)/*.out