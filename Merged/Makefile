# this is a makefile for PDP8
CC=g++
CFLAGS=-c -Wall

all: pdp8

pdp8: main.o memory.o bits.o parser.o 
	$(CC) main.o memory.o bits.o parser.o -o pdp8

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

memory.o: memory.cpp
	$(CC) $(CFLAGS) memory.cpp

bits.o: bits.cpp
	$(CC) $(CFLAGS) bits.cpp

parser.o: parser.cpp
	$(CC) $(CFLAGS) parser.cpp

#summary.o: summary.cpp
#	$(CC) $(CFLAGS) summary.cpp

clean:
	rm *o pdp8
