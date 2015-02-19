
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
	ifstream myFile;            // hold trace file
	bool SILENT = true;         // silent is set
	bool running = false;       // used to continue program execution
	bool indirect = false;      // value of indirect bit from parsing
	bool currentPage = false;   // value of memory page bit from parsing
	int pc = STARTING_ADDRESS;  // program counter beginning at 200 octal
	int loadOffset = 0;         // offset used to load data from text file
	int lineCount = 0;          // current line of input file
	int address = 0;            // output from input file
	int opcode = 0;             // opcode used to decode instructions
	int offset = 0;             // offset from parsing address
	int id = 0;                 // data or instruction word
	int ret = 0;
	
	// INITIALIZATION
	pagetable Memory(NUM_PAGES, PAGE_CAPACITY);  // initialize memory
	BitTwiddle PDP8;  // initialize cpu ISA class
	
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

	/* BEGIN PROGRAM EXECUTION */
	if (DEBUG || !SILENT)
	{
		cout << "\n***BEGIN PROGRAM EXECUTION***\n";
	}
	
	// begin program execution
	running = true;
	while (running)
	{
		// fetch data
		address = Memory.load(pc);
		if (address < 0)
		{   // error occurred
			cout << "ERROR - address " << pc << " could not be found\n";
			goto EXIT;
		}

		// increment program counter
		++pc;

		// parse the address
		ret = parseAddress(address, opcode, indirect, currentPage, offset);
		if (ret < 0)
		{   // error occurred
			cout << "ERROR - could not parse address\n";
			goto EXIT;
		}

		// decode the address
		switch (opcode)
		{
		case 0:
			if (DEBUG || !SILENT)
			{
				cout << "[AND|" << indirect << "|" << currentPage
                     << "|" << offset << "]\n";
			}
			PDP8.PDP_AND(indirect, currentPage, offset);
			break;
		case 1:
			if (DEBUG || !SILENT)
			{
				cout << "[TAD|" << indirect << "|" << currentPage
					<< "|" << offset << "]\n";
			}
			PDP8.PDP_TAD(indirect, currentPage, offset);
			break;
		case 2:
			if (DEBUG || !SILENT)
			{
				cout << "[ISZ|" << indirect << "|" << currentPage
					<< "|" << offset << "]\n";
			}
			PDP8.PDP_ISZ(indirect, currentPage, offset);
			break;
		case 3:
			if (DEBUG || !SILENT)
			{
				cout << "[DCA|" << indirect << "|" << currentPage
					<< "|" << offset << "]\n";
			}
			PDP8.PDP_DCA(indirect, currentPage, offset);
			break;
		case 4:
			if (DEBUG || !SILENT)
			{
				cout << "[JMS|" << indirect << "|" << currentPage
					<< "|" << offset << "]\n";
			}
			PDP8.PDP_JMS(indirect, currentPage, offset);
			break;
		case 5:
			if (DEBUG || !SILENT)
			{
				cout << "[JMP|" << indirect << "|" << currentPage
					<< "|" << offset << "]\n";
			}
			PDP8.PDP_JMP(indirect, currentPage, offset);
			break;
		default:
			if (DEBUG || !SILENT)
			{
				cout << "[" << opcode << "|" << indirect << "|" 
					 << currentPage	<< "|" << offset << "]\n";
			}
			break;
		}
	}

	// end of program execution
	if (DEBUG || !SILENT)
	{
		cout << "\n***FINISHED EXECUTING PROGRAM***\n";
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


