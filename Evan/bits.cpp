#include "bits.h"

#define IO_verbose 0

#define REGISTERSIZE 12

int main(){
    return 0;
}


BitTwiddle::BitTwiddle(){
    AC=0;
    PC=0;
    link=0;

   AND_Count=0;        //number of AND instruction
   TAD_Count=0;    //number of TAD instruction
   ISZ_Count=0;    //number of ISZ instruction 
   DCA_Count=0;    //number of DCA instruction
   JMS_Count=0;    //number of JMS instruction 
   JMP_Count=0;    //number of JMP instruction 
   IO_Count=0;     //number of IO instruction 
   uInstr_Count=0; //number of micro instruction 
}


//AND C(accumulator) with memory
void BitTwiddle::PDP_AND(bool addr_bit,bool mem_page,int offset){
    AND_Count++;
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();

    AC=AC & MEM_LOAD(EAddr);
    return;
}


//two's compliment add to C(accumulator)
void BitTwiddle::PDP_TAD(bool addr_bit,bool mem_page,int offset){
    TAD_Count++;
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();

    int adda = AC;
    int addb = MEM_LOAD(EAddr);    
    int addc = adda + addb;

    /* OVERFLOW LOGIC
    if(adda&(1<<(REGISTERSIZE-1)) == addb&(1<<(REGISTERSIZE-1)) 
    && addb&(1<<(REGISTERSIZE-1)) == addc&(1<<(REGISTERSIZE-1))) { //compare the sign of a b and c, check for overflow
        //overflow=0; //no overflow
    }else{
        //overflow=1; //overflow
    }
    */

    AC=addc & ((1<<REGISTERSIZE)-1);//carry and overflow are removed

    if((1<<REGISTERSIZE) == addc&(1<<REGISTERSIZE)) link = !link;//compliment link if carry out

    return;
}


//increment memory and skip if zero
void BitTwiddle::PDP_ISZ(bool addr_bit,bool mem_page,int offset){
    ISZ_Count++;
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    int C_EAddr = MEM_LOAD(EAddr);
    increment_PC();

    int adda=AC;   
    int addc=adda + 1;

    /* OVERFLOW LOGIC
        if((adda)&(1<<(REGISTERSIZE-1))==0 
        && (addc)&(1<<(REGISTERSIZE-1))==0) { //compare the sign of a and c, check for overflow
            //overflow=0; //no overflow
        }else{
            //overflow=1; //overflow
        }
    */

    AC=addc & ((1<<REGISTERSIZE)-1);//carry and overflow are removed

    if(!((C_EAddr + 1) & ((1<<REGISTERSIZE)-1))){
        increment_PC();; //skip if zero
    }
    return;
}


// Function for Deposit and Clear Accumulator
void BitTwiddle::PDP_DCA(bool addr_bit,bool mem_page,int offset)
{
    DCA_Count++;
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
    JMS_Count++;
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
    JMP_Count++;
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











/*//
PRIVATE FUNCTIONS!
//*/
int BitTwiddle::find_EAddr(bool addr_bit,bool mem_page,int offset){
//TODO: make into case
    if(!addr_bit){
        if(!mem_page){//bit3=0 bit4=0
            if(offset>=8 && offset<=15){ //autoindexing offset=010o-017o
                int temp = MEM_LOAD(offset)+1; //load C(AutoIndex_Register)+1
                MEM_STORE(offset , temp); //store it into C(AutoIndex_Register)
                return temp; //C(AutoIndex_Register) is our EAddr
            }else{    
                return offset; //00000 cat Offset is our EAddr
            }
        }else{       //bit3=0 bit4=1            
            return (PC & ((1<<5)-1)) + offset; //PC
        }
    }else{          
        if(!mem_page){//bit3=1 bit4=0
            return MEM_LOAD(offset);
        }else{        //bit3=1 bit4=1
            return MEM_LOAD((PC & ((1<<5)-1))+offset);
        }
    }
}


void BitTwiddle::increment_PC(){
    PC=(PC + 1) & ((1<<REGISTERSIZE)-1);
    return;
}


int BitTwiddle::MEM_LOAD(int dummy){
return 0;
}

void BitTwiddle::MEM_STORE(int dummy1,int dummy2){
return;
}




