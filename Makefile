.PHONY: clean

CXXFLAGS= --std=c99 -lomp -O2
OMPFLAGS= -Xpreprocessor -fopenmp

all: mp wo

wo: main.c
	gcc main.c -o wo $(CXXFLAGS)

mp: main.c
	gcc main.c -o mp $(CXXFLAGS) $(OMPFLAGS)

clean:
	rm -f *.o wo mp
