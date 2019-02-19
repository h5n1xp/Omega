//
//  DMA.c
//  The Omega Project
//  https://github.com/h5n1xp/Omega
//
//  Created by Matt Parsons on 02/02/2019.
//  Copyright © 2019 Matt Pasons. All rights reserved.
//  <h5n1xp@gmail.com>
//
//
//  This Source Code Form is subject to the terms of the
//  Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "DMA.h"
#include "Memory.h"
#include "Chipset.h"
#include "CIA.h"
#include "Host.h"

#include "CPU.h"


#include "debug.h"

#include "Floppy.h"













































































void (*DMAOperation[]) (void) = {
    evenCycle,
    dramCycle,
    evenCycle,
    dramCycle,
    evenCycle,
    dramCycle,
    evenCycle,
    diskCycle,
    evenCycle,
    diskCycle,
    evenCycle,
    diskCycle,
    evenCycle,
    audioCycle,
    evenCycle,
    audioCycle,
    evenCycle,
    audioCycle,
    evenCycle,
    audioCycle,
    evenCycle,
    spriteCycle,    //00
    evenCycle,
    spriteCycle,    //00
    evenCycle,
    spriteCycle,    //01
    evenCycle,
    spriteCycle,    //01
    evenCycle,
    spriteCycle,    //02
    evenCycle,
    spriteCycle,    //02
    evenCycle,
    spriteCycle,    //03
    evenCycle,
    spriteCycle,    //03
    evenCycle,
    spriteCycle,    //04
    evenCycle,
    spriteCycle,    //04
    evenCycle,
    spriteCycle,    //05
    evenCycle,
    spriteCycle,    //05
    evenCycle,
    spriteCycle,    //06
    evenCycle,
    spriteCycle,    //06
    evenCycle,
    spriteCycle,    //07
    evenCycle,
    spriteCycle,    //07
    evenCycle,
    oddCycle,
    evenCycle,
    oddCycle,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    bitplaneCycle0_4,
    bitplaneCycle4_2,
    bitplaneCycle6_3,
    bitplaneCycle2_1,
    bitplaneCycle0_4,
    bitplaneCycle3_2,
    bitplaneCycle5_3,
    bitplaneCycle1,
    evenCycle,
    oddCycle,
    evenCycle,
    oddCycle,
    evenCycle,
    oddCycle,
    evenCycle,
    oddCycle,
    evenCycle,
    oddCycle,

};



void dma_execute(){
  
    chipset.vposr   = internal.LOF | internal.vPos >> 8;
    chipset.vhposr  = internal.vPos << 8;
    chipset.vhposr |= internal.hPos;
   
    
    DMAOperation[internal.hPos]();
    eclock_execute(&chipset);   // CIA timers

    internal.hPos++;
    
    
    //end of line reached! 227 colour clocks have executed
    if(internal.hPos >0xE3){
        
        internal.hPos = 0;
        internal.vPos +=1;
        CIATODEvent(&CIAB);
        
        //Update bitplane modulo
        chipset.bpl1pt += chipset.bpl1mod;
        chipset.bpl3pt += chipset.bpl1mod;
        chipset.bpl5pt += chipset.bpl1mod;
        chipset.bpl7pt += chipset.bpl1mod;
        chipset.bpl2pt += chipset.bpl2mod;
        chipset.bpl4pt += chipset.bpl2mod;
        chipset.bpl6pt += chipset.bpl2mod;
        chipset.bpl8pt += chipset.bpl2mod;
        
        //VBL Time
        if(internal.vPos > 0x106 ){ //0x106 is the propper ntsc vbl
            internal.vPos = 0;
            
            
            putChipReg16[COPJMP1](0);
            CIATODEvent(&CIAA);
            
            
            //need to generate a vbl int
            putChipReg16[INTREQ](0x8020);
            
            host.FBCounter = 0; // restart the frambuffer pointer
            
            
            
            //Draw sprites
            if( (chipset.dmaconr & 0x220)== 0x220){
                //Software renderer - draw sprite to the frame buffer
                
                
                if(chipset.spr0pt !=0){
                    uint16_t* pos = &internal.chipramW[chipset.spr0pt];
                    int mod = 0;
                    
                    int Nx=(pos[0] & 0xFF00) >> 7;
                    Nx |=( (pos[1] >> 8) & 1);
                    Nx = (Nx * 2)-254;
                    int Ny= ((pos[0] & 255))-43;
                    
                    
                    for(int i=2;i<32; i +=2){
                        sprite2chunky(&host.pixels[mod+(Ny*640*4)], &internal.palette[16], Nx, pos[i], pos[i+1],16);
                        mod +=640*4;
                    }
                    
                }
            }
            
            
            
            hostDisplay(&chipset);
        }
        
    }
}


