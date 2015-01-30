// Author: Meng Lei
// PDP-8 memory simulator
// memory.h: header file for memory implementation

#include <iostream>
#include <cstring>
#include <cctype>

const int RegSize = 12; // 12 bits = 4096
const int PageSize = 5; // 5 bits = 32
const int LineSize = 7; // 7 bits = 128

// Each PTE is handled by a ptr in page table
// One entry contains:
struct entry
{
	int pagenumber=0; // page number (5 bits by default)
	int offset=0; // page offset (7 bits by default)
	int word=0; // data (12 bits by default)
};

class pagetable
{
public:
	pagetable(int numberofpages, int capacityofpage); // constructor
	~pagetable(); // deconstructor
	int load(int address); // wrapper function interacting with "bits" module
	int store(int address, int value); // wrapper function interacting with "bits" module
	int probe(int address); // wrapper function containing "display"
private:
	int add(entry & to_add); // add new memory unit data into page table
	int clear(int pagenumber, int offset); // clear a certain memory unit
	int retrieve(int pagenumber, int offset, entry & retrieved); // retrieve memory unit from page table
	int display(int pagenumber, int offset); // quick display of an entry
	int breakdown(int address, int & result_pagenumber, int & result_offset);// break down addressing format
	int number_of_pages;
	int capacity_of_page;
	entry ** Table;
};