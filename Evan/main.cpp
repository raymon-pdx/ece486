#include <iostream>
#include "constants.h"

using namespace std;



int PDP_uintructions(bool bit3, bool bit4, int offset);
void rotateBits(int accumulator, int link, char dir);
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


    cout<<"\nTesting group 2 uinstructions:\n\n";

    cout<<"Testing OR subgroup:\n";
    cout<<"instuction=7420o link=1b\n";
    cout<<"Expected: SKIP!\n";
    link=1;
    cout<<"result:   ";
    PDP_uintructions(1,0,272);   
    cout<<"\n\n";

    cout<<"instuction=7420o link=0b\n";
    cout<<"Expected: [nothing]\n";
    link=0;
    cout<<"result:   ";
    PDP_uintructions(1,0,272);   
    cout<<"\n\n";

}



int PDP_uintructions(bool bit3, bool bit4, int offset){
	// TODO: add increment_PC() to specific uInst
	++uInstr_Count;
	++sumInstr;
    sumClk = sumClk + 1; //takes 1 clk cycles
	
	//increment_PC();
    bool bit5  = read_bit_x(offset, 5);
    bool bit6  = read_bit_x(offset, 6);
    bool bit7  = read_bit_x(offset, 7);
    bool bit8  = read_bit_x(offset, 8);
    bool bit9  = read_bit_x(offset, 9);
    bool bit10 = read_bit_x(offset,10);
    bool bit11 = read_bit_x(offset,11);

//*************GROUP 1*******************
    if(!bit3){

        if(bit4){   //clear accumulator (CLA)
            AC = 0; 
        }

        if(bit5){   //clear link (CLL)
            link = 0;
        }

        if(bit6){   //complement accumulator (CMA)
            AC = (~AC) & ((1<<pdp8::REGISTERSIZE)-1);
        }

        if(bit7){   //complement link (CML)
            link = ~link;
        }

        if(bit11){  //increment accumulator (IAC)
            AC = (AC + 1) & ((1<<pdp8::REGISTERSIZE)-1);
        }

        if(bit8){   //rotate accumulator and link right (RAR)

           int direction = 0; //0 indicates right direction
           rotateBits(AC, link, direction);

           if(bit10){ //rotate accumulator, link right twice (RTL)
                rotateBits(AC, link, direction);
           }

        }else if(bit9){   //rotate accumulator and link left (RAL)

           int direction = 1; //1 indicates left direction
           rotateBits(AC, link, direction);

           if(bit10){ //rotate accumulator, link left twice (RTL)
                rotateBits(AC, link, direction);
           }
        }

    //*************GROUP 2*******************
    }else if(!bit11){                   //uinstructions            
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
                increment_PC();
                return 0;                 //the skip for SPA,SNA,SZL, and SKP
            }
        }else{                          //OR subgroup
            if(bit5 && read_bit_x(AC, 0)){//SMA
                increment_PC();
                return 0; //is the 0th bit of AC a 1?
            }else if(bit6 && (AC==0)){    //SZA
                increment_PC();
                return 0; //is AC 0?
            }else if(bit7 && link){       //SNL
                increment_PC();
                return 0; //is link 1?
            }
        }
                                        //CLA OSR and HLT
        if(bit4){                       //CLA
            AC=0;
        }
        if(bit9){                       //OSR
            AC=SR|AC;
        }
        if(bit10){                      //HLT
            return -1;
        }
    //*************GROUP 3*******************
    }else{              

     std::cout << "Warning: Group 3 uInstruction encountered. No Op.\n";                

    }
    return 0;
}





void rotateBits(int accumulator, int link, char dir){

    int lsb = 0; //value of lsb from accumulator
    int msb = 0; //value of msb from accumulator
    int templink = 0; //temporarily holds value of link

    #ifndef rotatebit_DEBUG
    std::cout << "Current accumulator value: " << (bitset<12>) accumulator << "\n";
	std::cout << "Current link bit: " << link << "\n";
    std::cout << "Current direction: " << dir << "\n\n";
    #endif

    if(dir == 0){ //ROTATE RIGHT

        lsb = accumulator & 1; //save lsb of accumulator
        #ifndef rotatebit_DEBUG
        std::cout << "Value of lsb is: " << lsb << "\n";
        #endif

        accumulator = accumulator >> 1; //shift accumulator to right by 1
        #ifndef rotatebit_DEBUG
		std::cout << "Value of shifted accumulator: " << (bitset<12>)accumulator << "\n";
        #endif

        templink = link; //save value of link before we change it
        link = lsb; //lsb will be shifted into the link
        #ifndef rotatebit_DEBUG
		std::cout << "New value of link: " << link << "\n";
        #endif

        templink = templink << 11; //put old value of link into accumulator MSB
        accumulator = (accumulator | templink); //put MSB into the accumulator
        #ifndef rotatebit_DEBUG
		std::cout << "New value of accumulator: " << (bitset<12>)accumulator << "\n"; 
        #endif

    }else if(dir == 1){ //ROTATE LEFT

        msb = (accumulator>>11) & 1; //save msb of accumulator
        #ifndef rotatebit_DEBUG
		std::cout << "Value of msb is: " << msb << "\n";
        #endif

        accumulator = accumulator << 1; //shift accumulator to left by 1
        #ifndef rotatebit_DEBUG
		std::cout << "Value of shifted accumulator: " << (bitset<12>)accumulator << "\n";
        #endif
     
        templink = link; //save value of link before we change it
        #ifndef rotatebit_DEBUG
		std::cout << "Value of templink: " << templink << "\n";
        #endif

        link = msb; //lsb will be shifted into the link
        #ifndef rotatebit_DEBUG
		std::cout << "New value of link: " << link << "\n";
        #endif

        accumulator = (accumulator | templink); //put MSB into the accumulator as MSB
        #ifndef rotatebit_DEBUG
		std::cout << "New value of accumulator: " << (bitset<12>)accumulator << "\n"; 
        #endif

    }else{

        std::cout << "Error rotating bits in uInstruction.\n";
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
	cout<<"SKIP!";
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


