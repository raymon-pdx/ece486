// Author: Meng Lei
// PDP-8 memory simulator
// memory.cpp: memory implementation

#include <iostream>
#include <cstring>
#include <cctype>
#include "memory.h"

pagetable::pagetable(int numberofpages, int capacityofpage)
{
	number_of_pages = numberofpages;
	capacity_of_page = capacityofpage;

	Table = new entry *[number_of_pages, capacity_of_page];
}

pagetable::~pagetable()
{
	for (int i = 0; i < number_of_pages; ++i)
	{
		for (int j = 0; j < capacity_of_page; ++j)
		{
			if (Table[i, j])
			{
				delete Table[i, j];
				Table[i, j] = NULL;
			}
		}
	}
	Table = NULL;

	number_of_pages = 0;
	capacity_of_page = 0;
}

int pagetable::add(entry & to_add)
{
	// Assume the address does not exist,
	// or overwrite permission granted
	Table[to_add.pagenumber, to_add.offset] = new entry; // Create a new PTE

	Table[to_add.pagenumber, to_add.offset]->pagenumber = to_add.pagenumber;
	Table[to_add.pagenumber, to_add.offset]->offset = to_add.offset;
	Table[to_add.pagenumber, to_add.offset]->word = to_add.word;
	// Copy address and data values in to PTE

	return true;
}

int pagetable::clear(int pagenumber, int offset)
{
	// Assume the address is exists
	// and delete permission granted
	Table[pagenumber, offset]->pagenumber = 0;
	Table[pagenumber, offset]->offset = 0;
	Table[pagenumber, offset]->word = 0;
	// Delete address and data values

	delete Table[pagenumber, offset]; // Delete PTE

	Table[pagenumber, offset] = NULL;

	return 1;
}

int pagetable::retrieve(int pagenumber, int offset, entry & retrieved)
{
	// Assume the address exists
	retrieved.pagenumber = Table[pagenumber, offset]->pagenumber;
	retrieved.offset = Table[pagenumber, offset]->offset;
	retrieved.word = Table[pagenumber, offset]->word;

	return 1;
}

int pagetable::breakdown(int address, int & result_pagenumber, int & result_offset)
{
	temp_mask = 
}