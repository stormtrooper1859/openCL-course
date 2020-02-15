.PHONY: clean

CXXFLAGS= --std=c++11 -Xpreprocessor -fopenmp -lomp

all: hello

hello: main.o
	g++ main.o -o hello $(CXXFLAGS)

main.o: main.cpp
	g++ -c main.cpp $(CXXFLAGS)

clean:
	rm -f *.o hello