void evenCycle(void){

    
    if(copperExecute()==1){
        return;
    }
    
    //if the copper doesn't want the even cycle, give it to the odd cycle devices.
    oddCycle();

}

void oddCycle(void){
    
    if(blitterExecute()==1){
        return;
    }
    
    //A Free slot for CPU... but the CPU isn't currently bound to the DMA timing
    
}

void dramCycle(void){

}

void diskCycle(void){
    
    static int count=0;

    
    if( (chipset.dmaconr & 0x210) && chipset.dsklen & 0x8000 ){
        
        if(chipset.dsklen & 0x3FFF){
            uint8_t byte = floppyDataRead(&df0);
            low16Meg[chipset.dskpt] = byte;
            chipset.dskpt +=1;
            byte = floppyDataRead(&df0);
            low16Meg[chipset.dskpt] = byte;
            chipset.dskpt +=1;
        
            chipset.dsklen -= 1;
        
            count +=1;
            
            if( (chipset.dsklen & 0x3FFF) == 0){
                    putChipReg16[INTREQ](0x8002);
                
                /*
                //Debug should point to the memory where the track was written from disk
                int originaldskpt = chipset.dskpt-(count*2);  //debugging
                int deCount = count;//debugging
                printf("Transfered %d words from track %d (head %d) to 0x%06x\n",count,df0.diskTrack,1 - df0.diskSide,chipset.dskpt-(count*2));
                */
                
                count = 0;
            }
        }
    }
}
void audioCycle(void){

}
void spriteCycle(void){

}

int bitplaneActive(){
    
    //check if DMA is on, if not let the Copper and Blitter run.
    if((chipset.dmaconr & 0x300) != 0x300){
        return 0;
    }
    
    //too early horisonal position let the Copper and Blitter run
    if(internal.hPos<chipset.ddfstrt){
        return 0;
    }
 
    //too late horisonal position let the Copper and Blitter run
    if(internal.hPos>(chipset.ddfstop+8)){
        return 0;
    }
    
    //too early vertical position let the Copper and Blitter run
    if(internal.vPos<(chipset.diwstrt>>8)){
        return 0;
    }
    
    // vpos not working...
    if(internal.vPos> ((chipset.diwstop>>8)| 256)){
    //    return 0;
    }
    
    return 1;
}

