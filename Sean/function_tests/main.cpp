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
#define TEST_FILE "test2.txt" // used for debugging
#define DELIMITER '\n' // character used as the delimeter
#define BASE 10   // base used for stoi()
#define ADDRESS_SIZE 12  // size of address
#define OFFSET_SIZE 7  // size of offset used for bitset
#define MAX_NUMBERS 5  // number is single line from input file
#define NUM_OPCODE_BITS 3 // number of bits used for opcode
#define BINARY_STRING_LENGTH 12 // number of bits created with binary string


/* FUNCTION PROTOTYPES */
string octalToBinary(string octal);
string genOpcodeString(int value);
string createBinaryString(int value);
int findOpcode(string address);
bool isOctal(char character);
int getOffset(string binaryAddress);
void exitMessage();

void test1();
void test2();
void test3();
void test4();
void test5();


int main(int argc, char *argv[])
{
	// test octalToBinary()
	test1();

  // test genOpcodeString()
	test2();

	// test findOpcode()
	test3();

  // test isOctal()
	test4();

	// test getOffset()
	test5();	

  // test exitMessage()
	exitMessage();
	return 0;
}	

/*TEST FUNCTIONS*/
void test1()
{
	string octalString1 = "1234567";  // all octal values
	string octalString2 = "1239765";  // not all octal
	string binaryAddress;

	cout << "octalString1 = " << octalString1 << endl;
	if((binaryAddress = octalToBinary(octalString1)).empty())
	{
		cout << "ERROR - non octal number in string\n";
	}
	else
	{
		cout << "output for string1 = " << binaryAddress << endl;
	}

	cout << "octalString2 = " << octalString2 << endl;
 	if((binaryAddress = octalToBinary(octalString2)).empty())
	{
		cout << "ERROR - non octal number in string\n";
	}
	else
	{
		cout << "output for string2 = " << binaryAddress << endl;
	} 
}

// test creating a binary string
void test2()
{
	string output;  // return string
	for(int i = 0; i < pow(2, NUM_OPCODE_BITS); ++i)
	{
		output = genOpcodeString(i);
		cout << i << " in binary with " << NUM_OPCODE_BITS
				 << " bits = " << output << endl;
	}
}


void test3()
{  
	int result = 0;
	
  // create binary string to test opcodes 
	string address[]={ createBinaryString(0),    // opcode 0
										 createBinaryString(512),  // opcode 1
										 createBinaryString(1024), // opcode 2
										 createBinaryString(1536), // opcode 3
										 createBinaryString(2048), // opcode 4
										 createBinaryString(2560), // opcode 5
										 createBinaryString(3072), // opcode 6
										 createBinaryString(3584), // opcode 7
										 createBinaryString(256),  // opcode 0
										 createBinaryString(768),  // opcode 1
										 createBinaryString(4095)  // opcode 7
									 }; 

	// Create loop to check all opcodes and a number that
  // is not an opcode to check for errors 
	for(int i = 0; i < pow(2,NUM_OPCODE_BITS) + 3; ++i)
	{
		result = findOpcode(address[i]);
		if(result < 0)
		{
			cout << "ERROR - " << address[i] << " had invalid opcode\n";
		}
		else
		{
			cout << "string " << address[i] << " OPCODE = " << result << endl;
		}
	}
}


void test4()
{ // not implemented
	char firstNum = '0';
	for(int i = 0; i < 20; ++i)
	{
		if(isOctal(firstNum + i))
		{
			cout << static_cast<char>(firstNum + i) << " is OCTAL\n";
		}
		else
		{
			cout << static_cast<char>(firstNum + i) << " NOT OCTAL\n";	
		}
	}
}


void test5()
{
	int output = 0;
  // generate a 12 bit binary string with with offset 32
	string address1 = createBinaryString(32);    // offset 32
	string address2 = createBinaryString(3136);  // offset 64
	string address3 = createBinaryString(1420);  // offset 12
	string address4 = createBinaryString(1090); 	// offset 66 
	
	output = getOffset(address1);
	cout << "address1 = " << address1 << ", offset = " << output << endl;
	output = getOffset(address2);
	cout << "address2 = " << address2 << ", offset = " << output << endl;
	output = getOffset(address3);
	cout << "address3 = " << address3 << ", offset = " << output << endl;
	output = getOffset(address4);
	cout << "address4 = " << address4 << ", offset = " << output << endl;
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
		if(binaryAddress.empty())
		{ // exit the loop
			i = static_cast<int>(octal.length() - 1);
		}
	}
	return binaryAddress;
}

// this function generates a binary string based
// on the number of opcode bits
// PRE-COND: can only be used to compare opcode
// param: value - binary value of string
// return: string of NUM_OPCODE_BITS bits long
string genOpcodeString(int value)
{
	bitset<NUM_OPCODE_BITS> aString (value);
	return aString.to_string();
}

// this function generates a binary string based
// on a predetermined binary string length
// PRE-COND: size of string must be decided before
//           compile time
// param: value - binary value of string
// return: string of BINARY_STRING_LENGTH bits long
string createBinaryString(int value)
{
	bitset<BINARY_STRING_LENGTH> aString (value);
	return aString.to_string();	
}

// returns the opcode from the address
// PRE-COND: opcode must be a binary string
// RET: int of opcode if found
//      -1 if error occured
int findOpcode(string address)
{
	for(int i = 0; i < pow(2, NUM_OPCODE_BITS); ++i)
	{
		if (address.compare(0, NUM_OPCODE_BITS, genOpcodeString(i)) == 0)
		{ // matching opcode found
			return i;
		}
	}
	// no match found so return error
	return -1;
}

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

// convert OFFSET_SIZE segment of binary string to an integer
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
