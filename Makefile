.PHONY: clean

CXXFLAGS= --std=c99 -lomp
OMPFLAGS= -Xpreprocessor -fopenmp

all: hello

wo: main.c
	gcc main.c -o wo $(CXXFLAGS)

hello: main.o
	gcc main.o -o hello $(CXXFLAGS) $(OMPFLAGS)

main.o: main.c
	gcc -c main.c $(CXXFLAGS) $(OMPFLAGS)

clean:
	rm -f *.o hello
