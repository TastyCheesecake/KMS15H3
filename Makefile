CC=gcc
CFLAGS=-std=c11 -Wall -O3 -pedantic -fopenmp -march=native
ASFLAGS=-Wall -O3 -pedantic
LDFLAGS=-fopenmp

all: library

.PHONY: all clean

library: util.o fastmd5.o md5.o words.o library.o
	$(CC) $(LDFLAGS) -o library util.o fastmd5.o md5.o words.o library.o

library.o: library.c words.h util.h

words.o: words.c md5.h fastmd5.h

util.o: util.c util.h

fastmd5.o: fastmd5.c fastmd5.h

clean:
	rm -f library
	rm -f library.exe
	rm -f library.o
	rm -f md5.o
	rm -f words.o
	rm -f util.o
	rm -f fastmd5.o
