CC = g++
CFLAGS = -Wall -g
 
main: main.o Cell.o SingletonTable.o
	$(CC) $(CFLAGS) -o main main.o Cell.o SingletonTable.o
 
main.o: main.cpp Cell.h SingletonTable.h
	$(CC) $(CFLAGS) -c main.cpp
 
Cell.o: Cell.h

SingletonTable.o: SingletonTable.h Cell.h

