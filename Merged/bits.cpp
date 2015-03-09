//Evan Sprecher & Ommaimah Hussein
#include "bits.h"

// TODO: dca may not be storing correct value back to memory

// TODO: look at output3.txt for help, LOCATION: merged/test_files

//-----------------------------------------------------------
// Constructor
//-----------------------------------------------------------
BitTwiddle::BitTwiddle(pagetable *table, std::ofstream *output)
{
	AC=0;
	PC=128;         //PC starts at 0200o
	link=false;
    SR=0;
	
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
	outputTraceFile = output;
}


//-----------------------------------------------------------
// Destructor
//-----------------------------------------------------------
BitTwiddle::~BitTwiddle(){
}


//-----------------------------------------------------------
// Function for Logical AND
//-----------------------------------------------------------
void BitTwiddle::PDP_AND(bool addr_bit,bool mem_page,int offset){

    ++AND_Count;
	++sumInstr;
    sumClk = sumClk + 2; //takes 2 clk cycles

    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();

    AC=AC & MEM_LOAD(EAddr);
	return;
}


//-----------------------------------------------------------
// Function for Two's Complement Add 
//-----------------------------------------------------------
void BitTwiddle::PDP_TAD(bool addr_bit,bool mem_page,int offset){

    ++TAD_Count;
	++sumInstr;
    sumClk = sumClk + 2; //takes 2 clk cycles

    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();

    int adda = AC;
    int addb = MEM_LOAD(EAddr);    
    int addc = adda + addb;

	AC = addc & ((1 << pdp8::REGISTERSIZE) - 1);//carry and overflow are removed

	if ((1 << pdp8::REGISTERSIZE) == (addc&(1 << pdp8::REGISTERSIZE))) link = !link;//compliment link if carry out
	return;
}


//-----------------------------------------------------------
// Function for Increment and Skip on Zero
//-----------------------------------------------------------
void BitTwiddle::PDP_ISZ(bool addr_bit,bool mem_page,int offset){

    ++ISZ_Count;
	++sumInstr;
    sumClk = sumClk + 2; //takes 2 clk cycles

    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    int C_EAddr = MEM_LOAD(EAddr);
    increment_PC();
   
    int addc=C_EAddr + 1;

	MEM_STORE(EAddr,addc & ((1 << pdp8::REGISTERSIZE) - 1));//carry and overflow are removed

	if (!((addc) & ((1 << pdp8::REGISTERSIZE) - 1))){
        increment_PC(); //skip if zero
    }
	return;
}


//-----------------------------------------------------------
// Function for Deposit and Clear Accumulator
//-----------------------------------------------------------
void BitTwiddle::PDP_DCA(bool addr_bit,bool mem_page,int offset)
{
    ++DCA_Count;
	++sumInstr;
    sumClk = sumClk + 2; //takes 2 clk cycles

    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();
    
    // take AC and place into content of EAddr
    MEM_STORE(EAddr,AC);
    // store 0 into AC (clear AC)
    AC = 0;
	return;
}


//-----------------------------------------------------------
// Function for Jump to Subroutine
//-----------------------------------------------------------
void BitTwiddle::PDP_JMS(bool addr_bit,bool mem_page,int offset)
{
    ++JMS_Count;
	++sumInstr;
    sumClk = sumClk + 2; //takes 2 clk cycles

    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();
    
    // take contents of PC and place into content of EAddr
    MEM_STORE(EAddr,PC);
    // take EAddr, add 1, and store into PC
	PC = (EAddr + 1) & ((1 << pdp8::REGISTERSIZE) - 1);//carry and overflow are removed
	return;
}


//-----------------------------------------------------------
// Function for Jump
//-----------------------------------------------------------
void BitTwiddle::PDP_JMP(bool addr_bit,bool mem_page,int offset)
{
    ++JMP_Count;
	++sumInstr;
    sumClk = sumClk + 1; //takes 1 clk cycle

    int EAddr = find_EAddr(addr_bit,mem_page,offset);
    increment_PC();

    // take EAddr and store into PC
    PC= EAddr;
	return;
}


//-----------------------------------------------------------
// Forbidden IO functionality
//-----------------------------------------------------------
void BitTwiddle::PDP_IO(int device_num,int opcode){

    ++IO_Count;
	++sumInstr;

	increment_PC();
    std::cout << "Warning: I/O instruction encountered.\n";                
	return;
}


