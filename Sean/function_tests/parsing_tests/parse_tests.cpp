// tests parsing functions for ece486 project
#include <iostream>
#include <cmath>

#define REGISTERSIZE 12

using std::cout;

/*FUNCTION PROTOTYPES*/
int microInstr_parser(int address, int &groupBit, int &CLA, int &offset);
int IO_parser(int address, int &devNumber, int &func);

void testMicroParser();
void testIOParser();


int main()
{
	testMicroParser();
	testIOParser();
    return 0;
}


/*FUNCTION DEFINITIONS*/
int microInstr_parser(int address, int &groupBit, int &CLA, int &offset)
{
	int bit3Mask = 0x100;
	int bit4Mask = 0x080;
	int addressMask = 0x07F;
	int temp = 0;  

	// check that passed address is not too large
	if(address > (pow(2, REGISTERSIZE) - 1))
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


int IO_parser(int address, int &devNumber, int &func)
{
	int deviceMask = 0x1F8;
	int funcMask = 0x007;
	int temp = 0;

	// check that passed address is not too large
	if(address > (pow(2, REGISTERSIZE) - 1))
	{   // passed address too large
		return -1;
	}

	// get device number
	temp = address & deviceMask;  // get bits 3:8
	devNumber = temp >> 3;        // shift right 3 bits

	// get function
	func = address & funcMask;    // get lowest 3 bits

	return 0;
}


void testMicroParser()
{
	// opcode is always 7
	int test1 = 0x001;  //GB = 0, CLA = 0, offset = 1
	int test2 = 0x082;  //GB = 0, CLA = 1, offset = 2
	int test3 = 0x104;  //GB = 1, CLA = 0, offset = 4
	int test4 = 0x194;  //GB = 1, CLA = 1, offset = 20
	int groupBit = 0;
	int CLA = 0;
	int offset = 0;
	//microInstr_parser(int address, int &groupBit, int &CLA, int &offset)
	// now test results
	cout << "\n***BEGINNING MICRO TEST***\n";

	// test 1
	if(microInstr_parser(test1, groupBit, CLA, offset) < 0)
	{
		std::cout << "ERROR - test1 out of bounds\n";
	}
	else
	{
		if((groupBit != 0) || (CLA != 0) || (offset != 1))
		{
			std::cout << "ERROR - failed Micro Test 1\n";
		}
	}

	// test 2
	if(microInstr_parser(test2, groupBit, CLA, offset) < 0)
	{
		std::cout << "ERROR - test2 out of bounds\n";
	}
	else
	{
		if((groupBit != 0) || (CLA != 1) || (offset != 2))
		{
			std::cout << "ERROR - failed Micro Test 2\n";
		}
	}

	// test 3
	if(microInstr_parser(test3, groupBit, CLA, offset) < 0)
	{
		std::cout << "ERROR - test3 out of bounds\n";
	}
	else
	{
		if((groupBit != 1) || (CLA != 0) || (offset != 4))
		{
			std::cout << "ERROR - failed Micro Test 3\n";
		}
	}

	// test 4
	if(microInstr_parser(test4, groupBit, CLA, offset) < 0)
	{
		std::cout << "ERROR - test4 out of bounds\n";
	}
	else
	{
		if((groupBit != 1) || (CLA != 1) || (offset != 20))
		{
			std::cout << "ERROR - failed Micro Test 4\n";
		}
	}
	cout << "\n***FINISHED MICRO TEST***\n";
}


void testIOParser()
{
	// opcode is always 6
	int test1 = 0x059;  //dev = 11,  func = 1
	int test2 = 0x062;  //dev = 12,  func = 2
	int test3 = 0x0FC;  //dev = 31, func = 4
	int test4 = 0x1F5;  //dev = 62, func = 5
	int dev = 0;
	int funct = 0;
	// int IO_parser(int address, int &devNumber, int &func)
	// now test results
	cout << "\n***BEGINNING IO TEST***\n";

	// test 1
	if(IO_parser(test1, dev, funct) < 0)
	{
		std::cout << "ERROR - test1 out of bounds\n";
	}
	else
	{
		if((dev != 11) || (funct != 1))
		{
			std::cout << "ERROR - failed IO Test 1\n";
		}
	}

	// test 2
	if(IO_parser(test2, dev, funct) < 0)
	{
		std::cout << "ERROR - test2 out of bounds\n";
	}
	else
	{
		if((dev != 12) || (funct != 2))
		{
			std::cout << "ERROR - failed IO Test 2\n";
		}
	}

	// test 3
	if(IO_parser(test3, dev, funct) < 0)
	{
		std::cout << "ERROR - test3 out of bounds\n";
	}
	else
	{
		if((dev != 31) || (funct != 4))
		{
			std::cout << "ERROR - failed IO Test 3\n";
		}
	}

	// test 4
	if(IO_parser(test4, dev, funct) < 0)
	{
		std::cout << "ERROR - test4 out of bounds\n";
	}
	else
	{
		if((dev != 62) || (funct != 5))
		{
			std::cout << "ERROR - failed IO Test 4\n";
		}
	}
	cout << "\n***FINISHED IO TEST***\n";
}

