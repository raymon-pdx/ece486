/*
* AUTHORS: Sean Hendrickson,
* FILE: parser.cpp
* LAST MODIFIED: 2/09/2015
* DESCRIPTION: Functions to parse information from text file
*/

#include "parser.h"

/* FUNCTION DEFINITIONS */

// load the file into memory
// PRE-COND: file must already be opened
//           lineCount must be initialized to 0
// POST-COND: value of id and address returned 
//            through corresponding variables
// PARAM: file is a string of the file to be accessed
//        lineCount - current line of input file
// RET: -1 if file not open
//      0 if successfull
//      lineNumber of error if failed
int getAddress(std::ifstream &file, int &id, int &address, int &lineCount)
{
	// check that the file is open
	if (file.is_open())
	{
		std::string octalAddress;  // stores combined address
		std::string id1, id2;           // hold id of each line 
		std::string address1, address2; // hold adresses after ids
		bool addressComplete = false;  // flag if both addresses found
		char currentChar = ' ';    // current character from file
		int numCount = 0;   // track current line
		int addrCount = 0;  // track which address is being accessed

		// set lineCount to 1 if it is zero
		if (lineCount == 0)
		{
			lineCount = 1;
		}

		if (pdp8::DEBUG) std::cout << "file found\n";

		// loop until both addresses found
		while ((addressComplete == false) && (!file.eof()))
		{
			// step through each character in line
			while (((currentChar = file.get()) != '\n') && (!file.eof()))
			{
				if (pdp8::DEBUG) std::cout << "currentChar = " << currentChar << std::endl;

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
							if (pdp8::DEBUG)
							{
								std::cout << "line" << lineCount << ", non-octal number found\n";
								exitMessage();
							}
							return lineCount;
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
							if (pdp8::DEBUG)
							{
								std::cout << "line" << lineCount << ", non-octal number found\n";
								exitMessage();
							}
							return lineCount;
						}
					}
				}
				else
				{  // error so return
					if (pdp8::DEBUG)
					{
						std::cout << "Unknown error with address counter occured\n";
						exitMessage();
					}
					return 0;
				}

				if (pdp8::DEBUG) std::cout << "lineC = " << lineCount
					<< ", addrC = " << addrCount
					<< ", numC = " << numCount << std::endl;
				if (pdp8::DEBUG) std::cout << "id1 = " << id1
					<< ", id2 = " << id2
					<< ", addr1 = " << address1
					<< ", addr2 = " << address2 << std::endl;

				// check if too many numbers found
				if (numCount > pdp8::MAX_NUMBERS)
				{
					if (pdp8::DEBUG)
					{
						std::cout << "line" << lineCount << ", too many numbers found\n";
						exitMessage();
					}
					return lineCount;
				}
			}  // end of line loop

			// check if too few numbers found
			if ((numCount < pdp8::MAX_NUMBERS) && (numCount > 0))
			{
				if (pdp8::DEBUG)
				{
					std::cout << "line" << lineCount << ", too few numbers found\n";
					exitMessage();
				}
				return lineCount;
			}

			// check if no numbers found
			if (numCount <= 0)
			{
				if (pdp8::DEBUG) std::cout << "line" << lineCount << ", line has no numbers\n";
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
					octalAddress = address1 + address2;
				}
			}
			// reset number counter for next line
			numCount = 0;
		} // end of address finding loop

		// add a debugging note for end of file
		if (file.eof())
		{
			if (pdp8::DEBUG) std::cout << "\n************************\n"
				<< "* END OF FILE DETECTED *\n"
				<< "************************\n\n";
		}

		// exit program if end of file but both addresses not obtained
		if (file.eof() && (addressComplete == false))
		{
			if (pdp8::DEBUG) std::cout << "both parts of address not found\n";
			return lineCount;
		}
		else
		{  // both addresses found so return information

			// convert address from octal string to binary
			address = octalToInt(octalAddress);

			if (pdp8::DEBUG) std::cout << "***** ADDRESS BREAKDOWN *****\n";
			if (pdp8::DEBUG) std::cout << "octal address = " << octalAddress
				<< "\ninteger address = " << address << std::endl;

			// get id to return
			id = octalToInt(id2 + id1);

			return 0;
		}
	}  // else - file failed to open
	else
	{
		if (pdp8::DEBUG) std::cout << "file not open" << std::endl;
		return -1;
	}
}


