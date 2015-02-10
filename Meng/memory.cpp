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

	for (int i = 0; i < number_of_pages; ++i)
	{
		for (int j = 0; j < capacity_of_page; ++j)
		{
			if (Table[i, j])
			{
				Table[i, j] = NULL; // Set every ptr in 2d array to NULL
			}
		}
	}
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
				Table[i, j] = NULL; // Set every ptr in 2d array to NULL
			}
		}
	}
	Table = NULL; // Clear entire page table

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
	// Assume the address exists
	// and delete permission granted
	Table[pagenumber, offset]->pagenumber = 0;
	Table[pagenumber, offset]->offset = 0;
	Table[pagenumber, offset]->word = 0;
	// Delete address and data values

	delete Table[pagenumber, offset]; // Delete PTE

	Table[pagenumber, offset] = NULL; // Clear this ptr

	return 1;
}

int pagetable::retrieve(int pagenumber, int offset, entry & retrieved)
{
	// Assume the address exists
	retrieved.pagenumber = Table[pagenumber, offset]->pagenumber;
	retrieved.offset = Table[pagenumber, offset]->offset;
	retrieved.word = Table[pagenumber, offset]->word;

	return 1; // execution successful
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
	if (Table[pagenumber,offset] == NULL)
		return 0; // Everything is 0, the address doesn't exist
	else return 1; // The address exists return true;
}

int pagetable::probe(int address)
{
	int pagenumber, offset;
	breakdown(address, pagenumber, offset);

	if (probe(pagenumber, offset))
		return 1; // If address exists return true;
	else return 0; // Otherwise return 0;	
}

int pagetable::load(int address)
{
	int pagenumber, offset;
	breakdown(address, pagenumber, offset);

	if (probe(pagenumber, offset))
	{
		// If address exists, start to load memory
		entry temp;
		if (retrieve(pagenumber, offset, temp))
		{
			return temp.word; // Return data word
		}
		else return -1; // Retrieve function failed
	}
	else
	{
		// Address does not exist
		cout << "Address non-existent" << endl;
		return -1;
	}

}

int pagetable::store(int address, int value)
{
	int pagenumber, offset;
	breakdown(address, pagenumber, offset);

	if (probe(pagenumber, offset))
	{
		// If address exists, prompt user
		char response = 'n';
		cout << "Address occupied; Overwrite?";
		cin >> response;
		cin.ignore(100, '\n');
		if (toupper(response) == 'Y')
		{
			// Overwrite granted
			entry to_add;
			to_add.pagenumber = pagenumber;
			to_add.offset = offset;
			to_add.word = value;
			if (add(to_add))
				return 1; // Execution successful
			else return -1; // "add" function failed
		}
		else
		{
			// User chose not to overwrite
			cout << "Overwrite aborted" << endl;
			return 1;
		}
	}
	else
	{
		// Address does not exist, store entry
		entry to_add;
		to_add.pagenumber = pagenumber;
		to_add.offset = offset;
		to_add.word = value;
		if (add(to_add))
			return 1; // Execution successful
		else return -1; // "add" function failed
	}
	return 0; // For some unknown reason it got to this point
}

int pagetable::display(int address)
{
	int pagenumber, offset;
	breakdown(address, pagenumber, offset);

	if (probe(pagenumber, offset))
	{
		// If address exists, start to display
		cout << "Page: " << Table[pagenumber, offset]->pagenumber << endl;
		cout << "Offset: " << Table[pagenumber, offset]->offset << endl;
		cout << "Data word: " << Table[pagenumber, offset]->word << endl;
		return 1;
	}
	else
	{
		// Address is empty
		cout << "Address non-existent" << endl;
		return 0;
	}
}

int pagetable::clear(int address)
{
	int pagenumber, offset;
	breakdown(address, pagenumber, offset);

	if (probe(pagenumber, offset))
	{
		// If address exists, prompt user
		char response = 'n';
		cout << "Address occupied; Delete?";
		cin >> response;
		cin.ignore(100, '\n');
		if (toupper(response) == 'Y')
		{
			// Delete granted
			if (clear(pagenumber, offset))
				return 1; // Execution successful
			else
			{
				cout << "Delete failed" << endl;
				return -1;
			}
		}
		else
		{
			// User chose not to delete
			cout << "Delete aborted" << endl;
			return 1;
		}
	}
	else
	{
		cout << "Address non-existent" << endl;
		return 0;
	}
}