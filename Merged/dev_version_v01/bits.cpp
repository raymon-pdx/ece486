//Evan Sprecher
#pragma once
#include "bits.h"
#include "memory.h"
#include <iostream>


// default constructor
BitTwiddle::BitTwiddle(){
	AC=0;
	PC=128;         //PC starts at 0200o
	link=0;
	
	sumInstr = 0;
	sumClk = 0;

	AND_Count=0;     //number of AND instruction
	TAD_Count=0;     //number of TAD instruction
	ISZ_Count=0;     //number of ISZ instruction 
	DCA_Count=0;     //number of DCA instruction
	JMS_Count=0;     //number of JMS instruction 
	JMP_Count=0;     //number of JMP instruction 
	IO_Count=0;      //number of IO instruction 
	uInstr_Count=0;  //number of micro instruction 
	memory = new pagetable(32,128);
}

// constructor
BitTwiddle::BitTwiddle(pagetable *table)
{
	AC=0;
	PC=128;         //PC starts at 0200o
	link=0;
	
	sumInstr = 0;
	sumClk = 0;

	AND_Count=0;     //number of AND instruction
	TAD_Count=0;     //number of TAD instruction
	ISZ_Count=0;     //number of ISZ instruction 
	DCA_Count=0;     //number of DCA instruction
	JMS_Count=0;     //number of JMS instruction 
	JMP_Count=0;     //number of JMP instruction 
	IO_Count=0;      //number of IO instruction 
	uInstr_Count=0;  //number of micro instruction 
	
	memory = table;
}


BitTwiddle::~BitTwiddle(){
	delete memory;
}


//AND C(accumulator) with memory
void BitTwiddle::PDP_AND(bool addr_bit,bool mem_page,int offset){
    ++AND_Count;
	++sumInstr;
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();

    AC=AC & MEM_LOAD(EAddr);
    return;
}


//two's compliment add to C(accumulator)
void BitTwiddle::PDP_TAD(bool addr_bit,bool mem_page,int offset){
    ++TAD_Count;
	++sumInstr;
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();

    int adda = AC;
    int addb = MEM_LOAD(EAddr);    
    int addc = adda + addb;

    AC=addc & ((1<<REGISTERSIZE)-1);//carry and overflow are removed

    if((1<<REGISTERSIZE) == (addc&(1<<REGISTERSIZE))) link = !link;//compliment link if carry out

    return;
}


//increment memory and skip if zero
void BitTwiddle::PDP_ISZ(bool addr_bit,bool mem_page,int offset){
    ++ISZ_Count;
	++sumInstr;
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    int C_EAddr = MEM_LOAD(EAddr);
    increment_PC();

    int adda=AC;   
    int addc=adda + 1;

    AC=addc & ((1<<REGISTERSIZE)-1);//carry and overflow are removed

    if(!((C_EAddr + 1) & ((1<<REGISTERSIZE)-1))){
        increment_PC(); //skip if zero
    }
    return;
}


// Function for Deposit and Clear Accumulator
void BitTwiddle::PDP_DCA(bool addr_bit,bool mem_page,int offset)
{
    ++DCA_Count;
	++sumInstr;
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();
    
    // take AC and place into content of EAddr
    MEM_STORE(EAddr,AC);
    // store 0 into AC (clear AC)
    AC = 0;
}


// Function for Jump to Subroutine
void BitTwiddle::PDP_JMS(bool addr_bit,bool mem_page,int offset)
{
    ++JMS_Count;
	++sumInstr;
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();
    
    // take contents of PC and place into content of EAddr
    MEM_STORE(EAddr,PC);
    // take EAddr, add 1, and store into PC
    PC = (EAddr + 1) & ((1<<REGISTERSIZE)-1);//carry and overflow are removed
}


//Function for Jump
void BitTwiddle::PDP_JMP(bool addr_bit,bool mem_page,int offset)
{
    ++JMP_Count;
	++sumInstr;
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();

    // take EAddr and store into PC
    PC= EAddr;
}


//forbidden IO funtionality
void BitTwiddle::PDP_IO(int device_num,int opcode){
    increment_PC();
    //IO is a NO-OP, but I'll display flags
    //IO_verbose is #defined to be 1 or 0
    if(device_num == 3){
        switch(opcode){
          case 0:
            //clear keayboard flag
            break;
          case 1:
            //skip on keyboard flag set
            break;
          case 2:
            //clear keyboad flag and accumulator
            //AC=0;
            break;
          case 4:
            //read keyboard buffer static
            //AC4-12 = AC4-12 | keyboard buffer;
            break;
          case 6:
            //read keyboard buffer dynamic
            //MEM_STORE(AC,0);
            //clear keyboard flag
            //AC4-12 = AC4-12 | keyboard buffer;
            break;
          default:
            break;
        }

    }else if(device_num == 4){


    }

    return;
}


