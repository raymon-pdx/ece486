//--------------------------------------------------------------------------------------
// Class constructor
//--------------------------------------------------------------------------------------
BitTwiddle::BitTwiddle(void)
{

  //TODO: Add these to private class of BitTwiddle
  //TODO: Add these to each function
  //count for sum of instruction accesses
  AND_Count = 0; //number of times AND instruction was executed
  TAD_Count = 0; //number of times TAD instruction was executed
  ISZ_Count = 0; //number of times ISZ instruction was executed
  DCA_Count = 0; //number of times DCA instruction was executed
  JMS_Count = 0; //number of times JMS instruction was executed
  JMP_Count = 0; //number of times JMP instruction was executed
  IO_Count = 0;  //number of times IO instruction was executed
  uInstr_Count = 0; //number of times uinstruction was executed

    //count for sum of access into uInstructions (Group 1)
    //TODO: Implement in function = DONE!
    //TODO: Add to .h file = NOPE!
    int sumCLA = 0;
    int sumCLL = 0;
    int sumCMA = 0;
    int sumCML = 0;
    int sumIAC = 0;
    int sumRAR = 0;
    int sumRTR = 0;
    int sumRAL = 0;
    int sumRTL = 0;
    int sumNOP = 0;


    //count for sum of access into uInstructions (Group 2)
    //TODO: Implement in function = NOPE!
    //TODO: Add to .h file = NOPE!

    int sumSMA = 0;
    int sumSZA = 0;
    int sumSNL = 0;
    int sumSPA = 0;
    int sumSNA = 0;
    int sumSZL = 0;
    int sumSKP = 0;
    int sumCLA = 0;
    int sumOSR = 0;
    int sumHLT = 0;

}

//--------------------------------------------------------------------------------------
// Class destructor
//--------------------------------------------------------------------------------------
BitTwiddle::~BitTwiddlee(void)
{

    //print out brief summary 
    cout << "-----------PDP-8 ISA Simulation Summary---------------\n\n";
    //TODO: Add the +1 to numInstr for every instruction
    cout << "Total number of Instructions executed: " << numInstr << "\n";
    //TODO: Add correct amount of clock cycles to each function
    cout << "Total number of clock cycles consumed: " << numClk << "\n\n";
    cout << "**Number of times each instruction type was executed**\n"
    //TODO: Add variable to each function to hold count
    cout << "|-----------------------------------------------------\n";
    cout << "|    Mnemonic   | Number of times executed            \n";
    cout << "|-----------------------------------------------------\n";
    cout << "|      AND      |       " << AND_Count << "\n";
    cout << "|      TAD      |       " << TAD_Count << "\n";
    cout << "|      ISZ      |       " << ISZ_Count << "\n";
    cout << "|      DCA      |       " << DCA_Count << "\n";
    cout << "|      JMS      |       " << JMS_Count << "\n";
    cout << "|      JMP      |       " << JMP_Count << "\n";
    cout << "|     <IO>      |       " << IO_Count << "\n";
    cout << "| uInstructions |       " << uInstr_Count << "\n";
    cout << "------------------------------------------------------\n";

   //TODO: Implemen accesses to various uInstructions

  }

