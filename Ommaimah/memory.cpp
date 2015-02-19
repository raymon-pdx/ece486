// Author: Meng Lei
// PDP-8 memory simulator
// memory.cpp: memory implementation

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <cstring>
#include <cctype>
#include <cmath>
#include <bitset>
#include "memory.h"

using namespace std;

#define timeStamp /*OPTIONAL TIMESTAMP*/

pagetable::pagetable(int numberofpages, int capacityofpage)
{
	number_of_pages = numberofpages;
	capacity_of_page = capacityofpage;

	Table = new entry **[number_of_pages];

	for (int i = 0; i < number_of_pages; i++)
	{
		Table[i] = new entry *[capacity_of_page];
	}

	for (int i = 0; i < number_of_pages; i++)
	{
		for (int j = 0; j < capacity_of_page; j++)
		{
			Table[i][j] = NULL; // Set every ptr in 2d array to NULL
		}
	}
}

pagetable::~pagetable()
{
	for (int i = 0; i < number_of_pages; i++)
	{
		for (int j = 0; j < capacity_of_page; j++)
		{
			if (Table[i][j])
			{
				delete Table[i][j];
				Table[i][j] = NULL; // Set every ptr in 2d array to NULL
			}
		}
		delete Table[i];
		Table[i] = NULL;
	}
	delete Table;
	Table = NULL; // Clear entire page table

	number_of_pages = 0;
	capacity_of_page = 0;
}

int pagetable::add(entry & to_add)
{
	// Assume the address does not exist,
	// or overwrite permission granted
	Table[to_add.pagenumber][to_add.offset] = new entry; // Create a new PTE

	Table[to_add.pagenumber][to_add.offset]->pagenumber = to_add.pagenumber;
	Table[to_add.pagenumber][to_add.offset]->offset = to_add.offset;
	Table[to_add.pagenumber][to_add.offset]->word = to_add.word;
	// Copy address and data values in to PTE
	return true;
}

int pagetable::clear(int pagenumber, int offset)
{
	// Assume the address exists
	// and delete permission granted
	Table[pagenumber][offset]->pagenumber = 0;
	Table[pagenumber][offset]->offset = 0;
	Table[pagenumber][offset]->word = 0;
	// Delete address and data values

	delete Table[pagenumber][offset]; // Delete PTE

	Table[pagenumber][offset] = NULL; // Clear this ptr

	return 1;
}

int pagetable::retrieve(int pagenumber, int offset, entry & retrieved)
{
	// Assume the address exists
	retrieved.pagenumber = Table[pagenumber][offset]->pagenumber;
	retrieved.offset = Table[pagenumber][offset]->offset;
	retrieved.word = Table[pagenumber][offset]->word;

	return 1; // execution successful
}

int pagetable::breakdown(int address, int & result_pagenumber, int & result_offset)
{
	int temp_mask = (1 << (PageSize + LineSize)) - 1;
	// Create mask with 1's, same length as EAddr

	int temp_address = address & temp_mask;
	// Mask EAddr to get rid of garbage bits

	temp_mask = (1 << (LineSize)) - 1;
	// Create mask with 1's, same length as offset

	result_offset = temp_address & temp_mask;
	// Mask temp_address to get rid of page number bits
	result_pagenumber = temp_address >> LineSize;
	// Right shift temp_address to get rid of offset bits

	return 1;
}



string pagetable::intToOctal(int value)
{
	std::string octalString = "";
	std::bitset<(PageSize+LineSize)> aString(value);
	std::string binaryString = aString.to_string();

	// step through entire string to get octal value
	for (int i = 0; i < (PageSize+LineSize); i += 3)
	{
		// find matching string
		if (binaryString.compare(i, 3, "000") == 0)
		{
			octalString += "0";
		}
		else if (binaryString.compare(i, 3, "001") == 0)
		{
			octalString += "1";
		}
		else if (binaryString.compare(i, 3, "010") == 0)
		{
			octalString += "2";
		}
		else if (binaryString.compare(i, 3, "011") == 0)
		{
			octalString += "3";
		}
		else if (binaryString.compare(i, 3, "100") == 0)
		{
			octalString += "4";
		}
		else if (binaryString.compare(i, 3, "101") == 0)
		{
			octalString += "5";
		}
		else if (binaryString.compare(i, 3, "110") == 0)
		{
			octalString += "6";
		}
		else if (binaryString.compare(i, 3, "111") == 0)
		{
			octalString += "7";
		}
		else
		{   // some error occured
			octalString = "";
			return octalString;
		}
	}
	return octalString;
}

