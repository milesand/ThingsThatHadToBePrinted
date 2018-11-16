CC = gcc
CFLAGS = -Wall

default: main

main: main.o term.o list.o listpolynomial.o
	$(CC) $(CFLAGS) -o main main.o term.o list.o listpolynomial.o

term.o: term.c term.h
	$(CC) $(CFLAGS) -c term.c

list.o: list.c term.h list.h
	$(CC) $(CFLAGS) -c list.c

listpolynomial.o: listpolynomial.c term.h list.h listpolynomial.h
	$(CC) $(CFLAGS) -c listpolynomial.c

main.o: main.c term.h list.h listpolynomial.h
	$(CC) $(CFLAGS) -c main.c

clean:
	$(RM) main *.o