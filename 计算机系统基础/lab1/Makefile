CC = gcc
CFLAGS = -O0 -m32 -Wall

all: btest
btest: btest.c bits.c decl.c tests.c btest.h bits.h
	$(CC) $(CFLAGS) -o btest bits.c btest.c decl.c tests.c

clean:
	rm -f *.o btest

