CC = g++
CFLAGS = -Wall -g
 
main: main.o Cell.o SingletonTable.o Application.o
	$(CC) $(CFLAGS) -o main main.o Cell.o SingletonTable.o Application.o
 
main.o: main.cpp Cell.h SingletonTable.h Application.h
	$(CC) $(CFLAGS) -c main.cpp
 
Cell.o: Cell.h

SingletonTable.o: SingletonTable.h Cell.h

Application.o: Application.h SingletonTable.h Cell.h
