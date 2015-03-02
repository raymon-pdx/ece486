//Ommaimah Hussein

#include "bits.h"
#include <iostream>
#include <bitset>

using namespace std;

#define IO_verbose 0
#define REGISTERSIZE 12

#define rotatebit_DEBUG

int main(){

    BitTwiddle bits;

    cout<<"\nTesting micro-Instructions Group 1:\n\n";

    //Testing NOP
    //Testing CLA 
    //Testing CLL
    //Testing CMA 
    //Testing CML 
    //Testing IAC 
    //Testing RAR
    //Testing RTR
    //Testing RAL
    //Testing RTL

    cout<<"C(EAddr)=4107o AC=4077o link=0b  //-1977+-1985\n";
    cout<<"AC Expected: 206o link: 1b      //134\n";
    bits.temp=2119;
    bits.AC=2111;
    bits.link=0;
    bits.PDP_TAD(1,0,123);
    cout<<"AC result:   "<<bits.AC<<" link: "<<bits.link<<"\n\n";

}

//-----------------------------------------------------------
// Constructor
//-----------------------------------------------------------
BitTwiddle::BitTwiddle(){

   //initialize value for AC, PC, and link
   AC = 0;
   PC = 128;         //PC starts at 0200o
   link = 0;

   //initialize values for sum of instructions
   sumInstr = 0;

   //initialize vlaues for clock cycles
   sumClk = 0;

   //initialize values for instruction accesses
   sumAND = 0;
   sumTAD = 0; 
   sumISZ = 0; 
   sumDCA = 0; 
   sumJMS = 0;  
   sumJMP = 0;  
   sumIO = 0;   
   sumuInstr = 0;  

}

//-----------------------------------------------------------
// Destructor
//-----------------------------------------------------------
BitTwiddle::~BitTwiddle(){

    //print out brief summary 
    cout << "-----------PDP-8 ISA Simulation Summary---------------\n\n";
    //TODO: Add the +1 to numInstr for every instruction
    cout << "Total number of Instructions executed: " << sumInstr << "\n";
    //TODO: Add correct amount of clock cycles to each function
    cout << "Total number of clock cycles consumed: " << sumClk << "\n\n";
    cout << "**Number of times each instruction type was executed**\n"
    cout << "|-----------------------------------------------------\n";
    cout << "|    Mnemonic   | Number of times executed            \n";
    cout << "|-----------------------------------------------------\n";
    cout << "|      AND      |       " << sumAND    << "\n";
    cout << "|      TAD      |       " << sumTAD    << "\n";
    cout << "|      ISZ      |       " << sumISZ    << "\n";
    cout << "|      DCA      |       " << sumDCA    << "\n";
    cout << "|      JMS      |       " << sumJMS    << "\n";
    cout << "|      JMP      |       " << sumJMP    << "\n";
    cout << "|     <IO>      |       " << sumIO     << "\n";
    cout << "| uInstructions |       " << sumuInstr << "\n";
    cout << "|-----------------------------------------------------\n\n";

}

//-----------------------------------------------------------
// Function for Logical AND
//-----------------------------------------------------------
void BitTwiddle::PDP_AND(bool addr_bit,bool mem_page,int offset){
    sumAND++;
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();

    AC=AC & MEM_LOAD(EAddr);
    return;
}

//-----------------------------------------------------------
// Function for Two's Complement Add 
//-----------------------------------------------------------
void BitTwiddle::PDP_TAD(bool addr_bit,bool mem_page,int offset){
    sumTAD++;
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();

    int adda = AC;
    int addb = MEM_LOAD(EAddr);    
    int addc = adda + addb;

    AC=addc & ((1<<REGISTERSIZE)-1);//carry and overflow are removed

    if((1<<REGISTERSIZE) == (addc&(1<<REGISTERSIZE))) link = !link;//compliment link if carry out

    return;
}

