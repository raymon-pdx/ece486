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
#include <cctype>
#include <cmath>
using namespace std;

#define DEBUG 1   // 1 for on, 0 for off
#define TEST_FILE "test3.txt" // used for debugging
#define DELIMITER '\n' // character used as the delimeter
#define BASE 10   // base used for stoi()
#define ADDRESS_SIZE 12  // number of bits in address
#define OFFSET_SIZE 7  // size of offset used for bitset
#define MAX_NUMBERS 3  // number of numbers to find per line
#define NUM_OPCODE_BITS 3 // number of bits used for opcode

/* FUNCTION PROTOTYPES */
string octalToBinary(string octal);
string genOpcodeString(int value);
string createBinaryString(int value);
int findOpcode(string address);
int getOffset(string binaryAddress);
bool isOctal(char character);
void exitMessage();


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

	// use test file
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
		
		bool addressComplete = false;  // flag if both addresses found
		char currentChar = ' ';    // current character from file
		int numCount = 0;   // track current line
		int addrCount = 0;  // track which address is being accessed
		int lineCount = 1;  // keep which half of address is being accessed

		if (DEBUG) std::cout << "file found\n";
		// loop until end of file 
		while (!myFile.eof())
		{
			// prepare for next address
			string id1, id2;           // hold id of each line 
			string address1, address2; // hold adresses after ids
			string address;  // store completed address 
			addrCount = 0;
			numCount = 0;
			addressComplete = false;
			char nextChar;  // check if the first character is \n
			
			// loop until both addresses found
			while ((addressComplete == false) && (!myFile.eof()))
			{
				// step through each character in line
				while (((currentChar = myFile.get()) != '\n') && (!myFile.eof()))
				{
					if (DEBUG) cout << "currentChar = " << currentChar << endl;
					 
					// find if current char is a number
					if (addrCount == 0)
					{  // first half of address
						if (isdigit(currentChar))
						{
							// check if an octal digit
							if (isOctal(currentChar))
							{
								// save character to string
								if ((numCount < 1))
								{
									id1 += currentChar;
									++numCount;
								}
								else if (numCount >= 1)
								{
									address1 += currentChar;
									++numCount;
								}
							}
							else  // number is not octal
							{
								cout << "line" << lineCount << ", non-octal number found\n";
								exitMessage();
								return 0;
							}
						}
					}
					else if (addrCount == 1)
					{  // second half of address
						if (isdigit(currentChar))
						{
							// check if an octal digit
							if (isOctal(currentChar))
							{
								// save character to string
								if ((numCount < 1))
								{
									id2 += currentChar;
									++numCount;
								}
								else if (numCount >= 1)
								{
									address2 += currentChar;
									++numCount;
								}
							}
							else  // number is not octal
							{
								cout << "line" << lineCount << ", non-octal number found\n";
								exitMessage();
								return 0;
							}
						}
					}
					else
					{  // error so return
						if(DEBUG) cout << "Unknown error with address counter occured\n";
						exitMessage();
						return 0;
					}

					if (DEBUG) cout << "lineC = " << lineCount
						<< ", addrC = " << addrCount
						<< ", numC = " << numCount << endl;
					if (DEBUG) cout << "id1 = " << id1
						<< ", id2 = " << id2
						<< ", addr1 = " << address1
						<< ", addr2 = " << address2 << endl;

					// check if too many numbers found
					if (numCount > MAX_NUMBERS)
					{
						cout << "line" << lineCount << ", too many numbers found\n";
						exitMessage();
						return 0;
					}
				}  // end of line loop

				// check if too few numbers found
				if ((numCount < MAX_NUMBERS) && (numCount > 0))
				{
					cout << "line" << lineCount << ", too few numbers found\n";
					exitMessage();
					return 0;
				}
				
				// check if no numbers found
				if (numCount <= 0)
				{   
					if (DEBUG) cout << "line" << lineCount << ", line has no numbers\n";
					++lineCount;
				}
				else
				{  // set address to prepare for next address
					if (addrCount == 0)
					{
						addrCount = 1;
						++lineCount;
					}
					else
					{
						addrCount = 0;
						++lineCount;
						addressComplete = true;
						address = address1 + address2;
					}
				}
				// reset number counter for next line
				numCount = 0;
			} // end of address finding loop

			// add a debugging note for end of file
			if (myFile.eof())
			{
				if (DEBUG) cout << "\n************************\n"
								<< "* END OF FILE DETECTED *\n"
								<< "************************\n\n";
			}

			// exit program if end of file but both addresses not obtained
			if (myFile.eof() && (addressComplete == false))
			{
				if (DEBUG) cout << "ending program since address not complete\n";
			}
			else
			{

				// convert address from octal string to binary
				string binaryAddress = octalToBinary(address);

				if (DEBUG) cout << "***** ADDRESS BREAKDOWN *****\n";
				if (DEBUG) cout << "binary address = " << binaryAddress
					<< "\nbinary opcode = " << binaryAddress.substr(0, NUM_OPCODE_BITS) << endl;

				// find opcode
				if ((opcode = findOpcode(binaryAddress)) < 0)
				{
					cout << "ERROR - opcode not found\n";
					exitMessage();
					return 0;
				}

				if (DEBUG) std::cout << "OP-CODE=" << opcode << endl;

				// find the value of I
				if (DEBUG) std::cout << "I=" << binaryAddress[NUM_OPCODE_BITS] << endl;
				if (binaryAddress[NUM_OPCODE_BITS] == '1')
				{
					I = true;
				}
				else
				{
					I = false;
				}

				// find the value of M
				if (DEBUG) std::cout << "M=" << binaryAddress[NUM_OPCODE_BITS + 1] << endl;
				if (binaryAddress[NUM_OPCODE_BITS + 1] == '1')
				{
					M = true;
				}
				else
				{
					M = false;
				}

				// get the offset and return as integer
				offset = getOffset(binaryAddress);
				if (DEBUG) cout << "FINAL OFFSET = " << offset << endl;

				// display the final breakdown of the address
				if (DEBUG)
				{
					cout << "ADDRESS [ OPCODE | I | M | OFFSET ]" << endl;
					cout << "ADDRESS [ " << opcode << " | " << I << " | "
						<< M << " | " << offset << " ]" << endl << endl;
				}
			}
		}  // loop if not end of file
	}  // else - file failed to open
	else
	{
		std::cout << "ERROR - File not found\n";
	}
	
	// close the file
	myFile.close();

	exitMessage();
	return 0;
}	



