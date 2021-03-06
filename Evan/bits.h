//--------------------------------------------------------------------------------------
// Class to hold functions for implementing the PDP-8 Instructions
//--------------------------------------------------------------------------------------
#pragma once
#include "constants.h"
#include "memory.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <bitset>


class BitTwiddle{

public:

	//default constructor
	BitTwiddle();

	// constructor
	BitTwiddle(pagetable *passedTable, std::ofstream *output);

	//destructor
	~BitTwiddle();

    //function for (OPCODE = 0) logical AND
    void PDP_AND(bool addr_bit,bool mem_page,int offset);
    
    //function for (OPCODE = 1) twos complement add
    void PDP_TAD(bool addr_bit,bool mem_page,int offset);

    //function for (OPCODE = 2) increment and skip on zero
    void PDP_ISZ(bool addr_bit,bool mem_page,int offset);

    //function for (OPCODE = 3) deposit and clear accumulator
    void PDP_DCA(bool addr_bit,bool mem_page,int offset);

    //function for (OPCODE = 4) jump to subroutine
    void PDP_JMS(bool addr_bit,bool mem_page,int offset);

    //function for (OPCODE = 5) jump
    void PDP_JMP(bool addr_bit,bool mem_page,int offset);
	
	//function for (OPCODE = 6) input/output
    void PDP_IO(int device_num,int opcode);
  
	//function for (OPCODE = 7) uinstructions
    int PDP_uintructions(bool group, bool cla, int offset);

	// return value of program counter
	int getPC()	{ return PC; }

	// return value of accumulator
	int getAC() { return AC; }
	
	// display data from BitTwiddle class
	void display();

private:
    int AC;          //accumulator
    int PC;          //Program Counter
    bool link;       //link bit
    int SR;          //console switch register

    int sumInstr;  // count for number of total instructions
    int sumClk;    // count for number of total clocks

    int AND_Count; //number of AND instruction
    int TAD_Count; //number of TAD instruction
    int ISZ_Count; //number of ISZ instruction 
    int DCA_Count; //number of DCA instruction
    int JMS_Count; //number of JMS instruction 
    int JMP_Count; //number of JMP instruction 
    int IO_Count;  //number of IO instruction 
    int uInstr_Count; //number of micro instruction 

    pagetable *memory;  // memory
	std::ofstream *outputTraceFile; // used to create tracefile

    //function for rotating bits
    void rotateBits(int accumulator, int link, char dir);	

    //function for finding the effective address
    int find_EAddr(bool addr_bit,bool mem_page,int offset);

	// increments the program counter
    void increment_PC();

    //reads bit x in a 12 bit register
    bool read_bit_x(int input,int x);

    //wrapper for loading from memory
    int MEM_LOAD(int address);

    //wrapper for storing into memory
    void MEM_STORE(int address,int value);

};
