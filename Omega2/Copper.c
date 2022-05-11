//
//  Copper.c
//  Omega2
//
//  Created by Matt Parsons on 28/04/2022.
//

#include "Copper.h"

uint16_t swap(uint16_t* p){
    return (*p >> 8) | (*p << 8);
}

void ExecuteCopper(Chipset_t* ChipsetState){
    
    // Cycle 1: Load IR1
    // Cycle 2: Load IR2
    //          Decode IR1
    // Cycle 3: Write Back
    
    
   // if(ChipsetState->CopperPC == 0 || ChipsetState->CopperPC > 2097151){
       //ChipsetState->CopperState = 4;
  //  }
    
    
    uint32_t VHPOS = ChipsetState->VHPOS & 0xFFFF; //ignore the top bit
    switch(ChipsetState->CopperState){
            
        case 0:
            //Load IR1
            ChipsetState->CopperIR1 = swap((uint16_t*)&ChipsetState->chipram[ChipsetState->CopperPC]);
            ChipsetState->CopperPC += 2;
            
            
            if( ChipsetState->CopperIR1 & 1){
                ChipsetState->CopperState = 1;  // Wait or Skip
            }else{
                ChipsetState->CopperState = 2; // it's a Move instruction
            }
            break;
            
            
            
        case 1:
            //Load IR2
            ChipsetState->CopperIR2 = swap((uint16_t*)&ChipsetState->chipram[ChipsetState->CopperPC]);
            ChipsetState->CopperPC += 2;
            
            
            //Decode IR1
                ChipsetState->CopperIR1 = ChipsetState->CopperIR1 & ChipsetState->CopperIR2 ; // Mask comparison value and remove IR2 bit 0
                
                //Kludge to trap too short a waiting time, my copper runs too fast due to lack of real DMA contention
            if( ChipsetState->CopperIR1 > 0xFF00){
                if(ChipsetState->CopperIR1 < 0xFF51){
                    ChipsetState->CopperIR1 = 0xFFDF;
                }
            }
            
                //This is a skip instruction
                if(ChipsetState->CopperIR2 & 1){
                    
                    if(VHPOS >= ChipsetState->CopperIR1 ){
                        ChipsetState->CopperPC += 4;
                    }
                    ChipsetState->CopperState = 0;
                    break;
                }
                
                
                
                //This is a wait Instruction, Do we need to wait?
                if( VHPOS >= ChipsetState->CopperIR1 ){
                    ChipsetState->CopperState = 0;
                    break;
                }
                
                //Jump to the wait state
                ChipsetState->CopperState = 3;
                break;
        
            
            
            
        case 2:
            //Load IR2
            ChipsetState->CopperIR2 = swap((uint16_t*)&ChipsetState->chipram[ChipsetState->CopperPC]);
            ChipsetState->CopperPC += 2;
            
            if( (ChipsetState->CopperIR1 & 511) < 0x40){    //Assumes Copper Dangerous bit is alwasy set.
                ChipsetState->CopperState = 4; // Illegal Reg write halts copper
                return;
            }
    
            ChipsetState->WriteWord[ChipsetState->CopperIR1 & 511](ChipsetState->CopperIR2);
            ChipsetState->CopperState = 0;
            break;
            
            
            
            
        case 3:
            //The Wait state
            if( VHPOS >= ChipsetState->CopperIR1){
                ChipsetState->CopperState = 0;
            }
            break;
            
        case 4:
            //HALT until next VBL where COP1LOC will be reloaded.
            break;
            
    }
    
}