//-----------------------------------------------------------
// Function for Micro Instructions
//-----------------------------------------------------------
int BitTwiddle::PDP_uintructions(bool bit3, bool bit4, int offset)
{
	++uInstr_Count;
	++sumInstr;
    sumClk = sumClk + 1; //takes 1 clk cycles
	
	increment_PC();
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
            link = !link;
        }

        if(bit11){  //increment accumulator (IAC)
            int addc= AC + 1;
            AC = addc & ((1<<pdp8::REGISTERSIZE)-1);
            if ((1 << pdp8::REGISTERSIZE) == (addc&(1 << pdp8::REGISTERSIZE))) link = !link;
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
                increment_PC();		//the skip for SPA,SNA,SZL, and SKP             
            }
        }else{                          //OR subgroup
            if(bit5 && read_bit_x(AC, 0)){//SMA
                increment_PC(); //is the 0th bit of AC a 1? 
            }else if(bit6 && (AC==0)){    //SZA
                increment_PC(); //is AC 0? 
            }else if(bit7 && link){       //SNL
                increment_PC(); //is link 1?
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

// display data from BitTwiddle class
void BitTwiddle::display()
{
	//print out brief summary 
	std::cout << "\n-----------PDP-8 ISA Simulation Summary---------------\n\n";
	std::cout << "Total number of Instructions executed: " << std::dec<< sumInstr << "\n";
	std::cout << "Total number of clock cycles consumed: " << std::dec<< sumClk << "\n\n";
	std::cout << "**Number of times each instruction type was executed**\n";
	std::cout << "|-----------------------------------------------------\n";
	std::cout << "|    Mnemonic   | Number of times executed            \n";
	std::cout << "|-----------------------------------------------------\n";
	std::cout << "|      AND      |       " << std::dec<< AND_Count << "\n";
	std::cout << "|      TAD      |       " << std::dec<< TAD_Count << "\n";
	std::cout << "|      ISZ      |       " << std::dec<< ISZ_Count << "\n";
	std::cout << "|      DCA      |       " << std::dec<< DCA_Count << "\n";
	std::cout << "|      JMS      |       " << std::dec<< JMS_Count << "\n";
	std::cout << "|      JMP      |       " << std::dec<< JMP_Count << "\n";
	std::cout << "|     <IO>      |       " << std::dec<< IO_Count << "\n";
	std::cout << "| uInstructions |       " << std::dec<< uInstr_Count << "\n";
	std::cout << "------------------------------------------------------\n";
}

//-----------------------------------------------------------
// Function for rotating bits (used in uInstruction Group 1)
//-----------------------------------------------------------
void BitTwiddle::rotateBits(int accumulator, int link, char dir){

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


//-----------------------------------------------------------
// Function for displaying registers
//----------------------------------------------------------
void BitTwiddle::displayRegisters()
{
	std::cout << "pc=" << std::oct << PC << ", ac=" << AC << ", link=" << link << std::endl;
}


//____________________________________________________________
//                    PRIVATE FUNCTIONS! 
//____________________________________________________________

//-----------------------------------------------------------
// Function for finding the effective address
//-----------------------------------------------------------
int BitTwiddle::find_EAddr(bool addr_bit,bool mem_page,int offset){
    if(!addr_bit){
        if(!mem_page){//bit3=0 bit4=0   
            return offset; //00000 cat Offset is our EAddr (zero page addressing) 
        }else{       //bit3=0 bit4=1 current page addresssing            
            return ((PC & (((1<<5)-1)<<7)) + offset);
        }
    }else{ 
        sumClk += 1; //one additional clk cycle for indirect addressing        
        if(!mem_page){//bit3=1 bit4=0 zero page indirect addressing
            if(offset>=8 && offset<=15){ //autoindexing offset=010o-017o
                sumClk += 2; //two additional clk cycle for autoindexing 
                int temp = MEM_LOAD(offset)+1; //load C(AutoIndex_Register)+1
                temp=temp & ((1<<pdp8::REGISTERSIZE)-1); //scrub it to 12 bits
                MEM_STORE(offset , temp); //store it into C(AutoIndex_Register)
                return temp; //C(AutoIndex_Register) is our EAddr
            }else{ 
                ++sumClk; //one additional clk cycle for indirect addressing 
                return MEM_LOAD(offset);
            }
        }else{        //bit3=1 bit4=1 current page indirect addressing
            if((PC & (((1<<5)-1)<<7))==0 && offset>=8 && offset<=15){ //autoindexing offset=010o-017o
                sumClk += 2; //two additional clk cycle for autoindexing 
                int temp = MEM_LOAD(offset)+1; //load C(AutoIndex_Register)+1
                temp=temp & ((1<<pdp8::REGISTERSIZE)-1); //scrub it to 12 bits
                MEM_STORE(offset , temp); //store it into C(AutoIndex_Register)
                return temp; //C(AutoIndex_Register) is our EAddr
            }else{
                ++sumClk; //one additional clk cycle for indirect addressing 
                return MEM_LOAD((PC & (((1<<5)-1)<<7)) + offset);
        }
    }
}


//-----------------------------------------------------------
// Function for incrementing the PC
//----------------------------------------------------------
void BitTwiddle::increment_PC(){
	PC = (PC + 1) & ((1 << pdp8::REGISTERSIZE) - 1);
    return;
}


//-----------------------------------------------------------
// Function for reading a specific bit location
//----------------------------------------------------------
bool BitTwiddle::read_bit_x(int input,int x){

	return 1 & (input >> (pdp8::REGISTERSIZE - (x + 1)));
}


//-----------------------------------------------------------
// Function for loading from memory
//----------------------------------------------------------
int BitTwiddle::MEM_LOAD(int address){

    //*********IMPLEMENTING TRACE FILE****************
    int type = 0; //1 - data read (load)
    (*outputTraceFile) << type << " " << std::oct << address << std::endl;
    //************************************************

	return memory->load(address);
}


//-----------------------------------------------------------
// Function for storing to memory
//----------------------------------------------------------
void BitTwiddle::MEM_STORE(int address,int value){

    //*********IMPLEMENTING TRACE FILE****************
    int type = 1; //1 - data write (store)
    (*outputTraceFile) << type << " " << std::oct << address << std::endl;
    //************************************************
	memory->store(address, value);
	return;
}

