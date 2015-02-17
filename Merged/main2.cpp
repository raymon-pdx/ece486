/*
* AUTHORS: Ommaimah Hussein Mohammed,
*          Even Sprecher,
*          Meng Lei,
*          Sean Hendrickson
* FILE: main.cpp
* LAST MODIFIED: 2/16/2015
* DESCRIPTION: a program that simulates the PDP8 instruction set
*/
#include "parser.h"
#include "memory.h"
#include "bits.h"
using namespace std;

#define TEST_FILE "test2.txt" // used for debugging
#define NUM_PAGES 32  // number of pages in memory
#define PAGE_CAPACITY 128 // number of addresses per page
#define STARTING_ADDRESS 128 // where program is stored


int main(int argc, char *argv[])
{
	BitTwiddle bits;    // class used for instructions
	ifstream myFile;    // hold trace file
	ofstream outputFile;  // show cleaned file
	int loadOffset = 0;  // offset used to load data from text file
	int opcode = 0;
	bool I = false;
	bool M = false;
	int offset = 0;
	int lineCount = 0;
	int address = 0;
	int id = 0;
	int ret = 0;
	int count = 0;
	
	// INITIALIZATION
	myFile.open(TEST_FILE);
	pagetable Memory(NUM_PAGES, PAGE_CAPACITY);  // initialize memory
	
	// write to an output file to see the cleaned file
	while (!myFile.eof())
	{
		count++;
		
		// break down the address
		ret = parseAddress(address, opcode, I, M, offset);
		if (ret < 0)
		{   // handle error and exit
			cout << "ERROR - failed to parse address on attempt " << count <<  endl;
			goto EXIT;
		}

		if(DEBUG)
		{
			cout << "Address = " << STARTING_ADDRESS + loadOffset << ", Data passed = " << address << endl;
		}
		
		// add data to octal address 200
		if (Memory.store(STARTING_ADDRESS + loadOffset, address) < 0)
		{
			cout << "ERROR - adding data failed\n";
			goto EXIT;
		}

		if (ret = Memory.load(STARTING_ADDRESS + loadOffset) != address)
		{
			cout << "ERROR - memory does not match passed data\n";
			goto EXIT;
		}
		if(DEBUG)
		{
			cout << "Data retrieved = " << ret << endl << endl;
		}
		
		++loadOffset;
	}
	
EXIT:
	// close the file
	myFile.close();
	exitMessage();
	return 0;
}	


