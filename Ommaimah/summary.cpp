//--------------------------------------------------------------------------------------
// Class constructor
//--------------------------------------------------------------------------------------
BitTwiddle::BitTwiddle(void)
{

  //TODO: Add these to private class of BitTwiddle
  //TODO: Add these to each function
  AND_Count = 0; //number of times AND instruction was executed
  TAD_Count = 0; //number of times TAD instruction was executed
  ISZ_Count = 0; //number of times ISZ instruction was executed
  DCA_Count = 0; //number of times DCA instruction was executed
  JMS_Count = 0; //number of times JMS instruction was executed
  JMP_Count = 0; //number of times JMP instruction was executed
  IO_Count = 0;  //number of times IO instruction was executed
  uInstr_Count = 0; //number of times micro instruction was executed

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

  }

