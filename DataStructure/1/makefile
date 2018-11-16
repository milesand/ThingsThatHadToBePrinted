CC = gcc
CFLAGS = -Wall

default: main

main: main.o term.o vec.o sparsematrix.o
	$(CC) $(CFLAGS) -o main main.o term.o vec.o sparsematrix.o

term.o: term.c term.h
	$(CC) $(CFLAGS) -c term.c

vec.o: vec.c vec.h term.h
	$(CC) $(CFLAGS) -c vec.c

sparsematrix.o: sparsematrix.c sparsematrix.h vec.h term.h
	$(CC) $(CFLAGS) -c sparsematrix.c

main.o: main.c sparsematrix.h vec.h term.h
	$(CC) $(CFLAGS) -c main.c

clean:
	$(RM) main *.o