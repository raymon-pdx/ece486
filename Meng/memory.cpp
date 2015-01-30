// Author: Meng Lei
// PDP-8 memory simulator
// memory.cpp: memory implementation

#include <iostream>
#include <cstring>
#include <cctype>
#include "memory.h"

using namespace std;

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
	int temp_mask = 0;
	for (int i = 0; i < PageSize + LineSize; ++i)
	{
		temp_mask |= temp_mask | (1 << i);
	}// Create mask with 1's, same length as EAddr

	int temp_address = address & temp_mask;
	// Mask EAddr to get rid of garbage bits

	temp_mask = 0; // Reset temp_mask to 0;

	for (int i = 0; i < LineSize; ++i)
	{
		temp_mask |= temp_mask | (1 << i);
	}// Create mask with 1's, same length as offset

	result_offset = temp_address & temp_mask;
	// Mask temp_address to get rid of page number bits
	result_pagenumber = temp_address >> LineSize;
	// Right shift temp_address to get rid of offset bits

	return 1;
}

int pagetable::probe(int pagenumber, int offset)
{
	if ((pagenumber == 0) && (offset == 0) && (Table[pagenumber, offset] == 0))
		return 0; // Everything is 0, the address doesn't exist
	else return 1; // The address exists return true;
}

int pagetable::probe(int address)
{
	int pagenumber, offset;
	breakdown(address, pagenumber, offset);

	
}

int pagetable::load(int address)
{
	int pagenumber, offset;
	breakdown(address, pagenumber, offset);

	if ()
}

int pagetable::display(int address)
{
	// Assume the address exists
	cout << "Page: " << Table[pagenumber, offset]->pagenumber << endl;
	cout << "Offset: " << Table[pagenumber, offset]->offset << endl;
	cout << "Data word: " << Table[pagenumber, offset]->word << endl;

	return 1;
}
