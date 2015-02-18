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
    
    //function for (OPCODE = 6) input/output
    void PDP_IO(int device_num,int opcode);
    //function for (OPCODE = 7) uinstructions
    void PDP_uintructions(bool group, bool cla, int offset);

   //function for displaying warning message with NOP
    void warningMessage();
   //function for rotating bits
   void rotateBits(int accumulator, int link, char dir);

//private:

   //accumulator
   int AC;
   //program counter
   int PC;
   //link bit
   bool link;

   //count for number of total instructions
   int sumInstr;
   //count for number of total clocks
   int sumClk;

   //count for instruction accesses
   int sumAND;
   int sumTAD; 
   int sumISZ; 
   int sumDCA; 
   int sumJMS;  
   int sumJMP;  
   int sumIO;   
   int sumuInstr;  

   //count for uInstructions (Group 1)
   int sumCLA;
   int sumCLL;
   int sumCMA;
   int sumCML;
   int sumIAC;
   int sumRAR;
   int sumRTR;
   int sumRAL;
   int sumRTL;
   int sumNOP;

   //count for uInstructions (Group 2)
   int sumSMA;
   int sumSZA;
   int sumSNL;
   int sumSPA;
   int sumSNA;
   int sumSZL;
   int sumSKP;
   int sumCLA;
   int sumOSR;
   int sumHLT;

   //function for finding the effective address
   int find_EAddr(bool addr_bit,bool mem_page,int offset);
   void increment_PC();
   bool read_bit_x(int input,int x);

   int MEM_LOAD(int dummy);
   void MEM_STORE(int dummy1,int dummy2);
   int temp;
private: //temp here, for testing

};
