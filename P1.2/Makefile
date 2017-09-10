CC = clang++
CFLAGS = -std=c++11 -g -Wall -Wpedantic

main: mesh.o main.cpp
	$(CC) $(CFLAGS) main.cpp mesh.o -o main

test: catch.hpp test-main.o test.cpp
	$(CC) -std=c++11 test.cpp test-main.o mesh.o -o test

test-main.o: test-main.cpp
	$(CC) -std=c++11 test-main.cpp -c

mesh mesh.o: mesh.h mesh.cpp vec.h mat.h
	$(CC) $(CFLAGS) -c mesh.cpp -o mesh.o

clean:
	rm -f *.o main