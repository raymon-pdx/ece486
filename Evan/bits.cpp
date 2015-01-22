#include "bits.h"

#define IO_verbose 0

int main(){
    return 0;
}


BitTwiddle::BitTwiddle(){
    AC=0;
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
    //TODO: implement PC
//        MEM_STORE(PC,MEM_LOAD(PC)+1);
    }
    return;
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
s




        }
    }

    return;
}






/*//
PRIVATE FUNCTIONS!
//*/
int find_EAddr(bool addr_bit,bool mem_page,int offset){
//TODO: make into case
    if(addr_bit){
        if(mem_page){
            if(offset>=8 && offset<=15){
                int auto_index;
                temp = MEM_LOAD(offset) + 1;
                MEM_STORE(offset , temp);
                return temp;
            }else{
                return offset;
            }
        }else{
//TODO: need to implement PC variable            
//            return PC<<7+offset;
        }
    }else{
        if(mem_page){
            return MEM_LOAD(offset);
        }else{
//            return MEM_LOAD(pc<<7+offset);
        }
    }
}


