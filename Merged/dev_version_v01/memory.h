// Author: Meng Lei
// PDP-8 memory simulator
// memory.h: header file for memory implementation
#pragma once
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cctype>
#include <bitset>

#ifndef REGSIZE
#define REGSIZE 12
#endif
#ifndef PAGESIZE
#define PAGESIZE 5
#endif
#ifndef LINESIZE
#define LINESIZE 7
#endif

/*
int RegSize; // 12 bits = 4096
int PageSize; // 5 bits = 32
int LineSize; // 7 bits = 128
*/

// Each PTE is handled by a ptr in page table
// One entry contains:
struct entry
{
	int pagenumber; // page number (5 bits by default)
	int offset; // page offset (7 bits by default)
	int word; // data (12 bits by default)
};

class pagetable
{
public:
	pagetable();
	pagetable(int numberofpages, int capacityofpage); // constructor
	~pagetable(); // destructor
	int load(int address); // wrapper function interacting with "bits" module
	int store(int address, int value); // wrapper function interacting with "bits" module
	int probe(int address); // quick probe to see if an address exists
	int display(int address); // quick display of an entry
	int display_all(); // display all memory entries
	int clear(int address); // wrapper function for "clear" in private
private:
	int add(entry & to_add); // add new memory unit data into page table
	int clear(int pagenumber, int offset); // clear a certain memory unit
	int retrieve(int pagenumber, int offset, entry & retrieved); // retrieve memory unit from page table
	int breakdown(int address, int & result_pagenumber, int & result_offset);// break down addressing format
	int probe(int pagenumber, int offset); // Private probe function to see if an adress exists
	std::string intToOctal(int value); // conversion from int to octal
	int number_of_pages;
	int capacity_of_page;
	entry *** Table;
};
