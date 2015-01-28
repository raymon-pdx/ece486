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


