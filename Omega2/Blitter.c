//
//  Blitter.c
//  Omega2
//
//  Created by Matt Parsons on 31/03/2022.
//

#include "Blitter.h"



typedef struct{
    
    uint16_t bltddat;
    uint16_t dmaconr;
    uint16_t vposr;
    uint16_t vhposr;
    uint16_t dskdatr;
    uint16_t joy0dat;
    uint16_t joy1dat;
    uint16_t clxdat;
    uint16_t adkconr;
    uint16_t pot0dat;
    uint16_t pot1dat;
    uint16_t potinp; // in ECS this was renamed potgor as it has compalmentary register potgo
    uint16_t serdatr;
    uint16_t dskbytr;
    uint16_t intenar;
    uint16_t intreqr;
    uint32_t dskpt;
    uint16_t dsklen;
    uint16_t dskdat;
    uint16_t refptr;
    uint16_t vposw;
    uint16_t vhposw;
    uint16_t copcon;
    uint16_t serdat;
    uint16_t serper;
    uint16_t potgo;
    uint16_t joytest;
    uint16_t strequ;
    uint16_t strvbl;
    uint16_t strhor;
    uint16_t strlong;
    uint16_t bltcon0;
    uint16_t bltcon1;
    uint16_t bltafwm;
    uint16_t bltalwm;
    uint32_t bltcpt;
    uint32_t bltbpt;
    uint32_t bltapt;
    uint32_t bltdpt;
    uint16_t bltsize;
    uint16_t bltcon0l;
    uint16_t bltsizv;
    uint16_t bltsizh;
     int16_t bltcmod;
     int16_t bltbmod;
     int16_t bltamod;
     int16_t bltdmod;
    uint16_t reserved00;
    uint16_t reserved01;
    uint16_t reserved02;
    uint16_t reserved03;
    uint16_t bltcdat;
    uint16_t bltbdat;
    uint16_t bltadat;
    uint16_t reserved04;
    uint16_t sprhdat;
    uint16_t reserved05;
    uint16_t deniseid;
    uint16_t dsksync;
    uint32_t cop1lc;
    uint32_t cop2lc;
    uint16_t copjmp1;
    uint16_t copjmp2;
    uint16_t copins;
    uint16_t diwstrt;
    uint16_t diwstop;
    uint16_t ddfstrt;
    uint16_t ddfstop;
    uint16_t dmacon;
    uint16_t clxcon;
    uint16_t intena;
    uint16_t intreq;
    uint16_t adkcon;
    uint32_t aud0lc;
    uint16_t aud0len;
    uint16_t aud0per;
    uint16_t aud0vol;
    uint16_t aud0dat;
    uint16_t reserved06;
    uint16_t reserved07;
    uint32_t aud1lc;
    uint16_t aud1len;
    uint16_t aud1per;
    uint16_t aud1vol;
    uint16_t aud1dat;
    uint16_t reserved08;
    uint16_t reserved09;
    uint32_t aud2lc;
    uint16_t aud2len;
    uint16_t aud2per;
    uint16_t aud2vol;
    uint16_t aud2dat;
    uint16_t reserved10;
    uint16_t reserved11;
    uint32_t aud3lc;
    uint16_t aud3len;
    uint16_t aud3per;
    uint16_t aud3vol;
    uint16_t aud3dat;
    uint16_t reserved12;
    uint16_t reserved13;
    uint32_t bpl1pt;
    uint32_t bpl2pt;
    uint32_t bpl3pt;
    uint32_t bpl4pt;
    uint32_t bpl5pt;
    uint32_t bpl6pt;
    uint32_t bpl7pt;
    uint32_t bpl8pt;
    uint16_t bplcon0;
    uint16_t bplcon1;
    uint16_t bplcon2;
    uint16_t bplcon3;
    uint16_t bpl1mod;
    uint16_t bpl2mod;
    uint16_t reserved14;
    uint16_t reserved15;
    uint16_t bpl1dat;
    uint16_t bpl2dat;
    uint16_t bpl3dat;
    uint16_t bpl4dat;
    uint16_t bpl5dat;
    uint16_t bpl6dat;
    uint16_t bpl7dat;
    uint16_t bpl8dat;
    uint32_t spr0pt;
    uint32_t spr1pt;
    uint32_t spr2pt;
    uint32_t spr3pt;
    uint32_t spr4pt;
    uint32_t spr5pt;
    uint32_t spr6pt;
    uint32_t spr7pt;
    uint16_t spr0pos;
    uint16_t spr0ctl;
    uint16_t spr0data;
    uint16_t spr0datb;
    uint16_t spr1pos;
    uint16_t spr1ctl;
    uint16_t spr1data;
    uint16_t spr1datb;
    uint16_t spr2pos;
    uint16_t spr2ctl;
    uint16_t spr2data;
    uint16_t spr2datb;
    uint16_t spr3pos;
    uint16_t spr3ctl;
    uint16_t spr3data;
    uint16_t spr3datb;
    uint16_t spr4pos;
    uint16_t spr4ctl;
    uint16_t spr4data;
    uint16_t spr4datb;
    uint16_t spr5pos;
    uint16_t spr5ctl;
    uint16_t spr5data;
    uint16_t spr5datb;
    uint16_t spr6pos;
    uint16_t spr6ctl;
    uint16_t spr6data;
    uint16_t spr6datb;
    uint16_t spr7pos;
    uint16_t spr7ctl;
    uint16_t spr7data;
    uint16_t spr7datb;
    uint16_t color00;
    uint16_t color01;
    uint16_t color02;
    uint16_t color03;
    uint16_t color04;
    uint16_t color05;
    uint16_t color06;
    uint16_t color07;
    uint16_t color08;
    uint16_t color09;
    uint16_t color10;
    uint16_t color11;
    uint16_t color12;
    uint16_t color13;
    uint16_t color14;
    uint16_t color15;
    uint16_t color16;
    uint16_t color17;
    uint16_t color18;
    uint16_t color19;
    uint16_t color20;
    uint16_t color21;
    uint16_t color22;
    uint16_t color23;
    uint16_t color24;
    uint16_t color25;
    uint16_t color26;
    uint16_t color27;
    uint16_t color28;
    uint16_t color29;
    uint16_t color30;
    uint16_t color31;
    uint16_t htotal;
    uint16_t hsstop;
    uint16_t hbstrt;
    uint16_t hbstop;
    uint16_t vtotal;
    uint16_t vsstop;
    uint16_t vbstrt;
    uint16_t vbstop;
    uint16_t reserved16;
    uint16_t reserved17;
    uint16_t reserved18;
    uint16_t reserved19;
    uint16_t reserved20;
    uint16_t reserved21;
    uint16_t beamcon0;
    uint16_t hsstrt;
    uint16_t vsstrt;
    uint16_t hcenter;
    uint16_t diwhigh;
    uint16_t reserved22;
    uint16_t reserved23;
    uint16_t reserved24;
    uint16_t reserved25;
    uint16_t reserved26;
    uint16_t reserved27;
    uint16_t reserved28;
    uint16_t reserved29;
    uint16_t reserved30;
    uint16_t reserved31;
    uint16_t reserved32;
    uint16_t reserved33;
    uint16_t noop;
    
}Regs_t;

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


