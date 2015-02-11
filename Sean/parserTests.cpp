/*
* AUTHORS: Sean Hendrickson, 
* FILE: ISASimulator.cpp
* LAST MODIFIED: 1/20/2015
* DESCRIPTION: Tests parsing octal input file for ISA simulator
*/
#include "parser.h"
using namespace std;

#define TEST_FILE "test2.txt" // used for debugging
#define OUTPUT_FILE "clean2.txt"  // clean version of text file 

int main(int argc, char *argv[])
{
	ifstream myFile;    // hold trace file
	ofstream outputFile;  // show cleaned file
	int opcode = 0;
	bool I = false;
	bool M = false;
	int offset = 0;
	int lineCount = 0;
	int address = 0;
	int id = 0;
	int ret = 0;
	int count = 0;
	
	// check for program error
	if (argc < 1)
	{
		std::cout << "ERROR - program execution\n";
	}

	myFile.open(TEST_FILE);
	outputFile.open(OUTPUT_FILE);
	
	// write to an output file to see the cleaned file
	while (!myFile.eof())
	{
		++count;
		cout << "Address before = " << address << endl;
		cout << "id before = " << id << endl;
		ret = getAddress(myFile, id, address, lineCount);
		cout << "Address after = " << address << endl;
		cout << "id after = " << id << endl;

		if (ret < 0)
		{   // handle error and exit
			cout << "ERROR - failed to get address on " << count << "th attempt\n";
			goto EXIT;
		}
		if (ret > 0)
		{   // handle error and exit
			cout << "ERROR found on line" << lineCount << endl;
			goto EXIT;
		}

		outputFile << id << "  " << address << endl;  // write address to file

		// break down the address
		ret = parseAddress(address, opcode, I, M, offset);
		if (ret < 0)
		{   // handle error and exit
			cout << "ERROR - failed to parse address on " << count << "th attempt\n";
			goto EXIT;
		}

		outputFile << "[ " << opcode << " | " << I << " | " << M << " | " << offset << " ]\n";
	}
	
EXIT:
	// close the file
	myFile.close();
	outputFile.close();

	exitMessage();
	return 0;
}	
