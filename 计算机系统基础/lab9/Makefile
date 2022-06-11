CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -pthread

all: proxy

proxy.o: proxy.c csapp.h

csapp.o: csapp.c csapp.h

proxy: proxy.o csapp.o

clean:
	rm -f *~ *.o proxy proxy.log