//-----------------------------------------------------------
// Function for Increment and Skip on Zero
//-----------------------------------------------------------
void BitTwiddle::PDP_ISZ(bool addr_bit,bool mem_page,int offset){
    sumISZ++;
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

//-----------------------------------------------------------
// Function for Deposit and Clear Accumulator
//-----------------------------------------------------------
void BitTwiddle::PDP_DCA(bool addr_bit,bool mem_page,int offset)
{
    sumDCA++;
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();
    
    //take AC and place into content of EAddr
    MEM_STORE(EAddr,AC);
    //store 0 into AC (clear AC)
    AC = 0;
    return;
}

//-----------------------------------------------------------
// Function for Jump to Subroutine
//-----------------------------------------------------------
void BitTwiddle::PDP_JMS(bool addr_bit,bool mem_page,int offset)
{
    sumJMS++;
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();
    
    //take contents of PC and place into content of EAddr
    MEM_STORE(EAddr,PC);
    //take EAddr, add 1, and store into PC
    PC = (EAddr + 1) & ((1<<REGISTERSIZE)-1);//carry and overflow are removed
    return;
}

//-----------------------------------------------------------
// Function for Jump
//-----------------------------------------------------------
void BitTwiddle::PDP_JMP(bool addr_bit,bool mem_page,int offset)
{
    sumJMP++;
    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();

    //take EAddr and store into PC
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

//-----------------------------------------------------------
// Function for Micro Instructions
//-----------------------------------------------------------
void BitTwiddle::PDP_uintructions(bool bit3, bool bit4, int offset){

    bool bit5  = read_bit_x(offset, 5);
    bool bit6  = read_bit_x(offset, 6);
    bool bit7  = read_bit_x(offset, 7);
    bool bit8  = read_bit_x(offset, 8);
    bool bit9  = read_bit_x(offset, 9);
    bool bit10 = read_bit_x(offset,10);
    bool bit11 = read_bit_x(offset,11);

    if(!bit3){  //group 1 uinstructions

        if(bit4){         //clear accumulator

            AC = 0; 

        }else if(bit5){   //clear link

            link = 0;

        }else if(bit6){   //complement accumulator

            AC = ~AC;

        }else if(bit7){   //complement link

            link = ~link;

        }else if(bit11){  //increment accumulator

            AC = AC++;

        }else if(bit8){   //rotate accumulator, link right

           char direction = 'R';
           rotateBits(AC, link, direction);

           if(bit10){ //rotate accumulator, link right twice (rotate once more)

            rotateBits(AC, link, direction);

           }

        }else if(bit9){   //rotate accumulator, link left

           char direction = 'L';
           rotateBits(AC, link, direction);

           if(bit10){ //rotate accumulator, link left twice (rotate once more)

           rotateBits(AC, link, direction);

           }

        }else{            //no operation

            warningMessage();
        }

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

//-----------------------------------------------------------
// Function for outputing trace file
//-----------------------------------------------------------
int BitTwiddle::traceFile(int type, int address){

   #ifndef timeStamp
   //call timestamp function
   time_t rawtime;
   time (&rawtime);
   #endif

   //create an output trace file
   ofstream outputTraceFile;
   outputTraceFile.open("TraceFile.txt");

   #ifndef timeStamp
   //add time stamp as header of tracefile
   outputTraceFile << "***** TIME STAMP *****\n";
   outputTraceFile << "Trace file generated: " << ctime(&rawtime);
   #endif

   //use outputTraceFile like cout, but into txt file 
   outputTraceFile << type << " " << oct << address << endl;

   outputTraceFile.close();
   cout << "Trace file generated.\n";

   return 0;

}

//-----------------------------------------------------------
// Function for displaying warning for a No-OP
//-----------------------------------------------------------
void BitTwiddle::warningMessage()
{
    cout << "Warning. A NOP (No Operation) has been encountered.\n";
    return;
}

//-----------------------------------------------------------
// Function for rotating bits (used in uInstruction Group 1)
//-----------------------------------------------------------
int BitTwiddle::rotateBits(int accumulator, int link, char dir){

    int lsb; //value of lsb from accumulator
    int msb; //value of msb from accumulator
    int templink; //temporarily holds value of link

    #ifndef rotatebit_DEBUG
    cout << "Current accumulator value: " << (bitset<12>) accumulator << "\n";
    cout << "Current link bit: " << link << "\n"; 
    cout << "Current direction: " << dir << "\n\n";
    #endif

    if(dir == 'R'){ //ROTATE RIGHT

        lsb = accumulator & 1; //save lsb of accumulator
        #ifndef rotatebit_DEBUG
        cout << "Value of lsb is:" << lsb << "\n\n";
        #endif

        accumulator = accumulator >> 1; //shift accumulator to right by 1
        #ifndef rotatebit_DEBUG
        cout << "Value of shifted accumulator: " << (bitset<12>)accumulator << "\n";
        #endif

        templink = link; //save value of link before we change it
        link = lsb; //lsb will be shifted into the link
        #ifndef rotatebit_DEBUG
        cout << "New value of link: " << link << "\n";
        #endif

        templink = templink << 11; //put old value of link into accumulator MSB
        accumulator = (accumulator | templink); //put MSB into the accumulator
        #ifndef rotatebit_DEBUG
        cout << "New value of accumulator: " << (bitset<12>)accumulator << "\n"; 
        #endif

    }else if(dir == 'L'){ //ROTATE LEFT

        msb = (accumulator>>11) & 1; //save msb of accumulator
        #ifndef rotatebit_DEBUG
        cout << "Value of msb is:" << msb << "\n\n";
        #endif

        accumulator = accumulator << 1; //shift accumulator to left by 1
        #ifndef rotatebit_DEBUG
        cout << "Value of shifted accumulator: " << (bitset<12>)accumulator << "\n";
        #endif
     
        templink = link; //save value of link before we change it
        #ifndef rotatebit_DEBUG
        cout << "Value of templink: " << templink << "\n";
        #endif

        link = msb; //lsb will be shifted into the link
        #ifndef rotatebit_DEBUG
        cout << "New value of link: " << link << "\n";
        #endif

        accumulator = (accumulator | templink); //put MSB into the accumulator as MSB
        #ifndef rotatebit_DEBUG
        cout << "New value of accumulator: " << (bitset<12>)accumulator << "\n"; 
        #endif

    }else{

        cout << "Error in reading direction of rotation.\n";
    }
    return;
}

//-----------------------------------------------------------
//                    PRIVATE FUNCTIONS! 
//-----------------------------------------------------------
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
int BitTwiddle::MEM_LOAD(int dummy){
//return 0;
  return temp;
}

void BitTwiddle::MEM_STORE(int dummy1,int dummy2){
return;
}

