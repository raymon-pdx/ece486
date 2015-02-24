#include <iostream>
#include "constants.h"

using namespace std;




void PDP_ISZ(bool addr_bit,bool mem_page,int offset);


int find_EAddr(bool addr_bit,bool mem_page,int offset);
void increment_PC();
bool read_bit_x(int input,int x);
int MEM_LOAD(int address);
void MEM_STORE(int address,int value);


    int AC=5;          //accumulator
    int PC;          //Program Counter
    bool link;       //link bit
    int SR;          //console switch register
    int temp;

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


int main(){


    cout<<"\nTesting the first 6 instuctions:\n\n";


    cout<<"C(EAddr)=0000o\n";
    cout<<"C(EAddr) Expected: 0001o\n";
    temp=0;
    cout<<"C(EAddr) result:   ";
    PDP_ISZ(1,0,123);   
    cout<<std::oct <<AC<<"o\n\n";

    cout<<"C(EAddr)=0000o\n";
    cout<<"C(EAddr) Expected: 0002o\n";
    temp=1;
    cout<<"C(EAddr) result:   ";
    PDP_ISZ(1,0,123);   
    cout<<std::oct <<AC<<"o\n\n";

    cout<<"C(EAddr)=0000o\n";
    cout<<"C(EAddr) Expected: 7777o\n";
    temp=4094;
    cout<<"C(EAddr) result:   ";
    PDP_ISZ(1,0,123);   
    cout<<std::oct <<AC<<"o\n\n";

    cout<<"C(EAddr)=0000o\n";
    cout<<"C(EAddr) Expected: 0000o\n";
    temp=4094;
    cout<<"C(EAddr) result:   ";
    PDP_ISZ(1,0,123);   
    cout<<std::oct <<AC<<"o\n\n";

}





void PDP_ISZ(bool addr_bit,bool mem_page,int offset){

    ++ISZ_Count;
	++sumInstr;
    sumClk = sumClk + 2; //takes 2 clk cycles

    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    int C_EAddr = MEM_LOAD(EAddr);
    increment_PC();
   
    int addc=C_EAddr + 1;

    cout<<"\n"<<(addc & ((1 << pdp8::REGISTERSIZE) - 1))<<"\n";
	MEM_STORE(EAddr,addc & ((1 << pdp8::REGISTERSIZE) - 1));//carry and overflow are removed

	if (!((addc) & ((1 << pdp8::REGISTERSIZE) - 1))){
        increment_PC(); //skip if zero
    }
	return;
}




int find_EAddr(bool addr_bit,bool mem_page,int offset){
    if(!addr_bit){
        if(!mem_page){//bit3=0 bit4=0
            if(offset>=8 && offset<=15){ //autoindexing offset=010o-017o
                int temp = MEM_LOAD(offset)+1; //load C(AutoIndex_Register)+1
                temp=temp & ((1<<pdp8::REGISTERSIZE)-1); //scrub it to 12 bits
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



//-----------------------------------------------------------
// Function for incrementing the PC
//----------------------------------------------------------
void increment_PC(){
	PC = (PC + 1) & ((1 << pdp8::REGISTERSIZE) - 1);
    return;
}


//-----------------------------------------------------------
// Function for reading a specific bit location
//----------------------------------------------------------
bool read_bit_x(int input,int x){

	return 1 & (input >> (pdp8::REGISTERSIZE - (x + 1)));
}





//-----------------------------------------------------------
// Function for loading from memory
//----------------------------------------------------------
int MEM_LOAD(int address){
	return temp;
}


//-----------------------------------------------------------
// Function for storing to memory
//----------------------------------------------------------
void MEM_STORE(int address,int value){
    cout<<value;
	return;
}


