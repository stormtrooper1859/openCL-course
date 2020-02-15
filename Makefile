.PHONY: clean

CXXFLAGS= --std=c99 -Xpreprocessor -fopenmp -lomp

all: hello

hello: main.o
	gcc main.o -o hello $(CXXFLAGS)

main.o: main.c
	gcc -c main.c $(CXXFLAGS)

clean:
	rm -f *.o hello
