CFLAGS= -g -Wall -pedantic -std=c99
LDFLAGS= -g

all: test-suma

test-suma.o suma.o: suma.h

test-suma: suma.o test-suma.o

clean:
	rm -f *.o
	rm -f test-suma
