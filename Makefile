.PHONY: clean

CXXFLAGS= --std=c++17

all: hello

hello: main.o lib.o
	g++ main.o lib.o -o hello $(CXXFLAGS)

main.o: main.cpp lib.h
	g++ -c main.cpp $(CXXFLAGS)

lib.o: lib.cpp lib.h
	g++ -c lib.cpp $(CXXFLAGS)

clean:
	rm -f *.o hello
