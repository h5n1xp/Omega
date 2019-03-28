//
//  Blitter.c
//  The Omega Project
//  https://github.com/h5n1xp/Omega
//
//  Created by Matt Parsons on 20/02/2019.
//  Copyright © 2019 Matt Parsons. All rights reserved.
//  <h5n1xp@gmail.com>
//
//
//  This Source Code Form is subject to the terms of the
//  Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//

/*

 cycle 0:   set up blitter;
            if(mode = copy mode){
                cycle = 0;
            }
            if(mode = fill mode){
                cycle = 5;
            }
            if(mode = line mode){
                cycle = -1
            }
 
 cycle 1:  if(A needs load){
                load A;
                cycle = 2;
            }else if{B needs to load){
                load B;
                cycle = 3;
            } else if{C needs to load){
                Load C;
                cycle = 4;
            }
 
 cycle 2:  if{B needs to load){
                load B;
                cycle = 3;
            } else if{C needs to load){
                Load C;
                cycle = 4;
            }
 
 cycle 3:  if{C needs to load){
                Load C;
                cycle = 4;
            }
 
 cycle 4:   mask A;
            Shift A;
            Shift B;
            apply logic functions
            set clear zero flag
 
            if(Store D needed){
                Store D;
            }
 
            update v and h counters
            cycle = 1 ; //repeat the operation
 
            if(v == end condition){
                reset v = starting value.
                increment h
            }
 
            if(h == end conditon{
                clear blitter busy flag
                Generate interrupt;
                cycle = 0;  // rest the blitter state
            }
 
 cycle 5:   fill mode
 
 
 */


#include "Blitter.h"


void blitterCycle(void){
    
}

void (*blitterOperation[]) (void) = {blitterCycle,blitterCycle};

int OblitterExecute(){
    
    static int cycle;
    
    //check if blitter DMA is on; if not then return 0
    //Check if blitter needs to run (blitter busy flag is set); if not then return 0;
    
    blitterOperation[cycle]();

    return 1; //if we got here we probably used the memory cycle...
    
    
}

//********************* BLITER SECTION *****************************

uint16_t logicFunction(int minterm,uint16_t wordA, uint16_t wordB, uint16_t wordC){
    
    //Not section
    uint16_t notA = ~wordA;
    uint16_t notB = ~wordB;
    uint16_t notC = ~wordC;
    uint16_t channelD = 0;
    
    //Logic Section
    
    if(minterm & 0x80){
        channelD =            (wordA & wordB & wordC);}
    if(minterm & 0x40){
        channelD = channelD | (wordA & wordB & notC);}
    if(minterm & 0x20){
        channelD = channelD | (wordA & notB  & wordC);}
    if(minterm & 0x10){
        channelD = channelD | (wordA & notB  & notC);}
    if(minterm & 0x08){
        channelD = channelD | (notA  & wordB & wordC);}
    if(minterm & 0x04){
        channelD = channelD | (notA  & wordB & notC);}
    if(minterm & 0x02){
        channelD = channelD | (notA  & notB  & wordC);}
    if(minterm & 0x01){
        channelD = channelD | (notA  & notB  & notC);}
    
    return channelD;
}

