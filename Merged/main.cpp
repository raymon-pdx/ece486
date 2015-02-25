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
#include <iomanip>

//*********IMPLEMENTING TRACE FILE****************
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#define timeStamp /*OPTIONAL TIMESTAMP*/
//************************************************

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
	int groupBit = 0;           // use for parsing micro instr
	int CLA = 0;                // use for parsing micro instr
	int microOffset = 0;        // use for parsing micro instr
	int deviceNum = 0;          // used for parsing address for IO instr
	int function = 0;           // used for parsing address for IO instr
	int id = 0;                 // data or instruction word
	int ret = 0;
	std::ofstream memoryOutput;  // used to save memory
	
    //*********IMPLEMENTING TRACE FILE****************
    #ifndef timeStamp
    //call timestamp function
    time_t rawtime;
    time (&rawtime);
    #endif

    //create an output trace file
    std::ofstream outputTraceFile;
    outputTraceFile.open("TraceFile.txt");

    #ifndef timeStamp
    //add time stamp as header of tracefile
    outputTraceFile << "***** TIME STAMP *****\n";
    outputTraceFile << "Trace file generated: " << ctime(&rawtime);
    #endif
    //************************************************

	// create file to store memory in
	memoryOutput.open(MEMORY_FILE, fstream::out);

	// INITIALIZATION
	pagetable Memory(pdp8::NUM_PAGES, pdp8::PAGE_CAPACITY);  // initialize memory
	BitTwiddle PDP8(&Memory, &outputTraceFile);  // initialize ISA class

	// LOAD FILE INTO MEMORY
	
	// check for program error
	if (argc < 1)
	{
		cout << "ERROR - program execution\n";
	}

	// check for trace file name
	if (argc < 2)
	{
		cout << "Please pass object file as first argument\n";
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
				cout << "Address = " << oct << loadLocation 
					 << ", Data passed = " << oct << address << endl;
			}

			// display for address
			if ((pdp8::DEBUG || !SILENT) && !myFile.eof() && (id == 1))
			{
				cout << "\n***NEW ADDRESS***\n";
				cout << "Address = " << oct << loadLocation << endl;
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

					cout << "Data retrieved = " << oct << ret << endl;
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
			cout << "ERROR - address " << std::oct << PDP8.getPC() << " could not be found\n";
			goto EXECUTION_DONE;
		}

        //*********IMPLEMENTING TRACE FILE****************
        int type = 2; //2 - instruction fetch
        outputTraceFile << type << " " << std::oct << PDP8.getPC() << std::endl;
        //************************************************

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
				cout << "[AND  |" << indirect << "|" << currentPage
                     << "|" << setfill('0') << setw(4) 
					 << oct << offset << "]\n";
			}
			PDP8.PDP_AND(indirect, currentPage, offset);
			break;
		case 1:
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[TAD  |" << indirect << "|" << currentPage
					<< "|" << setfill('0') << setw(4)
					<< oct << offset << "]\n";
			}
			PDP8.PDP_TAD(indirect, currentPage, offset);
			break;
		case 2:
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[ISZ  |" << indirect << "|" << currentPage
					 << "|" << setfill('0') << setw(4)
					 << oct << offset << "]\n";
			}
			PDP8.PDP_ISZ(indirect, currentPage, offset);
			break;
		case 3:
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[DCA  |" << indirect << "|" << currentPage
					 << "|" << setfill('0') << setw(4)
					 << oct << offset << "]\n";
			}
			PDP8.PDP_DCA(indirect, currentPage, offset);
			break;
		case 4:
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[JMS  |" << indirect << "|" << currentPage
					 << "|" << setfill('0') << setw(4)
					 << oct << offset << "]\n";
			}
			PDP8.PDP_JMS(indirect, currentPage, offset);
			break;
		case 5:
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[JMP  |" << indirect << "|" << currentPage
					 << "|" << setfill('0') << setw(4) 
					 << oct << offset << "]\n";
			}
			PDP8.PDP_JMP(indirect, currentPage, offset);
			break;
		case 6:
			// parse address for IO instruction
			if (IOInstrParser(address, deviceNum, function) < 0)
			{
				cout << "ERROR - Passed address to IO instructions is too large\n";
				goto EXIT;
			}

			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[IO   |" 
					 << setfill('0') << setw(3) << oct << deviceNum << "|" 
					 << setfill('0') << setw(4) << oct << function << "]"
                     <<"//I/O is not implemented, NO-OP\n";
			}
			PDP8.PDP_IO(deviceNum, function);
			break;
		case 7:
			// parse address for micro instructions
			if (microInstrParser(address, groupBit, CLA, microOffset) < 0)
			{
				cout << "ERROR - Passed address to micro instructions is too large\n";
				goto EXIT;
			}

			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[MICRO|" << groupBit << "|" << CLA
					 << "|" << setfill('0') << setw(4) 
					 << oct << microOffset << "]\n";
			}
			
			if (PDP8.PDP_uintructions(groupBit, CLA, microOffset) == -1)
			{   // halt found so stop running
				running = false;
			}
			break;
		default:
			if (pdp8::DEBUG || !SILENT)
			{
				cout << "[" << opcode << "|" << indirect << "|" 
					 << currentPage	<< "|" << oct << offset << "]\n";
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
	cout << "AC = " << setfill('0') << setw(4) 
		 << oct << PDP8.getAC() << endl;

EXIT:

    //*********IMPLEMENTING TRACE FILE****************
    outputTraceFile.close();
    std::cout << "Trace file generated.\n";
    //************************************************


	// output memory file
	if (memoryOutput.is_open())
	{
		// output stream re-direction
		std::streambuf *oldbuf = std::cout.rdbuf(); //save 
		std::cout.rdbuf(memoryOutput.rdbuf());

		Memory.display_all(); // Contents to cout will be written to text.txt

		//reset back to standard input
		std::cout.rdbuf(oldbuf);
	}
	else
	{
		if (pdp8::DEBUG || !SILENT)
		{
			cout << "ERROR - memory file not found\n";
		}
	}

	// close the file
	myFile.close();
	memoryOutput.close();
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