// break down an octal address into opcode, I, M, and offset bits
// PRE-COND: opcode, I, M, and offset must be declared
// POST-COND: data passed back through parameters
// RET: 0 on success
//     -1 if an error occured
int parseAddress(int address, int &opcode, bool &I, bool &M, int &offset)
{
	// convert address from int to a string
	std::string binaryAddress = createBinaryString(address);

	if (pdp8::DEBUG) std::cout << "***** ADDRESS BREAKDOWN *****\n";
	if (pdp8::DEBUG) std::cout << "binary address = " << binaryAddress
		<< "\nbinary opcode = " << binaryAddress.substr(0, pdp8::NUM_OPCODE_BITS) << std::endl;

	// find opcode
	if ((opcode = findOpcode(binaryAddress)) < 0)
	{
		if (pdp8::DEBUG)
		{
			std::cout << "ERROR - opcode not found\n";
			exitMessage();
		}
		return -1;
	}

	if (pdp8::DEBUG) std::cout << "OP-CODE=" << opcode << std::endl;

	// find the value of I
	if (pdp8::DEBUG) std::cout << "I=" << binaryAddress[pdp8::NUM_OPCODE_BITS] << std::endl;
	if (binaryAddress[pdp8::NUM_OPCODE_BITS] == '1')
	{
		I = true;
	}
	else
	{
		I = false;
	}

	// find the value of M
	if (pdp8::DEBUG) std::cout << "M=" << binaryAddress[pdp8::NUM_OPCODE_BITS + 1] << std::endl;
	if (binaryAddress[pdp8::NUM_OPCODE_BITS + 1] == '1')
	{
		M = true;
	}
	else
	{
		M = false;
	}

	// get the offset and return as integer
	offset = getOffset(binaryAddress);
	if (pdp8::DEBUG) std::cout << "FINAL OFFSET = " << offset << std::endl;

	// display the final breakdown of the address
	if (pdp8::DEBUG)
	{
		std::cout << "ADDRESS [ OPCODE | I | M | OFFSET ]" << std::endl;
		std::cout << "ADDRESS [ " << opcode << " | " << I << " | "
			<< M << " | " << offset << " ]" << std::endl << std::endl;
	}

	return 0;  // exit normally
}


// break down octal address for use in microinstructions
// PRE-COND: buffers must be designated for groupBit, CLA, and offset before function call
// POST-COND: values pased back through function parameters
// RET: 0 on success
//     -1 if passed address tp large (assumed 12 bits) 
int microInstrParser(int address, int &groupBit, int &CLA, int &offset)
{
	int bit3Mask = 0x100;
	int bit4Mask = 0x080;
	int addressMask = 0x07F;
	int temp = 0;

	// check that passed address is not too large
	if (address > (pow(2, pdp8::REGISTERSIZE) - 1))
	{   // passed address too large
		return -1;
	}

	// get bit 3
	temp = address & bit3Mask;  // get only bit 8
	groupBit = temp >> 8;       // shift right 8 bits

	// get bit 4
	temp = address & bit4Mask;  // get only bit 7
	CLA = temp >> 7;            // shift right 7 bits

	// get offset
	offset = address & addressMask;  // get lowest 7 bits
	return 0;
}


// break down octal address for use in IO instructions
// PRE-COND: buffers must be designated for devNumber, and function before function call
// POST-COND: values pased back through function parameters
// RET: 0 on success
//     -1 if passed address tp large (assumed 12 bits) 
int IOInstrParser(int address, int &devNumber, int &function)
{
	int deviceMask = 0x1F8;
	int funcMask = 0x007;
	int temp = 0;

	// check that passed address is not too large
	if (address > (pow(2, pdp8::REGISTERSIZE) - 1))
	{   // passed address too large
		return -1;
	}

	// get device number
	temp = address & deviceMask;  // get bits 3:8
	devNumber = temp >> 3;        // shift right 3 bits

	// get function
	function = address & funcMask;    // get lowest 3 bits

	return 0;
}


