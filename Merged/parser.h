/*
* AUTHORS: Sean Hendrickson,
* FILE: parser.h
* LAST MODIFIED: 2/09/2015
* DESCRIPTION: Functions to parse information from text file
*/
#pragma once
#include "constants.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <cctype>
#include <cmath>


/* FUNCTION PROTOTYPES */

// load the file into memory
// PRE-COND: file must already be opened
// POST-COND: value of id and address returned 
//            through corresponding variables
// PARAM: file is a string of the file to be accessed
// RET: 0 if successful
//      -1 if failed
int getAddress(std::ifstream &file, int &id, int &address, int &lineCount);


// break down an octal address into opcode, I, M, and offset bits
// PRE-COND: opcode, I, M, and offset must be declared
// POST-COND: data passed back through parameters
// RET: 0 on success
//     -1 if an error occured
int parseAddress(int address, int &opcode, bool &I, bool &M, int &offset);


// break down octal address for use in microinstructions
// PRE-COND: buffers must be designated for groupBit, CLA, and offset before function call
// POST-COND: values pased back through function parameters
// RET: 0 on success
//     -1 if passed address tp large (assumed 12 bits) 
int microInstrParser(int address, int &groupBit, int &CLA, int &offset);


// break down octal address for use in IO instructions
// PRE-COND: buffers must be designated for devNumber, and function before function call
// POST-COND: values pased back through function parameters
// RET: 0 on success
//     -1 if passed address tp large (assumed 12 bits) 
int IOInstrParser(int address, int &devNumber, int &function);


// this function takes a string of octal values and 
// converts it to a binary sting
// PRE-COND: characters must be ANSI
//           characters must be octal
// return: EMPTY if error occured
std::string octalToBinary(std::string octal);


// this function takes a string of octal values and 
// converts it to an integer
// PRE-COND: characters must be ANSI
//           characters must be octal
// return: -1 if error occured
int octalToInt(std::string octal);


// this function generates an octal string from
// a passed integer
// PRE-COND: address size must be decided before
//           compile time
// param: value - binary value of string
// return: string of ADDRESS_SIZE/3 in length
//         empty string if error
std::string intToOctal(int value);


// this function generates a binary string based
// on the number of opcode bits
// PRE-COND: can only be used to compare opcode
// param: value - binary value of string
// return: string of NUM_OPCODE_BITS bits long
std::string genOpcodeString(int value);


// this function generates a binary string based
// on the pre-determined address size 
// PRE-COND: size of string must be decided before
//           compile time
// param: value - binary value of string
// return: string of ADDRESS_SIZE bits long
std::string createBinaryString(int value);


// returns the opcode from the address
// PRE-COND: opcode must be a binary string
// RET: int of opcode if found
//      -1 if error occured
int findOpcode(std::string address);


// convert OFFSET_SIZE segment of binary string to an integer.
// note that the offset is the character at the end of the string
// PRE-COND: OFFSET_SIZE and ADDRESS_SIZE must already be set
// PARAM: binaryAddress - string of 1's and 0's with ADDRESS SIZE
//                        number of characters
// RET: offset as and integer
int getOffset(std::string binaryAddress);


// this function returns true if a character is 
// an octal value. Else false is returned
bool isOctal(char character);


// used to pause program to read debugging messages
void exitMessage();
