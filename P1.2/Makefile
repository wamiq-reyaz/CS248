CC = clang++
CFLAGS = -std=c++11 -g -Wall

all: mesh.o main.cpp
	$(CC) $(CFLAGS) main.cpp mesh.o -o main

mesh mesh.o: mesh.h mesh.cpp vec.h mat.h
	$(CC) $(CFLAGS) -c mesh.cpp -o mesh.o

clean:
	rm -f *.o main