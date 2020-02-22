.PHONY: clean

CXXFLAGS= --std=c99 -lomp -O2 -framework OpenCL
OMPFLAGS= -Xpreprocessor -fopenmp

all: mp wo

wo: main.c
	gcc main.c -o wo $(CXXFLAGS)

mp: main.c
	gcc main.c -o mp $(CXXFLAGS) $(OMPFLAGS)

clean:
	rm -f *.o wo mp
