
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

#define TEST_FILE "test.txt" // used for debugging
#define NUM_PAGES 32  // number of pages in memory
#define PAGE_CAPACITY 128 // number of addresses per page
#define STARTING_ADDRESS 128 // where program is stored


int main(int argc, char *argv[])
{
	BitTwiddle bits;    // class used for instructions
	ifstream myFile;    // hold trace file
	bool SILENT = true; // silent is set
	int loadOffset = 0;  // offset used to load data from text file
	int lineCount = 0;
	int address = 0;
	int id = 0;
	int ret = 0;
	
	// INITIALIZATION
	pagetable Memory(NUM_PAGES, PAGE_CAPACITY);  // initialize memory
	
	// check for program error
	if (argc < 1)
	{
		std::cout << "ERROR - program execution\n";
	}

	// check for trace file name
	if (argc < 2)
	{
		std::cout << "Please pass object file as first argument\n";
	}
	else
	{
		// open pass file
		myFile.open(argv[1]);
	}	
	
	// check for silent mode
	if(argc > 2)
	{
		// turn on additional debugging info
		if(strcmp(argv[2], "1") == 0)
		{
			SILENT = false;
		}
	}
	
	if(myFile.is_open())
	{	
		if(DEBUG || !SILENT)
			{
				cout << "\n***LOADING FILE INTO MEMORY***\n";
			}
		// place all data in file to memory
		while (!myFile.eof())
		{
			// get an address from the file
			ret = getAddress(myFile, id, address, lineCount);
			
			// check for errors getting address
			if (ret < 0)
			{   // handle error and exit
				cout << "ERROR - failed to get address\n";
				goto EXIT;
			}
			
			// display error if not finished with file
			if ((ret > 0) && !myFile.eof())
			{   // handle error and exit
				cout << "ERROR found on line" << lineCount << endl;
				goto EXIT;
			}

			if((DEBUG || !SILENT) && !myFile.eof())
			{
				cout << "\n***MEMORY ACCESS***\n";
				cout << "Address = " << STARTING_ADDRESS + loadOffset << ", Data passed = " << address << endl;
			}
		
			// check that not end of file before loading memory
			if(!myFile.eof())
			{
				// add data to octal address 200 + offset
				if (Memory.store(STARTING_ADDRESS + loadOffset, address) < 0)
				{
					cout << "ERROR - adding data failed\n";
					goto EXIT;
				}
				if(DEBUG || !SILENT)
				{
					// display what is in the address
					Memory.display(STARTING_ADDRESS + loadOffset);

					if ((ret = Memory.load(STARTING_ADDRESS + loadOffset)) != address)
					{
						cout << "ERROR - memory does not match passed data\n";
						goto EXIT;
					}
					cout << "Data retrieved = " << ret << endl;
				}
			}

			++loadOffset;
		}  // eof?
		
		// give additional information that file is loaded
		if(DEBUG || !SILENT)
			{
				cout << "\n***FINISHED LOADING FILE***\n";
			}
	} // end is_open()
    else
    {
        cout << "ERROR - file not found\n";
        goto EXIT;
    }
	
EXIT:
	// close the file
	myFile.close();
	exitMessage();
	return 0;
}	


/*FAUST PSEUDOCODE FROM CLASS*/
/*
  Running <- 1
  PC <- 200o
  while(Running)
      <- MEM[PC]
	  PC++
	  case IR<12:10>  // decode
	    ---
		---
		---
		---           // execute
		---
		---
		---
      print statistics
	  close files
*/


