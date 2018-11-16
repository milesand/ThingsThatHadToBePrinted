CC = gcc
CFLAGS = -Wall

default: main

main: main.o rc.o vec.o maze.o
	$(CC) $(CFLAGS) -o main main.o rc.o vec.o maze.o

rc.o: rc.c rc.h
	$(CC) $(CFLAGS) -c rc.c

vec.o: vec.c vec.h rc.h
	$(CC) $(CFLAGS) -c vec.c

maze.o: maze.c maze.h vec.h rc.h
	$(CC) $(CFLAGS) -c maze.c

main.o: main.c maze.h
	$(CC) $(CFLAGS) -c main.c

clean:
	$(RM) main *.o