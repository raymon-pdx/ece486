# this is a makefile for testing the functions
# used in the ECE 486 ISA Simulator 
# Author: Sean Hendrickson
# Date: 12/13/2014
CC=g++
CFLAGS=-c -Wall

all: test_isa_functions

test_isa_functions: main.o
	$(CC) main.o -o test_isa_functions

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp
	
clean:
	rm -rf *o test_isa_functions