void bitplaneCycle0_4(void){
    
    if(bitplaneActive()==0){
        evenCycle(); // let the copper run
        return;
    }
    

    
}
void bitplaneCycle4_2(void){
    
    if(bitplaneActive()==0){
        return;
    }
    
    if(chipset.bplcon0 & 0x8000){
        
        if( (internal.bitplaneMask & 0x2)  == 0x2){
        
            uint16_t* p = &internal.chipramW[chipset.bpl2pt];
            chipset.bpl2pt +=1;
            chipset.bpl2dat = *p;
            return;
        }
    }
    
    /*
    uint16_t* p = &internal.chipramW[chipset.bpl4pt];
    chipset.bpl4pt +=1;
    chipset.bpl4dat = *p;
    */
}
void bitplaneCycle6_3(void){
    
    if(bitplaneActive()==0){
        evenCycle(); // let the copper run
        return;
    }
    

    
}
void bitplaneCycle2_1(void){
    
    if(bitplaneActive()==0){
        return;
    }
    
    if(chipset.bplcon0 & 0x8000){
        
       if( (internal.bitplaneMask & 0x1)  == 0x1){
            uint16_t* p = &internal.chipramW[chipset.bpl1pt];
            chipset.bpl1pt +=1;
            chipset.bpl1dat = *p;
        
        
            if(host.pixels == NULL){
                return;
            }
        
            uint32_t* pixbuff = (uint32_t*)host.pixels;
            planar2chunky(&pixbuff[host.FBCounter], internal.palette, chipset.bpl1dat, chipset.bpl2dat, 0, 0, 0, 8);
            return;
       }
    }
    
    if( (internal.bitplaneMask & 0x2)  == 0x2){
        uint16_t* p = &internal.chipramW[chipset.bpl2pt];
        chipset.bpl2pt +=1;
        chipset.bpl2dat = *p;
    }
}

void bitplaneCycle3_2(void){
    
    if(bitplaneActive()==0){
        return;
    }
    
    if(chipset.bplcon0 & 0x8000){
        
        if( (internal.bitplaneMask & 0x2)  == 0x2){
            uint16_t* p = &internal.chipramW[chipset.bpl2pt];
            chipset.bpl2pt +=1;
            chipset.bpl2dat = *p;
            return;
       }
    }
    
    //uint16_t* p = &internal.chipramW[chipset.bpl3pt];
    //chipset.bpl3pt +=1;
    //chipset.bpl3dat = *p;
    
}
void bitplaneCycle5_3(void){
    
    if(bitplaneActive()==0){
        evenCycle(); // let the copper run
        return;
    }
    

    
}
void bitplaneCycle1(void){
    
    
    if(bitplaneActive()==0){
        return;
    }
    
    if( (internal.bitplaneMask & 0x1)  == 0x0){
        return;
    }
    
    uint16_t* p = &internal.chipramW[chipset.bpl1pt];
    chipset.bpl1pt +=1;
    chipset.bpl1dat = *p;
    

    if(host.pixels == NULL){
        return;
    }
    
    int delta = 16;
    if(chipset.bplcon0 & 0x8000){
        delta = 8;
    }

    
    if(internal.vPos>144){
        printf("");
    }
    
    uint32_t* pixbuff = (uint32_t*)host.pixels;
    planar2chunky(&pixbuff[host.FBCounter], internal.palette, chipset.bpl1dat, chipset.bpl2dat, 0, 0, 0, delta);
}

void drawBlank(){
    uint32_t* pixbuff = (uint32_t*)host.pixels;
    
    int res=32;
    
    if(chipset.bplcon0 & 0x8000){
        res = 16;
    }
    
    for(int i=0;i<res;++i){
        pixbuff[host.FBCounter]=internal.palette[0];
        host.FBCounter +=1;
    }
    
}

