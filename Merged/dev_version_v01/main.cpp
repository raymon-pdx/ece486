/*
* AUTHORS: Ommaimah Hussein Mohammed,
*          Even Sprecher,
*          Meng Lei,
*          Sean Hendrickson
* FILE: main.cpp
* LAST MODIFIED: 2/21/2015
* DESCRIPTION: a program that simulates the PDP8 instruction set
*/
#include "constants.h"
#include "parser.h"
#include "memory.h"
#include "bits.h"
using namespace std;



int main(int argc, char *argv[])
{
	ifstream myFile;            // hold trace file
	bool SILENT = true;         // silent is set
	bool running = false;       // used to continue program execution
	bool indirect = false;      // value of indirect bit from parsing
	bool currentPage = false;   // value of memory page bit from parsing
	int lineCount = 0;          // current line of input file
	int address = 0;            // output from input file
	int loadLocation = 0;       // address to load data too
	int opcode = 0;             // opcode used to decode instructions
	int offset = 0;             // offset from parsing address
	int id = 0;                 // data or instruction word
	int ret = 0;
	
	// INITIALIZATION
	pagetable Memory(pdp8::NUM_PAGES, pdp8::PAGE_CAPACITY);  // initialize memory
	BitTwiddle PDP8(&Memory);  // initialize ISA class

	// LOAD FILE INTO MEMORY
	
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
		if (pdp8::DEBUG || !SILENT)
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

			// check for valid id
			if ((id < 0) || (id > 1))
			{
				cout << "ERROR found on line" << lineCount 
					 << ", non-valid id found\n";
				goto EXIT;
			}
				
			// if id = 1 save the address since this is used to load data
			if (id == 1)
			{
				loadLocation = address;
			}
			
			// display before data
			if ((pdp8::DEBUG || !SILENT) && !myFile.eof() && (id == 0))
			{
				cout << "\n***LOAD DATA***\n";
				cout << "Address = " << loadLocation << ", Data passed = " << address << endl;
			}

			// display for address
			if ((pdp8::DEBUG || !SILENT) && !myFile.eof() && (id == 1))
			{
				cout << "\n***NEW ADDRESS***\n";
				cout << "Address = " << loadLocation << endl;
			}
		
			// check that not end of file before loading memory
			// and that address is data
			if(!myFile.eof() && (id == 0))
			{
				// add data to octal address 200 + offset
				if (Memory.store(loadLocation, address) < 0)
				{
					cout << "ERROR - adding data failed\n";
					goto EXIT;
				}
				if (pdp8::DEBUG || !SILENT)
				{
					// display what is in the address
					Memory.display(loadLocation);

					if ((ret = Memory.load(loadLocation)) != address)
					{
						cout << "ERROR - memory does not match passed data\n";
						goto EXIT;
					}
					cout << "Data retrieved = " << ret << endl;
				}
				++loadLocation;  // store data to next address
			}
		}  // eof?
		
		// give additional information that file is loaded
		if (pdp8::DEBUG || !SILENT)
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
	if (pdp8::DEBUG || !SILENT)
	{
		cout << "\n***BEGIN PROGRAM EXECUTION***\n";
	}
	
	// begin program execution
	running = true;
	while (running)
	{
		// fetch data
		address = Memory.load(PDP8.getPC());
		if (address < 0)
		{   // error occurred
			cout << "ERROR - address " << PDP8.getPC() << " could not be found\n";
			goto EXECUTION_DONE;
		}

		/*NOTE: PC handled by BitTwiddle class*/
		// increment program counter
		//PDP8.increment_PC();

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
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[AND|" << indirect << "|" << currentPage
                     << "|" << offset << "]\n";
			}
			PDP8.PDP_AND(indirect, currentPage, offset);
			break;
		case 1:
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[TAD|" << indirect << "|" << currentPage
					<< "|" << offset << "]\n";
			}
			PDP8.PDP_TAD(indirect, currentPage, offset);
			break;
		case 2:
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[ISZ|" << indirect << "|" << currentPage
					<< "|" << offset << "]\n";
			}
			PDP8.PDP_ISZ(indirect, currentPage, offset);
			break;
		case 3:
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[DCA|" << indirect << "|" << currentPage
					<< "|" << offset << "]\n";
			}
			PDP8.PDP_DCA(indirect, currentPage, offset);
			break;
		case 4:
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[JMS|" << indirect << "|" << currentPage
					<< "|" << offset << "]\n";
			}
			PDP8.PDP_JMS(indirect, currentPage, offset);
			break;
		case 5:
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[JMP|" << indirect << "|" << currentPage
					<< "|" << offset << "]\n";
			}
			PDP8.PDP_JMP(indirect, currentPage, offset);
			break;
		case 6:
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[IO|" << indirect << "|" << currentPage
					<< "|" << offset << "]\n";
			}
			PDP8.increment_PC();
			cout << "IO not implemented\n";
			break;
		case 7:
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[MICRO|" << indirect << "|" << currentPage
					<< "|" << offset << "]\n";
			}
			// TODO: parse address to pass to micro instruction
			// TODO: add the new function to parser file
			//PDP8.PDP_uintructions();
			cout << "uINSTRUCTIONS not implemented\n";
			goto EXECUTION_DONE;  // TODO: remove this when function implemented
			break;
		default:
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[" << opcode << "|" << indirect << "|" 
					 << currentPage	<< "|" << offset << "]\n";
				cout << "OPCODE " << opcode << " is not handled\n";
			}
			break;
		}
	} // end of RUNNING loop

EXECUTION_DONE:
	// end of program execution
	if (pdp8::DEBUG || !SILENT)
	{
		cout << "\n***FINISHED EXECUTING PROGRAM***\n";
	}

	// print statistics
	PDP8.display();
	cout << "AC = " << PDP8.getAC() << endl;

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


