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

