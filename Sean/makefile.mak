# this is a makefile for testing the functions
# used in the ECE 486 ISA Simulator 
# Author: Sean Hendrickson
# Date: 12/13/2014
CC=g++
CFLAGS=-c -Wall

all: isa_simulator

isa_simulator: ISASimulator.o
	$(CC) ISASimulator.o -o isa_simulator

ISASimulator.o: ISASimulator.cpp
	$(CC) $(CFLAGS) ISASimulator.cpp
	
clean:
	rm -rf *o isa_simulator
