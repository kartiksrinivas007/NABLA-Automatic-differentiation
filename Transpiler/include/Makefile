LIBDIR = ../lib
CXX = clang++

OBJECTS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: $(OBJECTS)
	mkdir -p $(LIBDIR)
	ar rcs $(LIBDIR)/libnb.a $(OBJECTS)

%.o: %.cpp
	$(CXX) -c -fPIC -o $@ $<

clean:
	rm -f *.o