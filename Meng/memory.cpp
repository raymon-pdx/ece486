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

	head = new entry *[number_of_pages];

	for (int i = 0; i < number_of_pages; ++i) // contruct an array of "heads"
	{
		head[i] = new entry;
		entry * temp = head[i]; // grab on to each head
		for (int j = 0; j < capacity_of_page; ++j) // construct linear linked list for PTEs
		{
			temp->pagenumber = i;
			temp->offset = j;
			temp->word = 0;
			temp->next = false;
			temp->next = new entry; // new entry
			temp = temp->next; // grab the next one
		}
		temp = NULL; // Set the last "next" to NULL
	}

}

pagetable::~pagetable()
{
	for (int i = 0; i < number_of_pages; ++i)
	{
		delete head[i];
		head[i] = NULL;
	}

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