/* FUNCTION DEFINITIONS */
// this function takes a string of octal values and 
// converts the string to binary
// PRE-COND: characters must be ANSI
//           characters must be octal
// return: EMPTY if error occured
string octalToBinary(string octal)
{
	string binaryAddress = "";  // store string to be returned

	for (int i = 0; i < static_cast<int>(octal.length()); ++i)
	{
		// find binary value of octal character
		switch (octal[i])
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
			binaryAddress = "";
			break;
		}
		// check that error did not occure
		if (binaryAddress.empty())
		{ // exit the loop
			i = static_cast<int>(octal.length() - 1);
		}
	}
	return binaryAddress;
}

// this function generates a binary string based
// on the pre-determined address size 
// PRE-COND: size of string must be decided before
//           compile time
// param: value - binary value of string
// return: string of ADDRESS_SIZE bits long
string createBinaryString(int value)
{
	bitset<ADDRESS_SIZE> aString(value);
	return aString.to_string();
}

// this function generates a binary string based
// on the number of opcode bits
// PRE-COND: can only be used to compare opcode
// param: value - binary value of string
// return: string of NUM_OPCODE_BITS bits long
string genOpcodeString(int value)
{
	bitset<NUM_OPCODE_BITS> aString(value);
	return aString.to_string();
}

// returns the opcode from the address
// PRE-COND: opcode must be a binary string
// RET: int of opcode if found
//      -1 if error occured
int findOpcode(string address)
{
	for (int i = 0; i < pow(2, NUM_OPCODE_BITS); ++i)
	{
		if (address.compare(0, NUM_OPCODE_BITS, genOpcodeString(i)) == 0)
		{ // matching opcode found
			return i;
		}
	}
	// no match found so return error
	return -1;
}

// this function returns true if a character is 
// an octal value. Else false is returned
bool isOctal(char character)
{
	if ((character >= '0') && (character <= '7'))
	{
		return true;
	}
	else
	{
		return false;
	}
}


// convert OFFSET_SIZE segment of binary string to an integer.
// note that the offset is the character at the end of the string
// PRE-COND: OFFSET_SIZE and ADDRESS_SIZE must already be set
// PARAM: binaryAddress - string of 1's and 0's with ADDRESS SIZE
//                        number of characters
// RET: offset as and integer
int getOffset(string binaryAddress)
{
	// assume size of address is ADDRESS_SIZE
	// store offset chunk of address in bitset
	bitset<OFFSET_SIZE> bitString(binaryAddress.substr(NUM_OPCODE_BITS + 2, NUM_OPCODE_BITS + 1 + OFFSET_SIZE));
	return static_cast<int>(bitString.to_ulong());
}


void exitMessage()
{
	std::cout << "Press ENTER to end program" << endl;
	std::cin.get();
}
