//--------------------------------------------------------------------------------------
// Class to hold functions for implementing the PDP-8 Instructions
//--------------------------------------------------------------------------------------
#define REGISTERSIZE 12
#define IO_verbose 0

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
    void PDP_IO(int device_num,int opcode);
  
    void PDP_uintructions(bool group, bool cla, int offset);

//private:

   int AC;          //accumulator
   int PC;          //Program Counter
   bool link;       //link bit

   int AND_Count; //number of AND instruction
   int TAD_Count; //number of TAD instruction
   int ISZ_Count; //number of ISZ instruction 
   int DCA_Count; //number of DCA instruction
   int JMS_Count; //number of JMS instruction 
   int JMP_Count; //number of JMP instruction 
   int IO_Count;  //number of IO instruction 
   int uInstr_Count; //number of micro instruction 

   //function for finding the effective address
   int find_EAddr(bool addr_bit,bool mem_page,int offset);
   void increment_PC();
   bool read_bit_x(int input,int x);


   int MEM_LOAD(int dummy);
   void MEM_STORE(int dummy1,int dummy2);
   int temp;
private: //temp here, for testing

};
