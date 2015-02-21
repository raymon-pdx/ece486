
/*
* AUTHORS: Ommaimah Hussein Mohammed,
*          Even Sprecher,
*          Meng Lei,
*          Sean Hendrickson
* FILE: main.cpp
* LAST MODIFIED: 2/16/2015
* DESCRIPTION: a program that simulates the PDP8 instruction set
*/

#include <iostream>
#include <cstring>

using namespace std;

#define TEST_FILE "test.txt" // used for debugging
#define NUM_PAGES 32  // number of pages in memory
#define PAGE_CAPACITY 128 // number of addresses per page
#define STARTING_ADDRESS 128 // where program is stored
bool SILENT=true;

void BitTwiddle(char * input_file);

int main(int argc, char *argv[])
{

	//ifstream myFile;    // hold trace file
	//bool SILENT = true; // silent is set
	//int loadOffset = 0;  // offset used to load data from text file
	//int lineCount = 0;
	//int address = 0;
	//int id = 0;
	//int ret = 0;
	
	// INITIALIZATION
	//pagetable Memory(NUM_PAGES, PAGE_CAPACITY);  // initialize memory
	
	// check for program error
	if (argc < 1)
	{
		std::cout << "ERROR - program execution\n";
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

	// check for trace file name
	if (argc < 2)
	{
		std::cout << "Please pass object file as first argument\n";
	}
	else
	{
    //pass file to bits
	BitTwiddle(argv[1]);    // class used for operation
	}	
    //TODO: we really shouldn't be here, cout a warning
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


