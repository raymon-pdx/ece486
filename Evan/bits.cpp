#include "bits.h"

#define IO_verbose 0

int main(){
    return 0;
}


BitTwiddle::BitTwiddle(){
    AC=0;
    PC=0;
    link=0;
}

void BitTwiddle::PDP_AND(bool addr_bit,bool mem_page,int offset){
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    MEM_STORE(AC,MEM_LOAD(AC) && MEM_LOAD(EAddr));
    return;
}

void BitTwiddle::PDP_TAD(bool addr_bit,bool mem_page,int offset){
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
//TODO: compliment link FOR 12 BITS
    MEM_STORE(AC,MEM_LOAD(AC) + MEM_LOAD(EAddr));
    return;
}

void BitTwiddle::PDP_ISZ(bool addr_bit,bool mem_page,int offset){
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    int C_EAddr = MEM_LOAD(EAddr);
    if(C_EAddr){
    //TODO: emulate 12 bit rollover
        MEM_STORE(EAddr,C_EAddr+1);
    }else{
        MEM_STORE(PC,MEM_LOAD(PC)+1);
    }
    return;
}

// Function for Deposit and Clear Accumulator
void BitTwiddle::PDP_DCA(bool addr_bit,bool mem_page,int offset)
{
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    
    // take contents of AC and place into content of EAddr
    MEM_STORE(EAddr,MEM_LOAD(AC));
    // store 0 into the content of AC (clear AC)
    MEM_STORE(AC, 0);
}

// Function for Jump to Subroutine
void BitTwiddle::PDP_JMS(bool addr_bit,bool mem_page,int offset)
{
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    
    // take contents of PC and place into content of EAddr
    MEM_STORE(EAddr,MEM_LOAD(PC));
    // take EAddr, add 1, and store into content of PC
    MEM_STORE(PC, EAddr + 1);
}

//Function for Jump
void BitTwiddle::PDP_JMP(bool addr_bit,bool mem_page,int offset)
{
    int EAddr = find_EAddr(addr_bit,mem_page,offset);

    // take EAddr and store into content of PC
    MEM_STORE(PC, EAddr);
}





void BitTwiddle::PDP_IO(int device_num,int opcode){
    //IO is a NO-OP, but I'll display flags
    //IO_verbose is #defined to be 1 or 0
    if(IO_verbose){
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

        }else if(device_num == 4){





        }
    }

    return;
}






/*//
PRIVATE FUNCTIONS!
//*/
int find_EAddr(bool addr_bit,bool mem_page,int offset){
//TODO: make into case
    if(!addr_bit){
        if(!mem_page){ //bit3=0 bit4=0
            if(offset>=8 && offset<=15){ //autoindexing offset=010o-017o
                int temp = MEM_LOAD(offset)+1; //load C(AutoIndex_Register)+1
                MEM_STORE(offset , temp); //store it into C(AutoIndex_Register)
                return temp; //C(AutoIndex_Register) is our EAddr
            }else{
                return offset; //00000 cat Offset is our EAddr
            }
        }else{            
            return (PC & 3968) + offset; //PC
        }
    }else{
        if(mem_page){
            return MEM_LOAD(offset);
        }else{
            return MEM_LOAD((PC & 3968)+offset);
        }
    }
}


