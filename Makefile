all: queen

queen: queen.o
	g++ -o queen queen.o

queen.o: main.cpp main_h.h
	g++ -Wall -std=c++11 -c main.cpp

clean:
	rm -rf *.o queen
