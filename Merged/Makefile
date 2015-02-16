# this is a makefile for PDP8
CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCE=main.cpp bits.cpp memory.cpp parser.cpp summary.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=pdp8

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) -o $@
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@