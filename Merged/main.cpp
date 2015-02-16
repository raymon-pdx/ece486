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
#define OUTPUT_FILE "clean2.txt"  // clean version of text file 
#define NUM_PAGES 32  // number of pages in memory
#define PAGE_CAPACITY 128 // number of addresses per page
#define STARTING_ADDRESS 128 // where program is stored


// FUNCTION PROTOTYPES
void testIntToOctal();
void testInstr(BitTwiddle &bits);

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
	
	// test intToOctal()
	testIntToOctal();
	testInstr(bits);

	
	// INITIALIZATION
	myFile.open(TEST_FILE);
	outputFile.open(OUTPUT_FILE);
	pagetable Memory(NUM_PAGES, PAGE_CAPACITY);  // initialize memory
	
	// write to an output file to see the cleaned file
	while (!myFile.eof())
	{
		++count;

		cout << "DATA " << count << endl;
		cout << "Address before = " << address << endl;
		cout << "id before = " << id << endl;
		ret = getAddress(myFile, id, address, lineCount);
		cout << "Address after = " << address << endl;
		cout << "id after = " << id << endl;

		if (ret < 0)
		{   // handle error and exit
			cout << "ERROR - failed to get address on attempt " << count << "\n";
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

		cout << "Address = " << STARTING_ADDRESS + loadOffset << ", Data passed = " << address << endl;

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
		cout << "Data retrieved = " << ret << endl << endl;

		++loadOffset;
	}
	
EXIT:
	// close the file
	myFile.close();
	outputFile.close();

	exitMessage();
	return 0;
}	


// FUNCTION DECLARATIONS

// this function assumes ADDRESS_LENGTH = 12
void testIntToOctal()
{
	int number = 0;
	string octalString = "";

	// create test values and check results
	number = 1350;  // octal: 2506
	octalString = intToOctal(number);
	if (octalString.compare("2506") != 0)
	{
		cout << "ERROR - did not convert int to octal\n";
	}
	else
	{
		cout << number << " in octal is " << octalString << endl;
	}

	// test another value
	// create test values and check results
	number = 0;  // octal: 0000
	octalString = intToOctal(number);
	if (octalString.compare("0000") != 0)
	{
		cout << "ERROR - did not convert int to octal\n";
	}
	else
	{
		cout << number << " in octal is " << octalString << endl;
	}

	// test another value
	// create test values and check results
	number = 4095;  // octal: 7777
	octalString = intToOctal(number);
	if (octalString.compare("7777") != 0)
	{
		cout << "ERROR - did not convert int to octal\n";
	}
	else
	{
		cout << number << " in octal is " << octalString << endl;
	}
	
}


void testInstr(BitTwiddle &bits)
{
    cout<<"\nTesting the first 6 instuctions:\n\n";


    cout<<"C(EAddr)=3671o AC=3701o link=0b  //1977+1985\n";
    cout<<"AC Expected: 7572o link: 0b      //-134\n";
    bits.temp=1977;
    bits.AC=1985;
    bits.link=0;
    bits.PDP_TAD(1,0,123);
    cout<<"AC result:   "<<bits.AC<<" link: "<<bits.link<<"\n\n";

    cout<<"C(EAddr)=4107o AC=4077o link=0b  //-1977+-1985\n";
    cout<<"AC Expected: 206o link: 1b      //134\n";
    bits.temp=2119;
    bits.AC=2111;
    bits.link=0;
    bits.PDP_TAD(1,0,123);
    cout<<"AC result:   "<<bits.AC<<" link: "<<bits.link<<"\n\n";	
}