uint32_t delay = 32;

void blitter_execute(Chipset_t* chipstate){
    
    /*
    delay -=1;
    
    if(delay>0){
        return;
    }
    delay = 32;
    */
    
    //Wait until the display fetch has finished.
    if(chipstate->bitplaneFetchActive == 1){
       return;
    }
     
    Regs_t* chipset = (Regs_t*)&chipstate->chipram[0xDFF000];
    
    //Word Address ChipRAM
    uint16_t* chipramW = (uint16_t*)chipstate->chipram;
    
    
    //All operations start with a Zero Flag
    uint16_t* p = (uint16_t*) &chipstate->chipram[0xDFF002];
    *p = *p | 0x20; // set Big endian zero bit
    chipstate->DMACONR = chipstate->DMACONR | 0x2000; // set Little endian zero bit
    
        if(chipset->bltcon1 & 1){
            //Line Mode
            
            //printf("Blitter Line Mode\n");
            
            int octCode = (chipset->bltcon1 >> 2) & 7;
            int length = chipset->bltsizv;
            int inc1 = chipset->bltamod; // 4(dy - dx)
            int D = (int16_t)chipset->bltapt;// start value of 4dy - 2dx
            
            int planeAddr = chipset->bltcpt & 0x1FFFFE;//word address
            
            int planeMod  = chipset->bltcmod;
            int inc2 = chipset->bltbmod;// 4dy
            int d=0;
            
            int startPixel = chipset->bltcon0 >>12;
            
            int oneDot = (chipset->bltcon1 >>1) & 1;    // I don't support one dot mode yet
            
            if(oneDot==1){
                //printf("No Single pixel per H-line mode yet\n");
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
                        chipramW[addr] = pixel;
                        
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
            
            
            
            chipset->bltcpt = addr;  // update cpt with the last known address... nothing should rely on this...
            //chipset->bltapt = D;    // update apt with the last known address... nothing should rely on this...
            
            chipset->bltsizh = 0; // all done;
            chipset->bltsizv = 0; // all done;
 
            
            uint16_t* p = (uint16_t*) &chipstate->chipram[0xDFF002];
            *p = *p & 0xFFBF; // Clear Big endian Blitter busy bit
            chipstate->DMACONR = chipstate->DMACONR & 0xBFFF; // clear Little endian Blitter busy bit
            //generate an interrupt!
            chipstate->WriteWord[0x9C](0x8040);
            
            
        }else{

            static int count = 0; //How many times has the bitter been used?
            
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
                   // printf("NO INCLUSIVE FILL MODE YET!!\n");
                }
                
                if(fillmode==2){
                    //printf("NO EXCLUSIVE FILL MODE YET!!\n");
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
            

            
            
            uint16_t previousA = 0;
            uint16_t previousB = 0;
            
           
            for(int y=0;y<sizev;++y){
                
                for(int x=0;x<sizeh;++x){
                    uint16_t channelD = 0;
                    
                    //Channel A
                    if(useMask & 8){
                        adat = chipramW[apt];
                        adat = adat << 8 | adat >> 8;
                        apt +=xIncrement;
                    }
                    uint16_t channelA = adat;
                    
                    //Channel B
                    if(useMask & 4){
                        bdat = chipramW[bpt];
                        bdat = bdat << 8 | bdat >> 8;
                        bpt +=xIncrement;
                    }
                    uint16_t channelB = bdat;
                    
                    //Channel C
                    if(useMask & 2){
                        cdat = chipramW[cpt];
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
                        p = (uint16_t*) &chipstate->chipram[0xDFF002];
                        *p = *p & 0xFFDF; // Clear Big endian Blitter Zero bit
                        chipstate->DMACONR = chipstate->DMACONR & 0xDFFF; // clear Little endian Blitter Zero bit
                        
                    }
                    
                    //Channel D
                    if(useMask & 1){
                        
                        channelD = channelD << 8 | channelD >>8;
                        chipramW[dpt] = channelD;
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
            
             p = (uint16_t*) &chipstate->chipram[0xDFF002];
            *p = *p & 0xFFBF; // Clear Big endian Blitter busy bit
            chipstate->DMACONR = chipstate->DMACONR & 0xBFFF; // clear Little endian Blitter busy bit
            
            
            chipstate->WriteWord[0x9C](0x8040); // generate an interrupt!
            return;
        }
    
    
}

