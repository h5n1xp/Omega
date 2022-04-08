//
//  DMA.c
//  Omega2
//
//  Created by Matt Parsons on 29/03/2022.
//

#include "DMA.h"
#include <stdint.h>

#include "Chipset.h"
#include "Blitter.h"
#include "CIA.h"

uint16_t swap(uint16_t* p){
    return (*p >> 8) | (*p << 8);
}

void P2C1(uint8_t* buffer, uint16_t source){
    
    source = source << 8 | source >> 8;
    
    for(int i = 15; i >= 0; --i){
        buffer[i] = source & 1;
        source = source >> 1;
    }
    
    
}

void P2C2(uint8_t* buffer, uint16_t source){
    
    source = source << 8 | source >> 8;
    
    for(int i = 15; i >= 0; --i){
        buffer[i] = (buffer[i] & 0xFD) | (source & 1) << 1;
        source = source >> 1;
    }

}

void P2C3(uint8_t* buffer,uint16_t source){
    
    source = source << 8 | source >> 8;
    
    for(int i = 15; i >= 0; --i){
        buffer[i] = (buffer[i] & 0xFB) | (source & 1) << 2;
        source = source >> 1;
    }

}

void P2C4(uint8_t* buffer,uint16_t source){
        
    source = source << 8 | source >> 8;
    
    for(int i = 15; i >= 0; --i){
        buffer[i] = (buffer[i] & 0xF7) | (source & 1) << 3;
        source = source >> 1;
    }
}

void P2C5(uint8_t* buffer,uint16_t source){
            
    source = source << 8 | source >> 8;
    
    for(int i = 15; i >= 0; --i){
        buffer[i] = (buffer[i] & 0xEF) | (source & 1) << 4;
        source = source >> 1;
    }

}

//char copperIns[100];

void ExecuteCopper(Chipset_t* ChipsetState){
    
    // Cycle 1: Load IR1
    // Cycle 2: Load IR2
    //          Decode IR1
    // Cycle 3: Write Back
    
    if(ChipsetState->CopperPC == 0){
        return;
    }
        
    uint32_t VHPOS = ChipsetState->VHPOS & 0xFFFF; //ignore the top bit
    switch(ChipsetState->CopperState){
            
        case 0:
            //Load IR1
            ChipsetState->CopperIR1 = swap((uint16_t*)&ChipsetState->chipram[ChipsetState->CopperPC]);
            ChipsetState->CopperPC += 2;
            ChipsetState->CopperState = 1;
            break;
            
            
            
        case 1:
            //Load IR2
            ChipsetState->CopperIR2 = swap((uint16_t*)&ChipsetState->chipram[ChipsetState->CopperPC]);
            ChipsetState->CopperPC += 2;
            
            
            //Decode IR1
            if( ChipsetState->CopperIR1 & 1){

                ChipsetState->CopperIR1 = ChipsetState->CopperIR1 & ChipsetState->CopperIR2 ; // Mask comparion value and remove IR2 bit 0
                
                //This is a skip instruction
                if(ChipsetState->CopperIR2 & 1){
                    
                    
                    if(VHPOS >= ChipsetState->CopperIR1 ){
                        ChipsetState->CopperPC += 4;
                    }
                    ChipsetState->CopperState = 0;
                    break;
                }
                
                
                
                //This is a wait Instruction
                
                if( VHPOS >= ChipsetState->CopperIR1 ){
                    ChipsetState->CopperState = 0;
                    break;
                }
                
                ChipsetState->CopperState = 3;
                break;
            }
            
            //It's a Move instruction so advance to the Move state
            ChipsetState->CopperState = 2;
            
            //printf("Move 0x%6x -> (0x%x)\n",ChipsetState->CopperIR2,0xDFF000+ChipsetState->CopperIR1);
            //sprintf(copperIns, "Move 0x%x -> (0x%x)\n",ChipsetState->CopperIR2,0xDFF000+ChipsetState->CopperIR1);
            
            break;
            
            
            
        case 2:
            //The Move Instruction
            
            if( (ChipsetState->CopperIR1 & 511) < 0x20){
                ChipsetState->CopperState = 0;
                return;
            }
            
            ChipsetState->WriteWord[ChipsetState->CopperIR1 & 511](ChipsetState->CopperIR2);
            ChipsetState->CopperState = 0;
            break;
            
            
            
            
        case 3:
            //The Wait Instruction
            
            if( VHPOS >= ChipsetState->CopperIR1){
                ChipsetState->CopperState = 0;
            }
                        
            break;
            
    }
    
}

int EclockCountdown = -1;

