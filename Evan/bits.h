//--------------------------------------------------------------------------------------
// Class to hold functions for implementing the PDP-8 Instructions
//--------------------------------------------------------------------------------------
class BitTwiddle{

public:

//constructor
BitTwiddle();
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

//TODO IO and uinstructions have specal inputs, not the normal ones.
//int PDP_IO(bool addr_bit,bool mem_page,int offset);
//int PDP_uintructions(bool addr_bit,bool mem_page,int offset);

private:

   //accumulator
   int AC;
   //function for finding the effective address
   int find_EAddr(bool addr_bit,bool mem_page,int offset);

   int AND_Count; //number of AND instruction
   int TAD_Count; //number of TAD instruction
   int ISZ_Count; //number of ISZ instruction 
   int DCA_Count; //number of DCA instruction
   int JMS_Count; //number of JMS instruction 
   int JMP_Count; //number of JMP instruction 
   int IO_Count;  //number of IO instruction 
   int uInstr_Count; //number of micro instruction 

};
