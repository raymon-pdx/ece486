Author: Sean Hendrickson

-----------------------------------------------------------
Project Coding: 
-----------------------------------------------------------
COMPLETE:
Parse OPCODE, I, M, & OFFSET out of input file
Test for too few and too many numbers
Test for octal numbers
Handle random characters, (\t,\n, space, A)
Fixed bug with endless loop if empty line at end of input file
Detect odd number of valid address lines
parseAddress placed in function
get 1 address from text file 


IN PROGRESS:
Ussing parser functions to place input in memory


TO DO:
Fix documentation in parser.h and parser.cpp


DEPENDENCY LIST: parser.h
1. getAddress()
     isOctal()
     exitMessage()
     octalToInt() 
2. parseAddress()
     createBinaryString()
	 findOpcode()
	   genOpcodeString()
	 getOffset()
3. findOpcode()
     genOpcodeString()


CODE AND TESTS:
-test1.txt: contains ideal data
-test2.txt: contains jumbled up data



