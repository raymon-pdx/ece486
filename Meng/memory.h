// Author: Meng Lei
// PDP-8 memory simulator
// memory.h: header file for memory implementation

#include <iostream>
#include <cstring>
#include <cctype>

// Each PTE is handled by a ptr in page table
// One entry contains:
struct entry
{
	int pagenumber=0; // page number (5 bits by default)
	int offset=0; // page offset (7 bits by default)
	int word=0; // data (12 bits by default)
	bool occupied = false; // When page table consturcted, set unoccupied by default
};

class pagetable
{
public:
	pagetable(int numberofpages, int capacityofpage); // constructor
	~pagetable(); // deconstructor

private:
	int add(entry & to_add); // add new memory unit data into page table
	int clear(int pagenumber, int offset); // clear a certain memory unit
	int retrieve(int pagenumber, int offset, entry & retrieved); // retrieve memory unit from page table
	int number_of_pages;
	int capacity_of_page;
	entry ** head;
};