void blitter_execute(Chipset_t* chipset){
    
    //Blitter DMA Enabled and blitter busy flag set (which means bltsize was written to and blitting should start).
    if( (chipset->dmaconr & 16960) == 16960 ){
        
        /*
         //make it so the blitter doesn't blit immediatly
         static int delay;
         
         delay -=1;
         
         if(delay>0){
         return;
         }
         delay = 10;
         */
        
        
        if(chipset->bltcon1 & 1){
            //Line Mode
            
            //printf("Blitter Line Mode\n");
            
            int octCode = (chipset->bltcon1 >> 2) & 7;
            int length = chipset->bltsizv;
            int inc1 = chipset->bltamod; // 4(dy - dx)
            int D = (int16_t)chipset->bltapt;// start value of 4dy - 2dx
            uint16_t* chipramW = internal.chipramW;
            
            int planeAddr = chipset->bltcpt & 0x1FFFFE;//word address
            
            int planeMod  = chipset->bltcmod;
            int inc2 = chipset->bltbmod;// 4dy
            int d=0;
            
            int startPixel = chipset->bltcon0 >>12;
            
            int oneDot = (chipset->bltcon1 >>1) & 1;    // I don't support one dot mode yet
            
            if(oneDot==1){
                printf("No Single pixel per H-line mode yet\n");
            }
            
            int minterm = chipset->bltcon0 & 255; //0xCA = normal 0x4A = XOR
            
            int patternShift = chipset->bltcon1 >>12;
            uint16_t pattern = chipset->bltbdat;
            
            pattern = (pattern >> patternShift) | (pattern <<(16-patternShift));
            
            int addr=0;//running address
            
            //printf("Octant %d: dx - %d dy - %d @ 0x%06x\n",octCode,length,inc2/4,chipset->bltcpt);
            
            switch(octCode){
                    
                case 0:
                    
                    for(int i=0;i<length;++i){
                        
                        int offset = d+startPixel;
                        addr = (planeAddr +(offset>>3)+(i*planeMod)) >> 1;
                        
                        //Pixel plot
                        uint16_t pixel = chipramW[addr];
                        pixel = (pixel <<8) | (pixel >>8);
                        pixel = logicFunction(minterm,0x8000 >> (offset&15),pattern,pixel);
                        pixel = (pixel <<8) | (pixel >>8);
                        chipramW[addr] =pixel;
                        
                        if(D>0){
                            D = D + inc1;
                            d = d + 1;
                            
                        }else{
                            D = D + inc2;
                        }
                        
                    }
                    break;
                    
                case 1:
                    for(int i=0;i<length;++i){
                        
                        int offset = d+startPixel;
                        addr =(planeAddr +(offset>>3)-(i*planeMod)) >> 1;
                        
                        //Pixel plot
                        uint16_t pixel = chipramW[addr];
                        pixel = (pixel <<8) | (pixel >>8);
                        pixel = logicFunction(minterm,0x8000 >> (offset&15),pattern,pixel);
                        pixel = (pixel <<8) | (pixel >>8);
                        chipramW[addr] =pixel;
                        
                        if(D>0){
                            D = D + inc1;
                            d = d + 1;
                            
                        }else{
                            D = D + inc2;
                        }
                        
                    }
                    break;
                    
                case 2:
                    startPixel = 15 - startPixel;
                    planeAddr +=1;
                    for(int i=0;i<length;++i){
                        
                        int offset = d+startPixel;
                        addr = (planeAddr - (offset>>3)+(i*planeMod))>>1;
                        
                        //Pixel plot
                        uint16_t pixel = chipramW[addr];
                        pixel = (pixel <<8) | (pixel >>8);
                        pixel = logicFunction(minterm,0x0001 << (offset&15),pattern,pixel);
                        pixel = (pixel <<8) | (pixel >>8);
                        chipramW[addr] = pixel;
                        
                        if(D>0){
                            D = D + inc1;
                            d = d + 1;
                            
                        }else{
                            D = D + inc2;
                        }
                        
                    }
                    break;
                    
                case 3:
                    
                    for(int i=0;i<length;++i){
                        
                        int offset =d+startPixel;
                        addr =(planeAddr +(offset>>3)-(i*planeMod))>>1;
                        
                        //Pixel plot
                        uint16_t pixel = chipramW[addr];
                        pixel = (pixel <<8) | (pixel >>8);
                        pixel = logicFunction(minterm,0x8000 >> (offset&15),pattern,pixel);
                        pixel = (pixel <<8) | (pixel >>8);
                        chipramW[addr] =pixel;
                        
                        if(D>0){
                            D = D + inc1;
                            d = d - 1;
                            
                        }else{
                            D = D + inc2;
                        }
                        
                    }
                    break;
                    
                case 4:
                    for(int i=0;i<length;++i){
                        
                        int offset = i+startPixel;
                        addr =(planeAddr +(offset>>3)+(d*planeMod))>>1;
                        
                        //Pixel plot
                        uint16_t pixel = chipramW[addr];
                        pixel = (pixel <<8) | (pixel >>8);
                        pixel = logicFunction(minterm,0x8000 >> (offset&15),pattern,pixel);
                        pixel = (pixel <<8) | (pixel >>8);
                        chipramW[addr] =pixel;
                        
                        if(D>0){
                            D = D + inc1;
                            d = d + 1;
                            
                        }else{
                            D = D + inc2;
                        }
                        
                    }
                    break;
                    
                case 5:
                    startPixel = 15 - startPixel;
                    planeAddr +=1;
                    for(int i=0;i<length;++i){
                        
                        int offset = i+startPixel;
                        addr = (planeAddr - (offset>>3)+(d*planeMod))>>1;
                        
                        //Pixel plot
                        uint16_t pixel = chipramW[addr];
                        pixel = (pixel <<8) | (pixel >>8);
                        pixel = logicFunction(minterm,0x0001 << (offset&15),pattern,pixel);
                        pixel = (pixel <<8) | (pixel >>8);
                        chipramW[addr] = pixel;
                        
                        if(D>0){
                            D = D + inc1;
                            d = d + 1;
                            
                        }else{
                            D = D + inc2;
                        }
                        
                    }
                    break;
                    
                case 6:
                    for(int i=0;i<length;++i){
                        
                        int offset = i+startPixel;
                        addr =(planeAddr +(offset>>3)-(d*planeMod))>>1;
                        
                        //Pixel plot
                        uint16_t pixel = chipramW[addr];
                        pixel = (pixel <<8) | (pixel >>8);
                        pixel = logicFunction(minterm,0x8000 >> (offset&15),pattern,pixel);
                        pixel = (pixel <<8) | (pixel >>8);
                        chipramW[addr] =pixel;
                        
                        if(D>0){
                            D = D + inc1;
                            d = d + 1;
                            
                        }else{
                            D = D + inc2;
                        }
                        
                    }
                    break;
                    
                case 7:
                    startPixel = 15 - startPixel;
                    planeAddr +=1;
                    for(int i=0;i<length;++i){
                        
                        int offset = i+startPixel;
                        addr = (planeAddr - (offset>>3)-(d*planeMod))>>1;
                        
                        //Pixel plot
                        uint16_t pixel = chipramW[addr];
                        pixel = (pixel <<8) | (pixel >>8);
                        pixel = logicFunction(minterm,0x0001 << (offset&15),pattern,pixel);
                        pixel = (pixel <<8) | (pixel >>8);
                        chipramW[addr] = pixel;
                        
                        if(D>0){
                            D = D + inc1;
                            d = d + 1;
                            
                        }else{
                            D = D + inc2;
                        }
                        
                    }
                    break;
                    
                default:
                    break;
                    
            }
            
            
            chipset->bltcpt =addr;  // update cpt with the last known address... nothing should rely on this...
            //chipset->bltapt = D;    // update apt with the last known address... nothing should rely on this...
            
            chipset->bltsizh = 0; // all done;
            chipset->bltsizv = 0; // all done;
            chipset->dmaconr = chipset->dmaconr & 49151; //clear blitter busy bit
            //generate an interrupt!
            putChipReg16[INTREQ](0x8040);
            
            
        }else{
            
            static int count = 0;
            
            //printf("Blitter Copy Mode: %d\n",count);
            
            count++;
            
            //Area Copy Blitter
            //printf("blit %d: A-%06x (%d) B-%06x (%d) C-%06x (%d) D-%06x (%d) W-%d H-%d\n",count,chipset->bltapt,chipset->bltamod,chipset->bltbpt,chipset->bltbmod,chipset->bltcpt,chipset->bltcmod,chipset->bltdpt,chipset->bltdmod,chipset->bltsizh,chipset->bltsizv);
            
            int useMask = chipset->bltcon0 >> 8;
            int shiftA = chipset->bltcon0 >> 12;
            int shiftB = chipset->bltcon1 >> 12;
            int minterm = chipset->bltcon0 & 255;
            int xIncrement = 1;
            int fillmode = (chipset->bltcon1 & 0x18) >> 3; // 1= Inclusive Fill, 2 = Exclusive Fill
            
            
            //if descend mode is on.
            if((chipset->bltcon1 & 2)){
                xIncrement = -1;
                
                if(fillmode==1){
                    printf("NO INCLUSIVE FILL MODE YET!!\n");
                }
                
                if(fillmode==2){
                    printf("NO EXCLUSIVE FILL MODE YET!!\n");
                }
                
            }
            
            
            //all pointers are word addressed and masked for 2meg only
            int apt=(chipset->bltapt >> 1);
            int bpt=(chipset->bltbpt >> 1);
            int cpt=(chipset->bltcpt >> 1);
            int dpt=(chipset->bltdpt >> 1);
            
            //Shifted seems to work properly
            int amod=chipset->bltamod >> 1;
            int bmod=chipset->bltbmod >> 1;
            int cmod=chipset->bltcmod >> 1;
            int dmod=chipset->bltdmod >> 1;
            
            //these are signed so need to be divided not shifted... but doesn't work properly
            //int amod=chipset->bltamod / 2;
            //int bmod=chipset->bltbmod / 2;
            //int cmod=chipset->bltcmod / 2;
            //int dmod=chipset->bltdmod / 2;
            
            uint16_t adat=chipset->bltadat;
            uint16_t bdat=chipset->bltbdat;
            uint16_t cdat=chipset->bltcdat;
            // int ddat=chipset->bltddat;
            
            uint16_t afwm = chipset->bltafwm;
            uint16_t alwm = chipset->bltalwm;
            
            int sizeh = chipset->bltsizh;
            int sizev = chipset->bltsizv;
            int lastHWord=sizeh - 1;
            
            //All operations start with a Zero Flag
            chipset->dmaconr = chipset->dmaconr | 0x2000;   //set zero flag
            
            for(int y=0;y<sizev;++y){
                
                uint16_t previousA = 0;
                uint16_t previousB = 0;
                
                
                for(int x=0;x<sizeh;++x){
                    uint16_t channelD = 0;
                    
                    //Channel A
                    if(useMask & 8){
                        adat = internal.chipramW[apt];
                        adat = adat << 8 | adat >> 8;
                        apt +=xIncrement;
                    }
                    uint16_t channelA = adat;
                    
                    //Channel B
                    if(useMask & 4){
                        bdat = internal.chipramW[bpt];
                        bdat = bdat << 8 | bdat >> 8;
                        bpt +=xIncrement;
                    }
                    uint16_t channelB = bdat;
                    
                    //Channel C
                    if(useMask & 2){
                        cdat = internal.chipramW[cpt];
                        cdat = cdat << 8 | cdat >> 8;
                        cpt +=xIncrement;
                    }
                    
                    
                    //Masking section
                    
                    if(x==0){
                        channelA = channelA & afwm;
                    }
                    
                    if(x==lastHWord){
                        channelA = channelA & alwm;
                    }
                    
                    uint16_t A = channelA; //Need to record the masked A for next cycle (Thanks to aros-sg on eab)
                    
                    //shifting section
                    if(xIncrement==-1){
                        
                        channelA = (previousA >> (16-shiftA)) | (channelA << shiftA);
                        channelB = (previousB >> (16-shiftB)) | (channelB << shiftB);
                        
                    }else{
                        
                        channelA = (previousA << (16-shiftA)) | (channelA >> shiftA);
                        channelB = (previousB << (16-shiftB)) | (channelB >> shiftB);
                        
                    }
                    //previousA = adat;
                    previousA = A;
                    previousB = bdat;
                    
                    
                    channelD = logicFunction(minterm, channelA, channelB, cdat);
                    
                    //Zero Flag
                    if(channelD!=0){
                        chipset->dmaconr = chipset->dmaconr & 0xDFFF; // clear zero flag
                    }
                    
                    //Channel D
                    if(useMask & 1){
                        
                        channelD = channelD << 8 | channelD >>8;
                        internal.chipramW[dpt] = channelD;
                        dpt +=xIncrement;
                    }
                    
                }
                
                if(xIncrement == -1){
                    apt -= amod;
                    bpt -= bmod;
                    cpt -= cmod;
                    dpt -= dmod;
                }else{
                    apt += amod;
                    bpt += bmod;
                    cpt += cmod;
                    dpt += dmod;
                }
                
            }
            
            
            chipset->bltsizh = 0; // all done;
            chipset->bltsizv = 0; // all done;
            
            //save blitter state - something might depend upon the blitter being in a known state
            chipset->bltapt = apt << 1;
            chipset->bltbpt = bpt << 1;
            chipset->bltcpt = cpt << 1;
            chipset->bltdpt = dpt << 1;
            chipset->bltadat = adat;
            chipset->bltbdat = bdat;
            chipset->bltcdat = cdat;
            //chipset->bltddat = ddat;
            
            chipset->dmaconr = chipset->dmaconr & 0xBFFF; //clear blitter busy bit
            putChipReg16[INTREQ](0x8040); // generate an interrupt!
            return;
        }
    }
    
}

