/*
* AUTHORS: Sean Hendrickson, 
* FILE: ISASimulator.cpp
* LAST MODIFIED: 1/20/2015
* DESCRIPTION: Tests parsing octal input file for ISA simulator
*/
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
using namespace std;

#define DEBUG 1   // 1 for on, 0 for off
#define TEST_FILE "test.txt" // used for debugging
#define DELIMITER '\n' // character used as the delimeter
#define BASE 10   // base used for stoi()
#define ADDRESS_SIZE 12  // size of address
#define OFFSET_SIZE 7  // size of offset used for bitset


int main(int argc, char *argv[])
{
	ifstream myFile;    // hold trace file
	int opcode = 0;
	bool I = false;
	bool M = false;
	int offset = 0;
	
	// check for program error
	if (argc < 1)
	{
		std::cout << "ERROR - program execution\n";
	}

	// use test trace file
	if(DEBUG)
	{
		myFile.open(TEST_FILE);
	}
	// check for trace file name
	else if (argc < 2)
	{
		std::cout << "Please pass object file as first argument\n";
	}
	else
	{
		// open pass file
		myFile.open(argv[1]);
	}

	// get contents of the file
	// THIS IS TEST CODE
	if(myFile.is_open())
	{
		int count = 0;

		if (DEBUG) std::cout << "file found\n";
		// display the contents of the file
		while(!myFile.eof())
		{
			string line1, line2;
			string binaryAddress;

			// get two lines
			getline(myFile, line1);
			getline(myFile, line2);

			if (DEBUG) std::cout << "*** PASS " << count + 1 << " ***" << endl;

			// parse the lines to get operation and address
			string operation = line1.substr(0, 1);
			if (DEBUG) std::cout << "OP=" << operation;

			string addrFirstHalf = line1.substr(1,2);
			if (DEBUG) std::cout << ", AD1=" << addrFirstHalf;

			string addrSecondHalf = line2.substr(1,2);
			if (DEBUG) std::cout << ", AD2=" << addrSecondHalf;

			string address = addrFirstHalf + addrSecondHalf;
			if (DEBUG) std::cout << ", ADDRESS=" << address << endl;

			// convert address from octal string to binary
			for (int i = 0; i < 4; ++i)
			{
				char value = address[i];
				switch (value)
				{
				case '0':
					binaryAddress += "000";
					break;
				case '1':
					binaryAddress += "001";
					break;
				case '2':
					binaryAddress += "010";
					break;
				case '3':
					binaryAddress += "011";
					break;
				case '4':
					binaryAddress += "100";
					break;
				case '5':
					binaryAddress += "101";
					break;
				case '6':
					binaryAddress += "110";
					break;
				case '7':
					binaryAddress += "111";
					break;
				default:
					cout << "ERROR - value in address not octal" << endl;
					break;
				}
			}

			if (DEBUG) std::cout << "BINARY ADDRESS = " << binaryAddress << endl;

			// break the binary string into the opcode, I, M, and offset
			// first find opcode
			if (DEBUG) std::cout << "OP-CODE=" << binaryAddress.substr(0, 3) << endl;
			
			if (binaryAddress.compare(0, 3, "000") == 0)
			{
				opcode = 0;
			}
			else if (binaryAddress.compare(0, 3, "001") == 0)
			{
				opcode = 1;
			}
			else if (binaryAddress.compare(0, 3, "010") == 0)
			{
				opcode = 2;
			}
			else if (binaryAddress.compare(0, 3, "011") == 0)
			{
				opcode = 3;
			}
			else if (binaryAddress.compare(0, 3, "100") == 0)
			{
				opcode = 4;
			}
			else if (binaryAddress.compare(0, 3, "101") == 0)
			{
				opcode = 5;
			}
			else if (binaryAddress.compare(0, 3, "110") == 0)
			{
				opcode = 6;
			}
			else if (binaryAddress.compare(0, 3, "111") == 0)
			{
				opcode = 7;
			}
			else
			{
				cout << "ERROR - could not find op-code" << endl;
			}
			if (DEBUG) std::cout << "OP-CODE=" << opcode << endl;

			// find the value of I
			if (DEBUG) std::cout << "I=" << binaryAddress[3] << endl;
			if (binaryAddress[3] == '1')
			{
				I = true;
			}
			else
			{
				I = false;
			}

			// find the value of M
			if (DEBUG) std::cout << "M=" << binaryAddress[3] << endl;
			if (binaryAddress[4] == '1')
			{
				M = true;
			}
			else
			{
				M = false;
			}
			

			// store offset in bitset to convert to integer
			bitset<OFFSET_SIZE> bitString (binaryAddress.substr(5, 11));
			if(DEBUG) cout << "OFFSET=" << bitString.to_string() << endl;
			if (DEBUG) cout << "OFFSET=" << bitString.to_ulong() << endl;

			offset = static_cast<int>(bitString.to_ulong());
			if (DEBUG) cout << "FINAL OFFSET = " << offset << endl;
			++count;

			// display the final breakdown of the address
			if (DEBUG)
			{
				cout << "ADDRESS [ OPCODE | I | M | OFFSET ]" << endl;
				cout << "ADDRESS [ " << opcode << " | " << I << " | " 
					 << M << " | " << offset << " ]" << endl << endl;
			}
		}
	}
	else
	{
		std::cout << "ERROR - File not found\n";
	}
	
	// close the file
	myFile.close();

	std::cout << "Press ENTER to end program" << endl;
	std::cin.get();
	return 0;
}	
