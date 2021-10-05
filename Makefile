CC = g++
CFLAGS = -Wall -g

main: main.o SingletonTable.o 
	$(CC) $(CFLAGS) -o main main.o SingletonTable.o
 
main.o: main.cpp SingletonTable.h 
	$(CC) $(CFLAGS) -c main.cpp
 
SingletonTable.o: SingletonTable.h