int copperExecute(){
    
    if((chipset.dmaconr & 0x280) != 0x280){
        return 0;
    }
        
    switch(internal.copperCycle){
        case 0:
            internal.IR1 = internal.chipramW[internal.copperPC>>1];
            internal.IR1 = (internal.IR1 <<8) | (internal.IR1 >>8);
            internal.copperPC += 2;
            internal.copperCycle = 1;
            
            if( (internal.IR1 & 0x1) == 0x1){
                internal.copperCycle = 2;
            }
            return 1;
            break;
            
        case 1:
            internal.IR2 = internal.chipramW[internal.copperPC>>1];
            internal.copperPC += 2;
            
            internal.IR1 = (internal.IR1 >> 1) & 255;   // divide by 2 and mask off bad bits
            
            if(internal.IR1<0x20){
                internal.copperCycle = 0;
                return 1;};         //do nothing
            if(internal.IR1<0x40){
                internal.copperCycle = 4;return 1;};         //pause copper until next vbl
            
            //Move
            internal.IR2 = (internal.IR2 <<8) | (internal.IR2 >>8);
            //printf("%04x Cop Move: %04x -> (%s)\n",internal.copperPC - 4,internal.IR2,regNames[internal.IR1]);
            putChipReg16[internal.IR1](internal.IR2);
            internal.copperCycle = 0;
            return 1;
            break;
            
        case 2:
            internal.IR2 = internal.chipramW[internal.copperPC>>1];
            internal.copperPC += 2;
            
            internal.IR2 = (internal.IR2 <<8) | (internal.IR2 >>8);
            
            internal.copperCycle = 3;
            
            //Skip
            if( (internal.IR2 & 1) == 1){
                
                if( chipset.vhposr >= internal.IR1){
                    internal.copperPC +=4;
                }
                internal.copperCycle = 0;
                
                //printf("%04x Cop Skip!\n",internal.copperPC - 4);
                return 1;
                
            }
            
            return 1;
            break;
            
        case 3:
            
            //Wait
            if( chipset.vhposr >= internal.IR1){
                internal.copperCycle = 0;
            }
            
            break;
        case 4:
            //Copper operation frozen until next VBL/copper reset
            break;
    }
    
    return 0;
}



//********************Blitter



int blitterExecute(){
    
    static int state = 0;
    
    
    switch(state){
            
        case 0:
            //Blitter DMA Enabled and blitter busy flag set (which means bltsize was written to and blitting should start).
            if( (chipset.dmaconr & 0x4240) == 0x4240 ){
                
                if(chipset.bltcon1 & 1){
                    //Line Mode
                    //printf("Line Mode\n");
                    state = 1;
                }else{
                    //Copy Mode
                    //printf("Copy Mode\n");
                    internal.useMask = chipset.bltcon0 >> 8;
                    internal.shiftA = chipset.bltcon0 >> 12;
                    internal.shiftB = chipset.bltcon1 >> 12;
                    internal.minterm = chipset.bltcon0 & 255;
                    internal.xIncrement = 1;
                    internal.fillmode = (chipset.bltcon1 & 0x18) >> 3; // 1= Inclusive Fill, 2 = Exclusive Fill
                    
                    //all pointers are word addressed and masked for 2meg only
                    internal.apt=(chipset.bltapt >> 1);
                    internal.bpt=(chipset.bltbpt >> 1);
                    internal.cpt=(chipset.bltcpt >> 1);
                    internal.dpt=(chipset.bltdpt >> 1);
                    
                    
                    internal.amod=chipset.bltamod / 2; //these are signed so need to be divided not shifted
                    internal.bmod=chipset.bltbmod / 2;
                    internal.cmod=chipset.bltcmod / 2;
                    internal.dmod=chipset.bltdmod / 2;
                    
                    //internal.lastHWord=chipset.bltsizh - 1;
                    internal.sizeh = chipset.bltsizh;
                    
                    Internal_t* debug = &internal;
                    
                    state = 2;
                }
            }
            break;
            
        case 1:
            //Line Mode
            
            //No DMA cycle mode yet
            
            //Immediate mode Blitter
            blitter_execute(&chipset);
            state = 0;
            
            break;
            
        case 2:
            //Copy Mode
            //if descend mode is on.
            if((chipset.bltcon1 & 2)){
                internal.xIncrement = -1;
                
                if(internal.fillmode>0){
                    printf("NO FILL MODE YET!!");
                }
                
            }
            
            //DMA cycle mode
            //state = blitterCopyCycle();   //doesn't work at all.
            
            
            //Immediate mode Blitter
            blitter_execute(&chipset);
            state = 0;
            
            break;
    }
    
    
    
    return 0;
}