void DMAExecute(void* address, uint32_t* framebuffer){
    
    
    Chipset_t* ChipsetState = address;
    ChipsetState->DMACycles += 1;
    
    //The CPU runs at 7.15909Mhz
    //The DMA runs at 3.579545 Mhz
    //The EClock runs at 0.715909Mhz
    //So the Eclock advances exactly every 5 DMA cycles
    // the execution ratio of CPU:DMA:CIA is 10:5:1
   
    /*
     //Old, Pre DMA cycle counter EClock
    EclockCountdown -= 1;
    if(EclockCountdown < 0){

        EclockCountdown = 4;
        RunCIACycle();
        ChipsetState->EClockcycles += 1;
    }
    */
    
    
    if((ChipsetState->DMACycles % 5) == 0){
        RunCIACycle();
        ChipsetState->EClockcycles += 1;
    }
    
    /*
    //RTC Clock TESt
    if( (ChipsetState->DMACycles % ChipsetState->DMAFreq) == 0 ){
        ChipsetState->chipram[0xDC0003] += 1;// Seconds...
        
        if( ChipsetState->chipram[0xDC0003] == 42){
            ChipsetState->chipram[0xDC0003] = 32; // Seconds...
            ChipsetState->chipram[0xDC0007] += 1;
            //ChipsetState->chipram[0xDC000b] += 2;
            //ChipsetState->chipram[0xDC000f] += 3;
        }
        
    }
    */
    
    IncrementVHPOS(); //Increment the Display beam position
    

    //DMA disabled
    if( !(ChipsetState->DMACONR & 0x200)){
        return;
    }
    
    if( ChipsetState->DMACONR & 0x10){
        //printf("Run Floppy Disk\n");
    }
    
    if( ChipsetState->DMACONR & 0x40){
        if(ChipsetState->DMACONR & 0x4000){
            blitter_execute(ChipsetState);
        }
    }
    
    if( ChipsetState->DMACONR & 0x80){
        ExecuteCopper(ChipsetState);
    }
    
    if( ChipsetState->DMACONR & 0x100){
        uint16_t* DFFSTRT = (uint16_t*) &ChipsetState->chipram[0xDFF092];
        uint16_t* DFFSTOP = (uint16_t*) &ChipsetState->chipram[0xDFF094];
        uint16_t* DIWSTRT = (uint16_t*) &ChipsetState->chipram[0xDFF08E];
        
        
        
        //Check the vertical position of the beam
        if( (ChipsetState->VHPOS & 0xFF00) == (*DIWSTRT & 0xFF00) ){
            ChipsetState->bitplaneFetchActive = 1;
        }
        
        if( (ChipsetState->VHPOS & 0x1FF00) == (ChipsetState->VSTOP) ){
            ChipsetState->bitplaneFetchActive = 0;
        }
        
        if(ChipsetState->bitplaneFetchActive == 0){
            return;
        }
        
        
        
        
        // Check the horizontal position of the beam
        ChipsetState->bitplaneFetchCountdown -= 1;
        
        if( (ChipsetState->VHPOS & 0xFF) == *DFFSTRT){
            ChipsetState->bitplaneFetchCountdown = 0; // Start Countdown
        }
        
        if( (ChipsetState->VHPOS & 0xFF) == *DFFSTOP + 8){
            ChipsetState->bitplaneFetchCountdown = -1; // countdown stop...
            
            uint16_t* BPLMOD1 = (uint16_t*)&ChipsetState->chipram[0xDFF108];
            
            //need to add the Bitplane modulus if there is one.
            if(*BPLMOD1 !=0){
                
                uint32_t* BP;
                uint16_t* Data;
            
                //Bitplane1
                BP  = (uint32_t*) &ChipsetState->chipram[0xDFF0E0];  // get BLP1PT register
                Data = (uint16_t*)&ChipsetState->chipram[*BP];     // Get the data pointed to by register
                *BP += *BPLMOD1;
                
                //Bitplane2
                BP  = (uint32_t*) &ChipsetState->chipram[0xDFF0E4];  // get BLP1PT register
                Data = (uint16_t*)&ChipsetState->chipram[*BP];     // Get the data pointed to by register
                *BP += *BPLMOD1;
                
                //Bitplane3
                BP  = (uint32_t*) &ChipsetState->chipram[0xDFF0E8];  // get BLP1PT register
                Data = (uint16_t*)&ChipsetState->chipram[*BP];     // Get the data pointed to by register
                *BP += *BPLMOD1;
                
                //Bitplane4
                BP  = (uint32_t*) &ChipsetState->chipram[0xDFF0EC];  // get BLP1PT register
                Data = (uint16_t*)&ChipsetState->chipram[*BP];     // Get the data pointed to by register
                *BP += *BPLMOD1;
                
                //Bitplane5
                BP  = (uint32_t*) &ChipsetState->chipram[0xDFF0F0];  // get BLP1PT register
                Data = (uint16_t*)&ChipsetState->chipram[*BP];     // Get the data pointed to by register
                *BP += *BPLMOD1;
                
                
            }
            
        }
        
        
        
        
        
        //Draw display
        uint16_t* BPLCON0 = (uint16_t*) &ChipsetState->chipram[0xDFF100];
        uint32_t hires = *BPLCON0 >> 15;
        uint32_t planeCount = (*BPLCON0 >> 12) & 0x7;
        
        if(hires){
            if(ChipsetState->bitplaneFetchCountdown == 0){
                ChipsetState->bitplaneFetchCountdown = 4; // Hires countdown... lowres would be 8
            
                uint8_t buffer[16]; // Bitplane buffer
                uint32_t* BP;
                uint16_t* Data;
            
                if(planeCount > 0){
                    //Bitplane1
                    BP  = (uint32_t*) &ChipsetState->chipram[0xDFF0E0];  // get BLP1PT register
                    Data = (uint16_t*)&ChipsetState->chipram[*BP];     // Get the data pointed to by register
                    *BP += 2;                                                       // increment register
                    P2C1(buffer, *Data);    //Planar to Chunky
                }
            
                if(planeCount > 1){
                //Bitplane2
                    BP  = (uint32_t*) &ChipsetState->chipram[0xDFF0E4];  // get BLP1PT register
                    Data = (uint16_t*)&ChipsetState->chipram[*BP];     // Get the data pointed to by register
                    *BP += 2;                                                       // increment register
                    P2C2(buffer, *Data);    //Planar to Chunky
                }
            
                if(planeCount > 2){
                    //Bitplane3
                    BP  = (uint32_t*) &ChipsetState->chipram[0xDFF0E8];  // get BLP1PT register
                    Data = (uint16_t*)&ChipsetState->chipram[*BP];     // Get the data pointed to by register
                    *BP += 2;                                                       // increment register
                    P2C3(buffer, *Data);    //Planar to Chunky
                }
            
                //Flush buffer to screen
                
                int up = ((*DIWSTRT & 0xFF00) >> 9) * 800; // a few calculations to centre the image on the framebuffer
                int centre = *DFFSTRT + up;
                
                for(int i = 0; i < 16; ++i){
                    ChipsetState->frameBuffer[ChipsetState->framebufferIndex+i-centre] = ChipsetState->Colour[buffer[i]];
                }
                ChipsetState->needsRedraw = 1;
            }
        }else{

            if(ChipsetState->bitplaneFetchCountdown == 0){
                ChipsetState->bitplaneFetchCountdown = 8;
            
                uint8_t buffer[16]; // Bitplane buffer
                uint32_t* BP;
                uint16_t* Data;
            
                if(planeCount > 0){
                    //Bitplane1
                    BP  = (uint32_t*) &ChipsetState->chipram[0xDFF0E0];  // get BLP1PT register
                    Data = (uint16_t*)&ChipsetState->chipram[*BP];     // Get the data pointed to by register
                    *BP += 2;                                                       // increment register
                    P2C1(buffer, *Data);    //Planar to Chunky
                }
            
                if(planeCount > 1){
                //Bitplane2
                    BP  = (uint32_t*) &ChipsetState->chipram[0xDFF0E4];  // get BLP1PT register
                    Data = (uint16_t*)&ChipsetState->chipram[*BP];     // Get the data pointed to by register
                    *BP += 2;                                                       // increment register
                    P2C2(buffer, *Data);    //Planar to Chunky
                }
            
                if(planeCount > 2){
                    //Bitplane3
                    BP  = (uint32_t*) &ChipsetState->chipram[0xDFF0E8];  // get BLP1PT register
                    Data = (uint16_t*)&ChipsetState->chipram[*BP];     // Get the data pointed to by register
                    *BP += 2;                                                       // increment register
                    P2C3(buffer, *Data);    //Planar to Chunky
                }
            
                if(planeCount > 3){
                    //Bitplane4
                    BP  = (uint32_t*) &ChipsetState->chipram[0xDFF0EC];  // get BLP1PT register
                    Data = (uint16_t*)&ChipsetState->chipram[*BP];     // Get the data pointed to by register
                    *BP += 2;                                                       // increment register
                    P2C4(buffer, *Data);    //Planar to Chunky
                }
                
                if(planeCount > 4){
                    //Bitplane5
                    BP  = (uint32_t*) &ChipsetState->chipram[0xDFF0F0];  // get BLP1PT register
                    Data = (uint16_t*)&ChipsetState->chipram[*BP];     // Get the data pointed to by register
                    *BP += 2;                                                       // increment register
                    P2C5(buffer, *Data);    //Planar to Chunky
                }
                
                //Flush buffer to screen
                
                int up = ((*DIWSTRT & 0xFF00) >> 9) * 800;  // a few calculations to centre the image on the framebuffer
                int centre = *DFFSTRT + up;
                
                for(int i = 0; i < 16; i += 1){
                    ChipsetState->frameBuffer[ChipsetState->framebufferIndex+(i*2)-centre] = ChipsetState->Colour[buffer[i]];
                    ChipsetState->frameBuffer[ChipsetState->framebufferIndex+(i*2)+1 - centre] = ChipsetState->Colour[buffer[i]];
                }
                ChipsetState->needsRedraw = 1;
            }
        }
        
        
        
    }
    
}

/*
 The Host display starts drawing at H position 96, this is where the Framebuffer index starts counting
 The Host display index needs to start a newline at position 224
 
 Each position is 4 pixels, so the index increases by 4
 From the DFFSTRT value the display needs to update every 4 DMA cycles (write 16 pixels to the screen) and advance the
 plane counter by 1, in lowres mode the plane counter needs to advance every 8 DMA cycles.
 
 
 
 */