int pagetable::probe(int pagenumber, int offset)
{
	if (Table[pagenumber][offset] == NULL)
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

    //*****TESTING TRACEFILE HERE*********************
    cout << "Reached the load/read function!\n";

    int type =0; //type 0 indicates read    
    traceFile(type, address);
    //************************************************


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

    //*****TESTING TRACEFILE HERE*********************
    cout << "Reached the store/write function!\n";

    int type = 1; //type 1 indicates write    
    traceFile(type, address);
    //************************************************

	int pagenumber, offset;
	breakdown(address, pagenumber, offset);

	if (value >= int(pow(2, RegSize))) // See if value to be stored exceeded max value
	{
		cout << "Value larger than register size; Will be trimmed down to "
			<< RegSize << " bits!\n";
		int temp_mask = 0;
		for (int i = 0; i < RegSize; ++i)
		{
			temp_mask |= temp_mask | (1 << i);
		}// Create mask with 1's, length of register size
		cout << "Original value: " << value << endl;
		value = value & temp_mask;
		// Trim value down to register size
		cout << "Trimmed down to: " << value << endl;
	}

	// Don't check if address exists, overwrite without prompt
	entry to_add;
	to_add.pagenumber = pagenumber;
	to_add.offset = offset;
	to_add.word = value;
	if (add(to_add))
		return 1; // Execution successful
	else return -1; // "add" function failed

}

int pagetable::display(int address)
{
	int pagenumber, offset;
	breakdown(address, pagenumber, offset);

	if (probe(pagenumber, offset))
	{
		// If address exists, start to display
		cout << "Page: " << Table[pagenumber][offset]->pagenumber << endl;
		cout << "Offset: " << Table[pagenumber][offset]->offset << endl;
		cout << "Data word: " << Table[pagenumber][offset]->word << endl;
		return 1;
	}
	else
	{
		// Address is empty
		cout << "Address non-existent" << endl;
		return 0;
	}
}

int pagetable::display_all()
{
	int previous_pagenumber = -1; // Specify starting pagenumber, so when it starts, pagenumber changes from -1 to 0
	
	for (int i = 0; i < int(pow(2, PageSize)); i++) // Loop vertically
	{
		// Here i is the pagenumber
		int previous_offset = -2; // Specify starting offset, reset to -2 every page

		for (int j = 0; j < int(pow(2, LineSize)); j++) // Loop horizontally
		{
			if (probe(i, j)) // We encountered valid data
			{
				if (i != previous_pagenumber) // Now we are on a different page
				{
					cout << "Page Number: " << intToOctal(i) << endl;
					previous_pagenumber = i; // Change previous_pagenumber to current page number
				}
				if (j != (previous_offset + 1)) // If the valid addresses are not continuous
				{
					cout << "\tOffset: " << intToOctal(j) << endl;
				}
				cout << "\t\tData: " << intToOctal(Table[i][j]->word) << endl;
				previous_offset = j; // Store the previous valid address
			}

		}
	}

	return 1;
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

int pagetable::traceFile(int type, int address){

   #ifndef timeStamp
   //call timestamp function
   time_t rawtime;
   time (&rawtime);
   #endif

   //create an output trace file
   ofstream outputTraceFile;
   outputTraceFile.open("TraceFile.txt");

   #ifndef timeStamp
   //add time stamp as header of tracefile
   outputTraceFile << "***** TIME STAMP *****\n";
   outputTraceFile << "Trace file generated: " << ctime(&rawtime);
   #endif

   //use outputTraceFile like cout, but into txt file 
   outputTraceFile << type << " " << oct << address << endl;

   outputTraceFile.close();
   cout << "Trace file generated.\n";

   return 0;

}


