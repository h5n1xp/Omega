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
#include "Floppy.h"
#include "m68k.h"
#include "Copper.h"
#include "Bitplane.h"
#include "Denise.h"


int EclockCountdown = -1;

/*
typedef struct{
    uint16_t SPRPOS;
    uint16_t SPRCTL;
    uint32_t Data[4096];
} Sprite_t;
*/


void SpriteP2C1(uint8_t* buffer, uint16_t source){
    
    source = source << 8 | source >> 8;
    
    for(int i = 15; i >= 0; --i){
        buffer[i] = source & 1;
        source = source >> 1;
    }
    
    
}

void SpriteP2C2(uint8_t* buffer, uint16_t source){
    
    source = source << 8 | source >> 8;
    
    for(int i = 15; i >= 0; --i){
        buffer[i] = (buffer[i] & 0xFD) | (source & 1) << 1;
        source = source >> 1;
    }

}

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
        FloppyCycle();
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
    
    uint16_t AUD0LEN = ChipsetState->chipram[0xDC00A4];
    if(AUD0LEN == 0){
        ChipsetState->WriteWord[0x9C](0x8080); //Generate an AUD0 interupt
    }
    
    //DMA disabled
    if( !(ChipsetState->DMACONR & 0x200)){
        return;
    }
    
    //Audio
    if( ChipsetState->DMACONR & 0xF){
       // printf("bugger");
        
    }
    


    
    //Floppy Drive
    if( ChipsetState->DMACONR & 0x10){
        FloppyExecute(ChipsetState);
    }
    
    //Blitter
    if( ChipsetState->DMACONR & 0x40){  //DMA on flag
        if(ChipsetState->DMACONR & 0x4000){ //Blitter busy Flag
            blitter_execute(ChipsetState);
        }
    }
    
    //Copper
    if( ChipsetState->DMACONR & 0x80){
            ExecuteCopper(ChipsetState);
    }
    
    
    
    
    //Bitplanes
    if( ChipsetState->DMACONR & 0x100){
        
        if(ChipsetState->hires){
            BitplaneExecuteHires(ChipsetState);
        }else{
            BitplaneExecuteLores(ChipsetState);
        }
        
    }

    
    //Sprites are drawn after the bitplane fetches have completed
    if( (ChipsetState->DMACONR & 0x20) && ((ChipsetState->VHPOS & 0xFF) == 0xDF)){
    
        uint32_t PATHAddress = 0xDFF120;
        //uint32_t CTRLAddress = 0xDFF144;

        //check each sprite in turn Currently ONLY SPRITE 0 is supported
        for(int i = 0; i < 1;++i){
            

            // We need to display a line of a sprite
            if(ChipsetState->sprite[i].VPOS == (ChipsetState->VHPOS & 0x1FF00)){
                uint32_t* data   = (uint32_t*) &ChipsetState->chipram[PATHAddress];
                uint16_t* datst = (uint16_t*) &ChipsetState->chipram[*data];
        
                //work out the x postiion of the sprite
                int x = ((datst[0] & 0xFF00) >> 7) | ((datst[1] & 0x100) >> 8);
                
                //Safety value
                if(x<96){
                    x = 98;
                }
                
                
                //Calcuate the index into the famebuffer
                uint32_t index =  ((ChipsetState->sprite[i].VPOS >> 8) * 800) + (x << 1) + 2;
                //printf("Sprite x:%d\n",x);
                
 
                
                //Planar To Chunky conversion
                uint8_t buffer[16];
    
                SpriteP2C1(buffer, *(ChipsetState->sprite[i].data++));
                SpriteP2C2(buffer, *(ChipsetState->sprite[i].data++));
                
                //Draw sprite
                for(int j=0;j<16; ++j){
                    

                    if(buffer[j] !=0){
                        int colour = ChipsetState->Colour[buffer[j]+16];
                        ChipsetState->frameBuffer[index + (j*2)  ] = colour;
                        ChipsetState->frameBuffer[index + (j*2)+1] = colour;
                    }
                }

                ChipsetState->sprite[0].VPOS += 256;
                
                if(ChipsetState->sprite[0].VPOS >= ChipsetState->sprite[0].stop){
                    ChipsetState->sprite[0].VPOS= 79874;// An unreachable value
                }
                
            }
            PATHAddress += 4;
            //CTRLAddress += 8;
        }
        
    }
        
   // DeniseExecute(ChipsetState);
   // DeniseExecute(ChipsetState);
}

/*
 The Host display starts drawing at H position 96, this is where the Framebuffer index starts counting
 The Host display index needs to start a newline at position 224
 
 Each position is 4 pixels, so the index increases by 4
 From the DFFSTRT value the display needs to update every 4 DMA cycles (write 16 pixels to the screen) and advance the
 plane counter by 1, in lowres mode the plane counter needs to advance every 8 DMA cycles.
 
 
 
 */