void BitTwiddle::PDP_uintructions(bool bit3, bool bit4, int offset){
	++uInstr_Count;
	++sumInstr;
    bool bit5  = read_bit_x(offset, 5);
    bool bit6  = read_bit_x(offset, 6);
    bool bit7  = read_bit_x(offset, 7);
    bool bit8  = read_bit_x(offset, 8);
    bool bit9  = read_bit_x(offset, 9);
    bool bit10 = read_bit_x(offset,10);
    bool bit11 = read_bit_x(offset,11);

    if(!bit3){                          //group 1 uinstructions



    }else if(!bit11){                   //group 2 uinstructions            
        if(bit8){                       //AND subgroup and SKP
            bool cond5=1; //combinations will skip when all conditions are true.
            bool cond6=1; //so we do not skip if any one condition is false.
            bool cond7=1; //the idea is that these get flagged false when a bit is asserted and it is conditionally false
            // cond 5 6 and 7 are then ANDed together at the end and if true, skip.
            if(bit5){                   //SPA
                cond5=!read_bit_x(AC, 0);//is the 0th bit of AC a 0?
            }
            if(bit6){                   //SNA
                cond6=(AC!=0);//is AC not 0?
            }
            if(bit7){                   //SZL
                cond7=!link;//is link 0?
            }
            if(cond5 && cond6 && cond7){
                return;                 //the skip for SPA,SNA,SZL, and SKP
            }
        }else{                          //OR subgroup
            if(bit5 && read_bit_x(AC, 0)){//SMA
                return; //is the 0th bit of AC a 1?
            }
            if(bit6 && (AC==0)){            //SZA
                return; //is AC 0?
            }
            if(bit7 && link){           //SNL
                return; //is link 1?
            }
        }
                                        //CLA OSR and HLT
        if(bit4){                       //CLA
            AC=0;
        }
        if(bit9){                       //OSR
        //TODO: What is a switch register?
        //    SR=SR|AC;
        }
        if(bit10){                      //HLT
        //TODO: halt. What does that mean?
        //    while(1); //a halt is the same as getting stuck on a while, right?
        }
    }else{                              //group 3 uinstructions


    }
    return;

}

// display data from BitTwiddle class
void BitTwiddle::display()
{
	//print out brief summary 
	std::cout << "-----------PDP-8 ISA Simulation Summary---------------\n\n";
	//TODO: Add the +1 to numInstr for every instruction
	std::cout << "Total number of Instructions executed: " << sumInstr << "\n";
	//TODO: Add correct amount of clock cycles to each function
	std::cout << "Total number of clock cycles consumed: " << sumClk << "\n\n";
	std::cout << "**Number of times each instruction type was executed**\n";
	//TODO: Add variable to each function to hold count
	std::cout << "|-----------------------------------------------------\n";
	std::cout << "|    Mnemonic   | Number of times executed            \n";
	std::cout << "|-----------------------------------------------------\n";
	std::cout << "|      AND      |       " << AND_Count << "\n";
	std::cout << "|      TAD      |       " << TAD_Count << "\n";
	std::cout << "|      ISZ      |       " << ISZ_Count << "\n";
	std::cout << "|      DCA      |       " << DCA_Count << "\n";
	std::cout << "|      JMS      |       " << JMS_Count << "\n";
	std::cout << "|      JMP      |       " << JMP_Count << "\n";
	std::cout << "|     <IO>      |       " << IO_Count << "\n";
	std::cout << "| uInstructions |       " << uInstr_Count << "\n";
	std::cout << "------------------------------------------------------\n";
}


/*//
PRIVATE FUNCTIONS!
//*/
int BitTwiddle::find_EAddr(bool addr_bit,bool mem_page,int offset){
    if(!addr_bit){
        if(!mem_page){//bit3=0 bit4=0
            if(offset>=8 && offset<=15){ //autoindexing offset=010o-017o
                int temp = MEM_LOAD(offset)+1; //load C(AutoIndex_Register)+1
                temp=temp & ((1<<REGISTERSIZE)-1); //scrub it to 12 bits
                MEM_STORE(offset , temp); //store it into C(AutoIndex_Register)
                return temp; //C(AutoIndex_Register) is our EAddr
            }else{    
                return offset; //00000 cat Offset is our EAddr
            }
        }else{       //bit3=0 bit4=1            
            return ((PC & (((1<<5)-1)<<7)) + offset); //PC
        }
    }else{          
        if(!mem_page){//bit3=1 bit4=0
            return MEM_LOAD(offset);
        }else{        //bit3=1 bit4=1
            return MEM_LOAD((PC & (((1<<5)-1)<<7)) + offset);
        }
    }
}


void BitTwiddle::increment_PC(){
    PC=(PC + 1) & ((1<<REGISTERSIZE)-1);
    return;
}


bool BitTwiddle::read_bit_x(int input,int x){
   return 1 & (input>>(REGISTERSIZE - (x + 1)));
}


//temp functions
int BitTwiddle::MEM_LOAD(int address){
  temp = memory->load(address);
  return temp;
}


void BitTwiddle::MEM_STORE(int address,int value){
  memory->store(address, value);
}

