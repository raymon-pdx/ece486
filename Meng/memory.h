// Author: Meng Lei
// PDP-8 memory simulator
// memory.h: header file for memory implementation

#include <iostream>
#include <cstring>
#include <cctype>

struct entry
{
	int pagenumber=0;
	int offset=0;
	int word=0;
	bool occupied = false;
	entry * next;
};

class pagetable
{
public:
	pagetable(int numberofpages, int capacityofpage); // constructor
	~pagetable(); // deconstructor
	int add(entry & to_add); // add new memory unit data into page table
	int clear(int pagenumber, int offset); // clear a certain memory unit
	int retrieve(int pagenumber, int offset, entry & retrieved); // retrieve memory unit from page table
private:
	int number_of_pages;
	int capacity_of_page;
	entry ** head;
};