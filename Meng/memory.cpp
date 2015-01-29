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
	for (int i = 0; i < number_of_pages; ++i)
	{
		if (head[i]->pagenumber == to_add.pagenumber)
		{
			entry * temp = head[i];
			for (int j = 0; j < capacity_of_page; ++j)
			{
				if (temp->offset == to_add.offset)
				{
					temp->word == to_add.word;
					temp->occupied == true;
				}
			}
		}
	}
}

int pagetable::clear(int pagenumber, int offset)
{
	for (int i = 0; i < number_of_pages; ++i)
	{
		if (head[i]->pagenumber == pagenumber)
		{
			entry * temp = head[i];
			for (int j = 0; j < capacity_of_page; ++j)
			{
				if (temp->offset == offset)
				{
					temp->word = 0;
					temp->occupied = false;
				}
			}
		}
	}
}

int pagetable::retrieve(int pagenumber, int offset, entry & retrieved)
{
	for (int i = 0; i < number_of_pages; ++i)
	{
		if (head[i]->pagenumber == pagenumber)
		{
			entry * temp = head[i];
			for (int j = 0; j < capacity_of_page; ++j)
			{
				if (temp->offset == offset)
				{
					retrieved.pagenumber = temp->pagenumber;
					retrieved.offset = temp->offset;
					retrieved.word = temp->word;
				}
			}
		}
	}
}