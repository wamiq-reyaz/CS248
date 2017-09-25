CC = clang++
CFLAGS = -Wall -Wpedantic -std=c++11

quat: quat.h vec.h mat.h
	$(CC) $(CFLAGS) -c quat.h

mat: mat.h vec.h quat.h
	$(CC) $(CFLAGS) -c mat.h

test_main test_main.o: test_main.cpp ../common/catch.hpp 
	$(CC) $(CFLAGS) test_main.cpp -o test_main.o -c

test: test_main.o test.cpp mat.h vec.h quat.h
	$(CC) $(CFLAGS) test.cpp test_main.o -o test