int blitterCopyCycle(){
    
    
    Internal_t* debug = &internal;
    
        static uint16_t previousA = 0;
        static uint16_t previousB = 0;
        
            uint16_t channelD = 0;
            
            //Channel A
            if(internal.useMask & 8){
                chipset.bltadat = internal.chipramW[internal.apt];
                chipset.bltadat = chipset.bltadat << 8 | chipset.bltadat >> 8;
                internal.apt +=internal.xIncrement;
            }
            uint16_t channelA = chipset.bltadat;
            
            //Channel B
            if(internal.useMask & 4){
                chipset.bltbdat = internal.chipramW[internal.bpt];
                chipset.bltbdat = chipset.bltbdat << 8 | chipset.bltbdat >> 8;
                internal.bpt +=internal.xIncrement;
            }
            uint16_t channelB = chipset.bltbdat;
            
            //Channel C
            if(internal.useMask & 2){
                chipset.bltcdat = internal.chipramW[internal.cpt];
                chipset.bltcdat = chipset.bltcdat << 8 | chipset.bltcdat >> 8;
                internal.cpt +=internal.xIncrement;
            }
            
            
            //Masking section
            
            if(chipset.bltsizh==internal.sizeh){
                channelA = channelA & chipset.bltafwm;
            }
            
            if(chipset.bltsizh==1){
                channelA = channelA & chipset.bltalwm;
            }
            
            //shifting section
            if(internal.xIncrement==-1){
                
                channelA = (previousA >> (16-internal.shiftA)) | (channelA << internal.shiftA);
                channelB = (previousB >> (16-internal.shiftB)) | (channelB << internal.shiftB);
                
            }else{
                
                channelA = (previousA << (16-internal.shiftA)) | (channelA >> internal.shiftA);
                channelB = (previousB << (16-internal.shiftB)) | (channelB >> internal.shiftB);
                
            }
            previousA = chipset.bltadat;
            previousB = chipset.bltbdat;
            
            
            channelD = logicFunction(internal.minterm, channelA, channelB, chipset.bltcdat);
            
            //Zero Flag
            if(channelD==0){
                chipset.dmaconr = chipset.dmaconr | 0x2000;   //set zero flag
            }else{
                chipset.dmaconr = chipset.dmaconr & 0xDFFF; // clear zero flag
            }
            
            //Channel D
            if(internal.useMask & 1){
                
                channelD = channelD << 8 | channelD >>8;
                internal.chipramW[internal.dpt] = channelD;
                internal.dpt +=internal.xIncrement;
            }
            
        
        
        if(internal.xIncrement == -1){
            internal.apt -= internal.amod;
            internal.bpt -= internal.bmod;
            internal.cpt -= internal.cmod;
            internal.dpt -= internal.dmod;
        }else{
            internal.apt += internal.amod;
            internal.bpt += internal.bmod;
            internal.cpt += internal.cmod;
            internal.dpt += internal.dmod;
        }
        
        chipset.bltsizh -= 1;
        
        if(chipset.bltsizh == 0x0){
            chipset.bltsizh = internal.sizeh;
            chipset.bltsizv -=1;
            previousA = 0;
            previousB = 0;

        }
        
        if(chipset.bltsizv==0x0){
            //chipset.bltsizh = 0;
            
            //save blitter state - something might depend upon the blitter being in a known state
            //chipset.bltapt = internal.apt << 1;
            chipset.bltbpt = internal.bpt << 1;
            chipset.bltcpt = internal.cpt << 1;
            chipset.bltdpt = internal.dpt << 1;
            
            chipset.dmaconr = chipset.dmaconr & 49151; //clear blitter busy bit
            putChipReg16[INTREQ](0x8040);              // generate an interrupt!
            return 0; // no more copy to do
        }
    
    return 2; // more copy to do!
}
