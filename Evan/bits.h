
class BitTwiddle {
public:
BitTwiddle();
//~BitTwiddle();

void PDP_AND(bool addr_bit,bool mem_page,int offset);
void PDP_TAD(bool addr_bit,bool mem_page,int offset);
void PDP_ISZ(bool addr_bit,bool mem_page,int offset);

//void PDP_DCA(bool addr_bit,bool mem_page,int offset);
//void PDP_JMS(bool addr_bit,bool mem_page,int offset);
//void PDP_JMP(bool addr_bit,bool mem_page,int offset);

//TODO IO and uinstructions have specal inputs, not the normal ones.
//int PDP_IO(bool addr_bit,bool mem_page,int offset);
//int PDP_uintructions(bool addr_bit,bool mem_page,int offset);

private:
int AC;
int find_EAddr(bool addr_bit,bool mem_page,int offset);
};
