#include <bitset>

void BitTwiddle::warningMessage()
{
    cout << "Warning. A NOP (No Operation) has been encountered.\n";
}

int BitTwiddle::rotateBits(int accumulator, int link, char direction){

    int lsb; //value of lsb from accumulator
    int msb; //value of msb from accumulator
    int templink; //temporarily holds value of link

    //cout << "Accumulator bits: " << (bitset<12>) accumulator << "\n";
    //cout << "Link bit: " << link << "\n"; 
    //cout << "Direction: " << direction << "\n\n";

    if(direction == 'R'){
        lsb = accumulator & 1; //save lsb of accumulator
      //cout << "Value of lsb is:" << lsb << "\n\n";

        accumulator = accumulator >> 1; //put accumulator to right by 1
      //cout << "Value of shifted accumulator: " << (bitset<12>)accumulator << "\n";
     
        templink = link; //save value of link

        link = lsb; //lsb will be shifted into the link
      //cout << "New value of link: " << link << "\n";

        templink = templink << 11; //put old value of link into accumulator MSB
        accumulator = (accumulator | templink); //put MSB into the accumulator
      //cout << "New value of accumulator: " << (bitset<12>)accumulator << "\n"; 

    }else if(direction == 'L'){ //rotate left

        msb = (accumulator>>11) & 1; //save msb of accumulator
      //cout << "Value of msb is:" << msb << "\n\n";

        accumulator = accumulator << 1; //shift accumulator to left by 1
      //cout << "Value of shifted accumulator: " << (bitset<12>)accumulator << "\n";
     
        templink = link; //save value of link
      //cout << "Value of templink: " << templink << "\n";

        link = msb; //lsb will be shifted into the link
      //cout << "New value of link: " << link << "\n";

        accumulator = (accumulator | templink); //put MSB into the accumulator
      //cout << "New value of accumulator: " << (bitset<12>)accumulator << "\n"; 

    }else{

        cout << "Error in reading direction of rotation.\n";
    }
}

void BitTwiddle::PDP_uintructions(bool bit3, bool bit4, int offset){


    bool bit5  = read_bit_x(offset, 5);
    bool bit6  = read_bit_x(offset, 6);
    bool bit7  = read_bit_x(offset, 7);
    bool bit8  = read_bit_x(offset, 8);
    bool bit9  = read_bit_x(offset, 9);
    bool bit10 = read_bit_x(offset,10);
    bool bit11 = read_bit_x(offset,11);


    //-----------------------------uinstructions (GROUP 1)---------------------------

    if(!bit3){

        if(bit4){         //clear accumulator

            sumCLA++;
            AC = 0; 

        }else if(bit5){   //clear link

            sumCLL++; 
            link = 0;


        }else if(bit6){   //complement accumulator

            sumCMA++;
            AC = ~AC;

        }else if(bit7){   //complement link

            sumCML++;
            link = ~link;

        }else if(bit11){  //increment accumulator

            sumIAC++;
            AC = AC++;

        }else if(bit8){   //rotate accumulator, link right

           sumRAR++;
           char direction = 'R';
           rotateBits(AC, link, direction);

           if(bit10){ //rotate accumulator, link right twice (rotate once more)

            sum RTR++;
            sumRAR--;
            rotateBits(AC, link, direction);

           }

        }else if(bit9){   //rotate accumulator, link left

           sumRAL++;
           char direction = 'L';
           rotateBits(AC, link, direction);

           if(bit10){ //rotate accumulator, link left twice (rotate once more)

           sumRTL++;
           sumRAL--;
           rotateBits(AC, link, direction);

           }

        }else{            //no operation

            sumNOP++;
            warningMessage();
        }

    //-----------------------------uinstructions (GROUP 2)---------------------------

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
                cond6=(AC!=0)//is AC not 0?
            }
            if(bit7){                   //SZL
                cond7=!link;//is link 0?
            }
            if(cond5 && cond6 && cond57){
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
        //    while(1);
        }
    }else{                              //group 3 uinstructions


    }
    return;

}