// this function takes a string of octal values and
// converts it to a binary sting
// PRE-COND: characters must be ANSI
//           characters must be octal
// return: EMPTY if error occured
std::string octalToBinary(std::string octal)
{
	std::string binaryAddress = "";  // store string to be returned

	for (int i = 0; i < static_cast<int>(octal.length()); ++i)
	{
		// find binary value of octal characters
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


// this function takes a string of octal values and 
// converts it to an integer
// PRE-COND: characters must be ANSI
//           characters must be octal
// return: -1 if error occured
int octalToInt(std::string octal)
{
	std::string binaryAddress = "";  // store string to be returned

	for (int i = 0; i < static_cast<int>(octal.length()); ++i)
	{
		// find binary value of octal characters
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
			return -1;
		}
	}

	// convert binary string to an integer
	std::bitset<pdp8::REGISTERSIZE> aString(binaryAddress);
	return static_cast<int>(aString.to_ulong());
}


// this function generates an octal string from
// a passed integer
// PRE-COND: address size must be decided before
//           compile time
// param: value - binary value of string
// return: string of ADDRESS_SIZE/3 in length
//         empty string if error
std::string intToOctal(int value)
{
	std::string octalString = "";
	std::bitset<pdp8::REGISTERSIZE> aString(value);
	std::string binaryString = aString.to_string();

	// step through entire string to get octal value
	for (int i = 0; i < pdp8::REGISTERSIZE; i += 3)
	{
		// find matching string
		if (binaryString.compare(i, 3, "000") == 0)
		{
			octalString += "0";
		}
		else if (binaryString.compare(i, 3, "001") == 0)
		{
			octalString += "1";
		}
		else if (binaryString.compare(i, 3, "010") == 0)
		{
			octalString += "2";
		}
		else if (binaryString.compare(i, 3, "011") == 0)
		{
			octalString += "3";
		}
		else if (binaryString.compare(i, 3, "100") == 0)
		{
			octalString += "4";
		}
		else if (binaryString.compare(i, 3, "101") == 0)
		{
			octalString += "5";
		}
		else if (binaryString.compare(i, 3, "110") == 0)
		{
			octalString += "6";
		}
		else if (binaryString.compare(i, 3, "111") == 0)
		{
			octalString += "7";
		}
		else
		{   // some error occured
			octalString = "";
			return octalString;
		}
	}

	return octalString;
}


// this function generates a binary string based
// on the pre-determined address size 
// PRE-COND: size of string must be decided before
//           compile time
// param: value - binary value of string
// return: string of ADDRESS_SIZE bits long
std::string createBinaryString(int value)
{
	std::bitset<pdp8::REGISTERSIZE> aString(value);
	return aString.to_string();
}


// this function generates a binary string based
// on the number of opcode bits
// PRE-COND: can only be used to compare opcode
// param: value - binary value of string
// return: string of NUM_OPCODE_BITS bits long
std::string genOpcodeString(int value)
{
	std::bitset<pdp8::NUM_OPCODE_BITS> aString(value);
	return aString.to_string();
}

// returns the opcode from the address
// PRE-COND: opcode must be a binary string
// RET: int of opcode if found
//      -1 if error occured
int findOpcode(std::string address)
{
	for (int i = 0; i < pow(2, pdp8::NUM_OPCODE_BITS); ++i)
	{
		if (address.compare(0, pdp8::NUM_OPCODE_BITS, genOpcodeString(i)) == 0)
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
int getOffset(std::string binaryAddress)
{
	// assume size of address is ADDRESS_SIZE
	// store offset chunk of address in bitset
	std::bitset<pdp8::OFFSET_SIZE> bitString(
		 binaryAddress.substr(pdp8::NUM_OPCODE_BITS + 2,
		                      pdp8::NUM_OPCODE_BITS + 1 + pdp8::OFFSET_SIZE));
	return static_cast<int>(bitString.to_ulong());
}


void exitMessage()
{
	std::cout << "Press ENTER to end program" << std::endl;
	std::cin.get();
}


