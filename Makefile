CC = g++
CFLAGS = -g -Wall -std=c++17

all: animator converter

animator: animator.o
	$(CC) -o animator animator.o

animator.o: animator.cpp
	$(CC) $(CFLAGS) -c animator.cpp

converter: converter.o translatePng.o translatePng.h
	$(CC) -o converter converter.o translatePng.o -lpng

converter.o: converter.cpp translatePng.h
	$(CC) $(CFLAGS) -c converter.cpp

translatePng.o: translatePng.cpp translatePng.h
	$(CC) $(CFLAGS) -c translatePng.cpp

.PHONY: clean
clean:
	rm -f *.o converter