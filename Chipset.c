//
//  Chipset.c
//  The Omega Project
//  https://github.com/h5n1xp/Omega
//
//  Created by Matt Parsons on 02/02/2019.
//  Copyright © 2019 Matt Parsons. All rights reserved.
//  <h5n1xp@gmail.com>
//
//
//  This Source Code Form is subject to the terms of the
//  Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "Chipset.h"
#include "CIA.h"
#include "CPU.h"
#include "m68k.h"
#include "DMA.h"
#include "debug.h"

Chipset_t chipset;
Internal_t internal;

int planeMask[]={
    0x00,
    0x01,
    0x03,
    0x07,
    0x0F,
    0x1F,
    0x3F,
    0x7F,
    0xFF
};

void ChipsetInit(){
    
    internal.LOF = 0;
    internal.chipramW =(uint16_t*)low16Meg;
    chipset.deniseid = 15; //set DeniseID to OCS
    chipset.dmaconr = 0;
    chipset.copcon = 0;
    chipset.cop1lc = 0;
    chipset.cop2lc = 0;
    chipset.dmaconr = 0;
    internal.copperPC = 0;
    internal.copperCycle = 0;
    internal.hPos = 0;
    internal.vPos = 0;
    internal.bitplaneMask = 0;
    
    //For OS 1.x
    chipset.dsksync=0x4489;
    chipset.vtotal = 262;

    
    
}

uint32_t OCS2ARGB(uint16_t color){
    
    uint32_t value  =  ((color <<12)& 0xF00000) | ((color <<8)&0xF000) | ((color << 4)&0xF0); // high nybble.
    value |= (value >> 4);// low nyble
    value  = value | 0xFF000000; //opaque alpha
    return value;
}

uint32_t EHB2ARGB(uint16_t color){

    int red = (((color <<12)& 0xF00000) >> 1) & 0xFF0000;
    int green = (((color <<8)& 0xF000) >> 1 ) & 0xFF00;
    int blue = (((color <<4)& 0xF0) >> 1 ) & 0xFF;
    
    uint32_t value  =  red | green | blue;
    value  = value | 0xFF000000; //opaque alpha
    return value;
   
    
    return 0;
}

uint16_t noRead(void){
    printf("can't read a Write only Register %s\n",regNames[debugChipAddress]);
    debugChipAddress = 0;
    return 0;
}

//*******************************************

void dskpthL(uint32_t value){   //
    chipset.dskpt = value;
}

void vposwL(uint32_t value){    //
    internal.LOF = value >> 16;
}

void bltafwmL(uint32_t value){  //
    chipset.bltafwm = value >> 16;
    chipset.bltalwm = value & 65535;
}

void bltcon0L(uint32_t value){  //
    chipset.bltcon0 = value >> 16;
    chipset.bltcon1 = value & 65535;
}

void bltcpthL(uint32_t value){  //
    chipset.bltcpt = value;
}

void bltbpthL(uint32_t value){  //
    chipset.bltbpt = value;
}

void bltapthL(uint32_t value){  //
    chipset.bltapt = value;
}

void bltdpthL(uint32_t value){  //
    chipset.bltdpt = value;
}

void bltcmodL(uint32_t value){
    chipset.bltcmod = value >> 16;
    chipset.bltbmod = value & 65535;
}

void bltbmodL(uint32_t value){  //
    chipset.bltbmod = value >>16;
    chipset.bltamod = value & 65535;
}

void bltamodL(uint32_t value){  //
    chipset.bltamod = value >> 16;
    chipset.bltdmod = value & 65535;
}

void bltbdatL(uint32_t value){  //
    chipset.bltbdat = value >> 16;
    chipset.bltadat = value & 65535;
}

void cop1lchL(uint32_t value){  //
    chipset.cop1lc = value >> 1;
    //printf("(32bit)Copper 1: %0x\n",chipset.cop1lc);
}

void cop2lchL(uint32_t value){  //
    
    if(value==0){
        return;
    }
    
    chipset.cop2lc = value >> 1;
    //printf("(32bit)Copper 2: %0x\n",chipset.cop2lc);
}

void aud0lcL(uint32_t value){
    chipset.aud0lc = value >> 1;    //word aligned
}

void aud1lcL(uint32_t value){
    chipset.aud1lc = value >> 1;    //word aligned
}

void aud2lcL(uint32_t value){
    chipset.aud2lc = value >> 1;    //word aligned
}

void aud3lcL(uint32_t value){
    chipset.aud3lc = value >> 1;    //word aligned
}

void aud0perL(uint32_t value){  //
    chipset.aud0per = value >> 16;
    chipset.aud0vol = value & 65535;
}

void aud1perL(uint32_t value){  //
    chipset.aud1per = value >> 16;
    chipset.aud1vol = value & 65535;
}

void aud2perL(uint32_t value){  //
    chipset.aud2per = value >> 16;
    chipset.aud2vol = value & 65535;
}

void aud3perL(uint32_t value){  //
    chipset.aud3per = value >> 16;
    chipset.aud3vol = value & 65535;
}

void vbstrtL(uint32_t value){
    chipset.vbstrt = value >> 16;
    chipset.vbstop = value & 65535;
}

void noopL(uint32_t value){
    
}

void longWrite(uint32_t value){
    printf("32bit Write: to %s (%0x - %d) not implemented!\n",regNames[debugChipAddress],debugChipAddress<<1,debugChipAddress);
        debugChipAddress = 0;
}

//**********************************************************

uint16_t bltddat(){
    return chipset.bltddat;
}
uint16_t dmaconr(){
    return chipset.dmaconr;
}
uint16_t vposr(){
    return chipset.vposr;
}
uint16_t vhposr(){
    return chipset.vhposr;
}
uint16_t dskdatr(){
    return chipset.dskdatr;
}
uint16_t joy0dat(){
    return chipset.joy0dat;
}
uint16_t joy1dat(){
    return chipset.joy1dat;
}
uint16_t clxdat(){
    return chipset.clxdat;
}
uint16_t adkconr(){
    
    printf("adkconr called\n");
    
    return chipset.adkconr;
}
uint16_t pot0dat(){
    return chipset.pot0dat;
}
uint16_t pot1dat(){
    return chipset.pot1dat;
}
uint16_t potgor(){
    return chipset.potinp;  //this was renamed in ECS to potgor
}
uint16_t serdatr(){
    return chipset.serdatr;
}
uint16_t dskbytr(){
    return chipset.dskbytr;
}
uint16_t intenar(){
    return chipset.intenar;
}
uint16_t intreqr(){
    return chipset.intreqr;
}

uint16_t deniseid(){
    return chipset.deniseid;
}

void wordWrite(uint16_t value){
    printf("16bit Write: %d to %s (%0x - %d) Not implemented!\n", debugChipValue, regNames[debugChipAddress],debugChipAddress<<1,debugChipAddress);
    debugChipAddress = 0;
}

void wordIllegalWrite(uint16_t value){
     printf("16bit Write: To a read only register %s\n",regNames[debugChipAddress]);
    debugChipAddress = 0;
}

void dskpth(uint16_t value){
    chipset.dskpt  = (value << 16) | (chipset.dskpt & 0x0000FFFF); //using the new 32bit register
}

void dskptl(uint16_t value){
    chipset.dskpt  =  value        | (chipset.dskpt & 0xFFFF0000); //using the new 32bit register
}

void dsklen(uint16_t value){
    static int dskDMALock = 0; //the dsklen register need to be written twice to update

    
    if(value & 0x8000){
        dskDMALock +=1;
    }else{
        dskDMALock = 0;
    }

    //printf("dsklen called: %04x (Lock:%d)\n",value,dskDMALock);
    
    if(dskDMALock==2){
        chipset.dsklen = value; //a write of 0X8000 here is a secondary lock on the DMA to avoid accidental disk writes
        dskDMALock =0;
    }else{
        chipset.dsklen = value & 0x7FFF;
    }
}

void dskdat(uint16_t value){
    chipset.dskdat = value;
}

void vposw(uint16_t value){
    internal.LOF = value;
}

void vhposw(uint16_t value){
    //internal.vPos = (value >> 8);
    //internal.hPos = value;
    //Converstion with Toni Wilen suggests this should never be written to!
}

void copcon(uint16_t  value){
    chipset.copcon = value;
}

void serdat(uint16_t value){
    chipset.serdat = value;
}

void serper(uint16_t value){
    chipset.serper = value;
}

void potgo(uint16_t value){
    chipset.potgo = value;
}

void bltcon0(uint16_t value){
    chipset.bltcon0 = value;
}

void bltcon0l(uint16_t value){
    chipset.bltcon0 = (chipset.bltcon0 & 0xFF00) | value;
}

void bltcon1(uint16_t value){
    chipset.bltcon1 = value;
}
void bltafwm(uint16_t value){
    chipset.bltafwm = value;
}
void bltalwm(uint16_t value){
    chipset.bltalwm = value;
}

void bltcpth(uint16_t value){
    chipset.bltcpt = (value << 16) | (chipset.bltcpt & 0x0000FFFF); //using the new 32bit register
}
void bltcptl(uint16_t value){
    chipset.bltcpt =  value        | (chipset.bltcpt & 0xFFFF0000); //using the new 32bit register
}
void bltbpth(uint16_t value){
    chipset.bltbpt = (value << 16) | (chipset.bltbpt & 0x0000FFFF); //using the new 32bit register
}
void bltbptl(uint16_t value){
    chipset.bltbpt =  value        | (chipset.bltbpt & 0xFFFF0000); //using the new 32bit register
}
void bltapth(uint16_t value){
    chipset.bltapt = (value << 16) | (chipset.bltapt & 0x0000FFFF); //using the new 32bit register
}
void bltaptl(uint16_t value){
    chipset.bltapt =  value        | (chipset.bltapt & 0xFFFF0000); //using the new 32bit register
}
void bltdpth(uint16_t value){
    chipset.bltdpt = (value << 16) | (chipset.bltdpt & 0x0000FFFF); //using the new 32bit register
}
void bltdptl(uint16_t value){
    chipset.bltdpt =  value        | (chipset.bltdpt & 0xFFFF0000); //using the new 32bit register
}

void bltsize(uint16_t value){
    chipset.bltsizv = value >> 6;
    chipset.bltsizh = value & 63;

    //uint16_t debugv = chipset.bltsizv;
    //uint16_t debugh = chipset.bltsizh;
    
    if(chipset.bltsizv == 0){
        chipset.bltsizv=1024;
    }

    if(chipset.bltsizh == 0){
        chipset.bltsizh=64;
    }

    //use the old OCS register to keep a copy of the blit width...not needed anymore
    chipset.bltsize = chipset.bltsizh;

    //Set to blitter busy
    chipset.dmaconr =chipset.dmaconr | 0x4000;
}

void bltsizv(uint16_t value){
    chipset.bltsizv = value;
}
void bltsizh(uint16_t value){
    chipset.bltsizh = value;
    chipset.bltsize = chipset.bltsizh;   //use the old OCS register to keep a copy of the blit width.
    
    //Set to blitter busy and start the blitter
    chipset.dmaconr =chipset.dmaconr | 0x4000;
}

void bltcmod(uint16_t value){
    chipset.bltcmod = value;
}
void bltbmod(uint16_t value){
    chipset.bltbmod = value;
}
void bltamod(uint16_t value){
    chipset.bltamod = value;
}
void bltdmod(uint16_t value){
    chipset.bltdmod = value;
}
void bltcdat(uint16_t value){
    chipset.bltcdat = value;
}
void bltbdat(uint16_t value){
    chipset.bltbdat = value;
}
void bltadat(uint16_t value){
    chipset.bltadat = value;
}

void dsksync(uint16_t value){
    chipset.dsksync=value;
}

void cop1lch(uint16_t value){
      chipset.cop1lc = (value << 15) | (chipset.cop1lc & 0x00007FFF); //this is only shifted by 15 because all addresses are word aligend
}
void cop1lcl(uint16_t value){
//    printf("(16bit)Copper 1: %0x\n",chipset.cop1lc);
        chipset.cop1lc = (value >> 1)  | (chipset.cop1lc & 0xFFFF8000);
}
void cop2lch(uint16_t value){
      chipset.cop2lc = (value << 15) | (chipset.cop1lc & 0x00007FFF); //this is only shifted by 15 because all addresses are word aligend
}
void cop2lcl(uint16_t value){
//    printf("(16bit)Copper 2: %0x\n",chipset.cop2lc);
    chipset.cop2lc = (value >> 1)  | (chipset.cop2lc & 0xFFFF8000);
}

void copjmp1(uint16_t value){
    //printf("COPJMP1: %0x\n",chipset.cop1lc);
    internal.copperPC = chipset.cop1lc;
    internal.copperCycle = 0; // reset the Copper
}

void copjmp2(uint16_t value){
    //printf("COPJMP2: %0x\n",chipset.cop2lc);
    internal.copperPC = chipset.cop2lc;

}

void copjmp2L(uint32_t value){
    copjmp2(value); //this just resets the copper...
}

void diwstrt(uint16_t value){
    chipset.diwstrt = value;
    //printf("diwstart: %04x\n",value);
}

void diwstop(uint16_t value){
    chipset.diwstop = value;
    
}


void ddfstrt(uint16_t value){
    chipset.ddfstrt = value;
    //printf("Fetch Start: %02x\n",value);
}

void ddfstop(uint16_t value){
    chipset.ddfstop = value;
    //printf("Fetch Stop: %02x\n",value);
}

void  dmacon(uint16_t value){
    if( (value & 32768) ==0){
        chipset.dmaconr = chipset.dmaconr ^ (chipset.dmaconr & value);
    }else{
        chipset.dmaconr = chipset.dmaconr | (value  & 2047); //top 5 bits not writable
    }
}

void clxcon(uint16_t value){
    
}

void intena(uint16_t value){
    
    if( (value & 32768) == 0){
        chipset.intenar = chipset.intenar ^ (chipset.intenar & value);
    }else{
        chipset.intenar = chipset.intenar | (value  & 32767);
    }
    
    checkInterrupt(&chipset);
}

void intreq(uint16_t value){
    if( (value & 32768) ==0){
        chipset.intreqr = chipset.intreqr ^ (chipset.intreqr & value);
    }else{
        chipset.intreqr = chipset.intreqr | (value  & 32767);
    }
    
    checkInterrupt(&chipset);

}

void adkcon(uint16_t value){
    if( (value & 32768) ==0){
        chipset.adkconr = chipset.adkconr ^ (chipset.adkconr & value);
    }else{
        chipset.adkconr = chipset.adkconr | (value  & 32767);
    }
}

void aud0lch(uint16_t value){
    chipset.aud0lc = (value << 15) | (chipset.aud0lc & 0x00007FFF); //this is only shifted by 15 because all addresses are word aligend
}

void aud0lcl(uint16_t value){
    chipset.aud0lc = (value >> 1)  | (chipset.aud0lc & 0xFFFF8000);
}

void aud0len(uint16_t value){
    chipset.aud0len = value;
}

void aud0per(uint16_t value){
    chipset.aud0per = value;
}

void aud0vol(uint16_t value){
    chipset.aud0vol = value;
}

void aud0dat(uint16_t value){
    chipset.aud0dat = value;
}

void aud1lch(uint16_t value){
    chipset.aud1lc = (value << 15) | (chipset.aud1lc & 0x00007FFF); //this is only shifted by 15 because all addresses are word aligend
}

void aud1lcl(uint16_t value){
    chipset.aud1lc = (value >> 1)  | (chipset.aud1lc & 0xFFFF8000);
}

void aud1len(uint16_t value){
    chipset.aud1len = value;
}

void aud1per(uint16_t value){
    chipset.aud1per = value;
}

void aud1vol(uint16_t value){
    chipset.aud0vol = value;
}

void aud1dat(uint16_t value){
    chipset.aud1dat = value;
}

void aud2lch(uint16_t value){
    chipset.aud2lc = (value << 15) | (chipset.aud2lc & 0x00007FFF); //this is only shifted by 15 because all addresses are word aligend
}

void aud2lcl(uint16_t value){
    chipset.aud2lc = (value >> 1)  | (chipset.aud2lc & 0xFFFF8000);
}

void aud2len(uint16_t value){
    chipset.aud2len = value;
}

void aud2per(uint16_t value){
    chipset.aud2per = value;
}

void aud2vol(uint16_t value){
    chipset.aud2vol = value;
}

void aud2dat(uint16_t value){
    chipset.aud2dat = value;
}

void aud3lch(uint16_t value){
    chipset.aud3lc = (value << 15) | (chipset.aud3lc & 0x00007FFF); //this is only shifted by 15 because all addresses are word aligend
}

void aud3lcl(uint16_t value){
    chipset.aud3lc = (value >> 1)  | (chipset.aud3lc & 0xFFFF8000);
}

void aud3len(uint16_t value){
    chipset.aud3len = value;
}

void aud3per(uint16_t value){
    chipset.aud3per = value;
}

void aud3vol(uint16_t value){
    chipset.aud3vol = value;
}

void aud3dat(uint16_t value){
    chipset.aud3dat = value;
}


void bpl1pth(uint16_t value){
    chipset.bpl1pt = (value << 15) | (chipset.bpl1pt & 0x00007FFF); //this is only shifted by 15 because all bitplane addresses are word aligend
}
void bpl1ptl(uint16_t value){
    chipset.bpl1pt = (value >> 1)  | (chipset.bpl1pt & 0xFFFF8000);
}

void bpl2pth(uint16_t value){
    chipset.bpl2pt = (value << 15) | (chipset.bpl2pt & 0x00007FFF);
}
void bpl2ptl(uint16_t value){
    chipset.bpl2pt = (value >> 1)  | (chipset.bpl2pt & 0xFFFF8000);
}

void bpl3pth(uint16_t value){
    chipset.bpl3pt = (value << 15) | (chipset.bpl3pt & 0x00007FFF);
}
void bpl3ptl(uint16_t value){
    chipset.bpl3pt = (value >> 1)  | (chipset.bpl3pt & 0xFFFF8000);
}

void bpl4pth(uint16_t value){
    chipset.bpl4pt = (value << 15) | (chipset.bpl4pt & 0x00007FFF);
}
void bpl4ptl(uint16_t value){
    chipset.bpl4pt = (value >> 1)  | (chipset.bpl4pt & 0xFFFF8000);
}

void bpl5pth(uint16_t value){
    chipset.bpl5pt = (value << 15) | (chipset.bpl5pt & 0x00007FFF);
}
void bpl5ptl(uint16_t value){
    chipset.bpl5pt = (value >> 1)  | (chipset.bpl5pt & 0xFFFF8000);
}

void bpl6pth(uint16_t value){
    chipset.bpl6pt = (value << 15) | (chipset.bpl6pt & 0x00007FFF);
}
void bpl6ptl(uint16_t value){
    chipset.bpl6pt = (value >> 1)  | (chipset.bpl6pt & 0xFFFF8000);
}

void bpl7pth(uint16_t value){
    chipset.bpl7pt = (value << 15) | (chipset.bpl7pt & 0x00007FFF);
}
void bpl7ptl(uint16_t value){
    chipset.bpl7pt = (value >> 1)  | (chipset.bpl7pt & 0xFFFF8000);
}

void bpl8pth(uint16_t value){
    chipset.bpl8pt = (value << 15) | (chipset.bpl8pt & 0x00007FFF);
}
void bpl8ptl(uint16_t value){
    chipset.bpl8pt = (value >> 1)  | (chipset.bpl8pt & 0xFFFF8000);
}

void bplcon0(uint16_t value){
    
    displayLineReset(); //restart drawing if the bplcon has changed. This might need to take a value as to how many lines were needed to change the mode

    chipset.bplcon0 = value;
    
    int planes = (value >> 12) & 7 ;
    
    internal.bitplaneMask = planeMask[planes];
}

void bplcon1(uint16_t value){
    chipset.bplcon1 = value;
}
void bplcon2(uint16_t value){
    chipset.bplcon2 = value;
}
void bplcon3(uint16_t value){
    chipset.bplcon3 = value;
}

void bpl1mod(uint16_t value){
    chipset.bpl1mod = value >> 1; // Word aligned
}
void bpl2mod(uint16_t value){
    chipset.bpl2mod = value >> 1;
}
void bplcon4(uint16_t value){
    //No AGA support yet
}

void bpl1dat(uint16_t value){
    chipset.bpl1dat = value;
}
void bpl2dat(uint16_t value){
    chipset.bpl2dat = value;
}
void bpl3dat(uint16_t value){
    chipset.bpl3dat = value;
}
void bpl4dat(uint16_t value){
    chipset.bpl4dat = value;
}
void bpl5dat(uint16_t value){
    chipset.bpl5dat = value;
}
void bpl6dat(uint16_t value){
    chipset.bpl6dat = value;
}
void bpl7dat(uint16_t value){
    chipset.bpl7dat = value;
}
void bpl8dat(uint16_t value){
    chipset.bpl8dat = value;
}

void spr0pth(uint16_t value){
    chipset.spr0pt = (value << 15) | (chipset.spr0pt & 0x00007FFF);  //this is only shifted by 15 becuase all addresses are word aligend
}
void spr0ptl(uint16_t value){
    chipset.spr0pt = (value>>1)    | (chipset.spr0pt & 0xFFFF8000);
}
void spr1pth(uint16_t value){
    chipset.spr1pt = (value << 15) | (chipset.spr1pt & 0x00007FFF);  //this is only shifted by 15 becuase all addresses are word aligend
}
void spr1ptl(uint16_t value){
    chipset.spr1pt = (value>>1)    | (chipset.spr1pt & 0xFFFF8000);
}
void spr2pth(uint16_t value){
    chipset.spr2pt = (value << 15) | (chipset.spr2pt & 0x00007FFF);  //this is only shifted by 15 becuase all addresses are word aligend
}
void spr2ptl(uint16_t value){
    chipset.spr2pt = (value>>1)    | (chipset.spr2pt & 0xFFFF8000);
}
void spr3pth(uint16_t value){
    chipset.spr3pt = (value << 15) | (chipset.spr3pt & 0x00007FFF);  //this is only shifted by 15 becuase all addresses are word aligend
}
void spr3ptl(uint16_t value){
    chipset.spr3pt = (value>>1)    | (chipset.spr3pt & 0xFFFF8000);
}
void spr4pth(uint16_t value){
    chipset.spr4pt = (value << 15) | (chipset.spr4pt & 0x00007FFF);  //this is only shifted by 15 becuase all addresses are word aligend
}
void spr4ptl(uint16_t value){
    chipset.spr4pt = (value>>1)    | (chipset.spr4pt & 0xFFFF8000);
}
void spr5pth(uint16_t value){
    chipset.spr5pt = (value << 15) | (chipset.spr5pt & 0x00007FFF);  //this is only shifted by 15 becuase all addresses are word aligend
}
void spr5ptl(uint16_t value){
    chipset.spr5pt = (value>>1)    | (chipset.spr5pt & 0xFFFF8000);
}
void spr6pth(uint16_t value){
    chipset.spr6pt = (value << 15) | (chipset.spr6pt & 0x00007FFF);  //this is only shifted by 15 becuase all addresses are word aligend
}
void spr6ptl(uint16_t value){
    chipset.spr6pt = (value>>1)    | (chipset.spr6pt & 0xFFFF8000);
}
void spr7pth(uint16_t value){
    chipset.spr7pt = (value << 15) | (chipset.spr7pt & 0x00007FFF);  //this is only shifted by 15 becuase all addresses are word aligend
}
void spr7ptl(uint16_t value){
    chipset.spr7pt = (value>>1)    | (chipset.spr7pt & 0xFFFF8000);
}

void spr0pos(uint16_t value){
    chipset.spr0pos = value;
}
void spr0ctl(uint16_t value){
    chipset.spr0ctl = value;
}

void spr1pos(uint16_t value){
    chipset.spr1pos = value;
}
void spr1ctl(uint16_t value){
    chipset.spr1ctl = value;
}

void spr2pos(uint16_t value){
    chipset.spr2pos = value;
}
void spr2ctl(uint16_t value){
    chipset.spr2ctl = value;
}

void spr3pos(uint16_t value){
    chipset.spr3pos = value;
}
void spr3ctl(uint16_t value){
    chipset.spr3ctl = value;
}

void spr4pos(uint16_t value){
    chipset.spr4pos = value;
}
void spr4ctl(uint16_t value){
    chipset.spr4ctl = value;
}

void spr5pos(uint16_t value){
    chipset.spr5pos = value;
}
void spr5ctl(uint16_t value){
    chipset.spr5ctl = value;
}

void spr6pos(uint16_t value){
    chipset.spr6pos = value;
}
void spr6ctl(uint16_t value){
    chipset.spr6ctl = value;
}

void spr7pos(uint16_t value){
    chipset.spr7pos = value;
}
void spr7ctl(uint16_t value){
    chipset.spr7ctl = value;
}

void spr7data(uint16_t value){
    chipset.spr7data = value;
}

void color00(uint16_t value){
    chipset.color00 = value; //old register
    internal.palette[0] = OCS2ARGB(value);
    internal.palette[32] = EHB2ARGB(value);
}
void color01(uint16_t value){
    chipset.color01 = value; //old register
    internal.palette[1] = OCS2ARGB(value);
    internal.palette[33] = EHB2ARGB(value);
}

void color02(uint16_t value){
    chipset.color02 = value; //old register
    internal.palette[2] = OCS2ARGB(value);
    internal.palette[34] = EHB2ARGB(value);
}
void color03(uint16_t value){
    chipset.color03 = value; //old register
    internal.palette[3] = OCS2ARGB(value);
    internal.palette[35] = EHB2ARGB(value);
}
void color04(uint16_t value){
    chipset.color04 = value; //old register
    internal.palette[4] = OCS2ARGB(value);
    internal.palette[36] = EHB2ARGB(value);
}
void color05(uint16_t value){
    chipset.color05 = value; //old register
    internal.palette[5] = OCS2ARGB(value);
    internal.palette[37] = EHB2ARGB(value);
}
void color06(uint16_t value){
    chipset.color06 = value; //old register
    internal.palette[6] = OCS2ARGB(value);
    internal.palette[38] = EHB2ARGB(value);
}
void color07(uint16_t value){
    chipset.color07 = value; //old register
    internal.palette[7] = OCS2ARGB(value);
    internal.palette[39] = EHB2ARGB(value);
}
void color08(uint16_t value){
    chipset.color08 = value; //old register
    internal.palette[8] = OCS2ARGB(value);
    internal.palette[40] = EHB2ARGB(value);
}
void color09(uint16_t value){
    chipset.color09 = value; //old register
    internal.palette[9] = OCS2ARGB(value);
    internal.palette[41] = EHB2ARGB(value);
}
void color10(uint16_t value){
    chipset.color10 = value; //old register
    internal.palette[10] = OCS2ARGB(value);
    internal.palette[42] = EHB2ARGB(value);
}
void color11(uint16_t value){
    chipset.color11 = value; //old register
    internal.palette[11] = OCS2ARGB(value);
    internal.palette[43] = EHB2ARGB(value);
}
void color12(uint16_t value){
    chipset.color12 = value; //old register
    internal.palette[12] = OCS2ARGB(value);
    internal.palette[44] = EHB2ARGB(value);
}
void color13(uint16_t value){
    chipset.color13 = value; //old register
    internal.palette[13] = OCS2ARGB(value);
    internal.palette[45] = EHB2ARGB(value);
}
void color14(uint16_t value){
    chipset.color14 = value; //old register
    internal.palette[14] = OCS2ARGB(value);
    internal.palette[46] = EHB2ARGB(value);
}
void color15(uint16_t value){
    chipset.color15 = value; //old register
    internal.palette[15] = OCS2ARGB(value);
    internal.palette[47] = EHB2ARGB(value);
}
void color16(uint16_t value){
    chipset.color16 = value; //old register
    internal.palette[16] = OCS2ARGB(value);
    internal.palette[48] = EHB2ARGB(value);
}
void color17(uint16_t value){
    chipset.color17 = value; //old register
    internal.palette[17] = OCS2ARGB(value);
    internal.palette[49] = EHB2ARGB(value);
}
void color18(uint16_t value){
    chipset.color18 = value; //old register
    internal.palette[18] = OCS2ARGB(value);
    internal.palette[50] = EHB2ARGB(value);
}
void color19(uint16_t value){
    chipset.color19 = value; //old register
    internal.palette[19] = OCS2ARGB(value);
    internal.palette[51] = EHB2ARGB(value);
}
void color20(uint16_t value){
    chipset.color20 = value; //old register
    internal.palette[20] = OCS2ARGB(value);
    internal.palette[52] = EHB2ARGB(value);
}
void color21(uint16_t value){
    chipset.color21 = value; //old register
    internal.palette[21] = OCS2ARGB(value);
    internal.palette[53] = EHB2ARGB(value);
}
void color22(uint16_t value){
    chipset.color22 = value; //old register
    internal.palette[22] = OCS2ARGB(value);
    internal.palette[54] = EHB2ARGB(value);
}
void color23(uint16_t value){
    chipset.color23 = value; //old register
    internal.palette[23] = OCS2ARGB(value);
    internal.palette[55] = EHB2ARGB(value);
}
void color24(uint16_t value){
    chipset.color24 = value; //old register
    internal.palette[24] = OCS2ARGB(value);
    internal.palette[56] = EHB2ARGB(value);
}
void color25(uint16_t value){
    chipset.color25 = value; //old register
    internal.palette[25] = OCS2ARGB(value);
    internal.palette[57] = EHB2ARGB(value);
}
void color26(uint16_t value){
    chipset.color26 = value; //old register
    internal.palette[26] = OCS2ARGB(value);
    internal.palette[58] = EHB2ARGB(value);
}
void color27(uint16_t value){
    chipset.color27 = value; //old register
    internal.palette[27] = OCS2ARGB(value);
    internal.palette[59] = EHB2ARGB(value);
}
void color28(uint16_t value){
    chipset.color28 = value; //old register
    internal.palette[28] = OCS2ARGB(value);
    internal.palette[60] = EHB2ARGB(value);
}
void color29(uint16_t value){
    chipset.color29 = value; //old register
    internal.palette[29] = OCS2ARGB(value);
    internal.palette[61] = EHB2ARGB(value);
}
void color30(uint16_t value){
    chipset.color30 = value; //old register
    internal.palette[30] = OCS2ARGB(value);
    internal.palette[62] = EHB2ARGB(value);
}
void color31(uint16_t value){
    chipset.color31 = value; //old register
    internal.palette[31] = OCS2ARGB(value);
    internal.palette[63] = EHB2ARGB(value);
}

void color00L(uint32_t value){
    color00(value >> 16);
    color01(value & 65535);
}

void color02L(uint32_t value){
    color02(value >> 16);
    color03(value & 65535);
}

void color04L(uint32_t value){
    color04(value >> 16);
    color05(value & 65535);
}

void color06L(uint32_t value){
    color06(value >> 16);
    color07(value & 65535);
}

void color08L(uint32_t value){
    color08(value >> 16);
    color09(value & 65535);
}

void color10L(uint32_t value){
    color10(value >> 16);
    color11(value & 65535);
}

void color12L(uint32_t value){
    color12(value >> 16);
    color13(value & 65535);
}

void color14L(uint32_t value){
    color14(value >> 16);
    color15(value & 65535);
}

void color16L(uint32_t value){
    color16(value >> 16);
    color17(value & 65535);
}

void color18L(uint32_t value){
    color18(value >> 16);
    color19(value & 65535);
}

void color20L(uint32_t value){
    color20(value >> 16);
    color21(value & 65535);
}

void color22L(uint32_t value){
    color22(value >> 16);
    color23(value & 65535);
}

void color24L(uint32_t value){
    color24(value >> 16);
    color25(value & 65535);
}

void color26L(uint32_t value){
    color26(value >> 16);
    color27(value & 65535);
}

void color28L(uint32_t value){
    color28(value >> 16);
    color29(value & 65535);
}

void color30L(uint32_t value){
    color30(value >> 16);
    color31(value & 65535);
}

void vtotal(uint16_t value){
    chipset.vtotal = value;
}

void beamcon0(uint16_t value){
    chipset.beamcon0 = value;
}

void fmode(uint16_t value){
    //no aga support yet
}

void noop(uint16_t value){
    
}


//***********************************************************************************************
uint32_t noReadL(void){
    printf("32bit Read: from %s (%0x - %d) Not implemented!\n",regNames[debugChipAddress],debugChipAddress<<1,debugChipAddress);
    return 0;
}

uint32_t vposrL(){  //0x2
    return chipset.vposr << 16 | chipset.vhposr;
}

uint8_t noReadB(void){
    printf("8bit Read: from %s (%0x - %d) Not implemented!\n",regNames[debugChipAddress],debugChipAddress<<1,debugChipAddress);
    return 0;
}

uint8_t dmaconrBH(){
    return chipset.dmaconr >> 8; // only read the top 8bits
}

uint8_t dmaconrBL(){
    return chipset.dmaconr & 0xFF; // only read the bottom 8bits
}

uint8_t vposrBH(){
    return chipset.vposr >> 8;
}

uint8_t vposrBL(){
    return chipset.vposr & 0xFF;
}

uint8_t vhposrBH(){
    return chipset.vhposr >> 8;
}

uint8_t vhposrBL(){
    return chipset.vhposr & 0xFF;
}

uint8_t joy0datBH(){
    return chipset.joy0dat >> 8; // only read the top 8bits
}

uint8_t joy0datBL(){
    return chipset.joy0dat & 0xFF; // only read the bottom 8bits
}

uint8_t joy1datBH(){
    return chipset.joy1dat >> 8; // only read the top 8bits
}

uint8_t joy1datBL(){
    return chipset.joy1dat & 0xFF; // only read the bottom 8bits
}

uint8_t potgorBH(){
    return chipset.potinp >> 8; // only read the top 8bits
}

uint8_t potgorBL(){ //  0xB
    return chipset.potinp & 0xFF; // only read the bottom 8bits
}

uint8_t intenarBH(){ //0xE
    return chipset.intenar >> 8;
}

uint8_t intenarBL(){ //0xE
    return chipset.intenar & 0xFF;
}

uint8_t intreqrBH(){ //0xE
    return chipset.intreqr >> 8;
}

uint8_t intreqrBL(){ //0xE
    return chipset.intreqr & 0xFF;
}













uint8_t (*getChipReg8[])() = {
    noReadB,
    noReadB,
    dmaconrBH,
    dmaconrBL,
    vposrBH,
    vposrBL,
    vhposrBH,
    vhposrBL,
    noReadB,
    noReadB,
    joy0datBH,
    joy0datBL,
    joy1datBH,
    joy1datBL,
    noReadB,
    noReadB,
    noReadB,
    noReadB,
    noReadB,
    noReadB,
    noReadB,
    noReadB,
    potgorBH,
    potgorBL,
    noReadB,
    noReadB,
    noReadB,
    noReadB,
    intenarBH,
    intenarBL,
    intreqrBH,
    intreqrBL
};

uint32_t (*getChipReg32[])() = {
    noReadL,
    noReadL,
    vposrL,
    noReadL,
    noReadL,
    noReadL,
    noReadL,
    noReadL,
    noReadL,
    noReadL,
    noReadL,
    noReadL,
    noReadL,
    noReadL,
    noReadL,
    noReadL
};


uint16_t (*getChipReg16[])(void) = {
    bltddat,
    dmaconr,
    vposr,
    vhposr,
    dskdatr,
    joy0dat,
    joy1dat,
    clxdat,
    adkconr,
    pot0dat,
    pot1dat,
    potgor,
    serdatr,
    dskbytr,
    intenar,
    intreqr,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    noRead,
    deniseid,
};














































void (*putChipReg32[])(uint32_t) ={
    longWrite,
    noopL,
    longWrite,
    longWrite,
    noopL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    noopL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    dskpthL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    vposwL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    bltcon0L,
    longWrite,
    bltafwmL,
    longWrite,
    bltcpthL,
    longWrite,
    bltbpthL,
    longWrite,
    bltapthL,
    longWrite,
    bltdpthL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    bltcmodL,
    bltbmodL,
    bltamodL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    bltbdatL,
    longWrite,
    noopL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    cop1lchL,
    longWrite,
    cop2lchL,
    longWrite,
    longWrite,
    copjmp2L,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    aud0lcL,
    longWrite,
    longWrite,
    aud0perL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    aud1lcL,
    longWrite,
    longWrite,
    aud1perL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    aud2lcL,
    longWrite,
    longWrite,
    aud2perL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    aud3lcL,
    longWrite,
    longWrite,
    aud3perL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    color00L,
    longWrite,
    color02L,
    longWrite,
    color04L,
    longWrite,
    color06L,
    longWrite,
    color08L,
    longWrite,
    color10L,
    longWrite,
    color12L,
    longWrite,
    color14L,
    longWrite,
    color16L,
    longWrite,
    color18L,
    longWrite,
    color20L,
    longWrite,
    color22L,
    longWrite,
    color24L,
    longWrite,
    color26L,
    longWrite,
    color28L,
    longWrite,
    color30L,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    vbstrtL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    noopL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    noopL
};


























































































































































































































































































































































































































































void (*putChipReg16[])(uint16_t) ={
    wordIllegalWrite,
    wordIllegalWrite,
    wordIllegalWrite,
    wordIllegalWrite,
    wordIllegalWrite,
    wordIllegalWrite,
    wordIllegalWrite,
    wordIllegalWrite,
    wordIllegalWrite,
    wordIllegalWrite,
    wordIllegalWrite,
    wordIllegalWrite,
    wordIllegalWrite,
    wordIllegalWrite,
    wordIllegalWrite,
    wordIllegalWrite,
    dskpth,
    dskptl,
    dsklen,
    dskdat,
    wordWrite,
    vposw,
    vhposw,
    copcon,
    serdat,
    serper,
    potgo,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    bltcon0,
    bltcon1,
    bltafwm,
    bltalwm,
    bltcpth,
    bltcptl,
    bltbpth,
    bltbptl,
    bltapth,
    bltaptl,
    bltdpth,
    bltdptl,
    bltsize,
    bltcon0l,
    bltsizv,
    bltsizh,
    bltcmod,
    bltbmod,
    bltamod,
    bltdmod,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    bltcdat,
    bltbdat,
    bltadat,
    noop,
    wordWrite,
    wordWrite,
    wordWrite,
    dsksync,
    cop1lch,
    cop1lcl,
    cop2lch,
    cop2lcl,
    copjmp1,
    copjmp2,
    wordWrite,
    diwstrt,
    diwstop,
    ddfstrt,
    ddfstop,
    dmacon,
    clxcon,
    intena,
    intreq,
    adkcon,
    aud0lch,
    aud0lcl,
    aud0len,
    aud0per,
    aud0vol,
    aud0dat,
    noop,
    noop,
    aud1lch,
    aud1lcl,
    aud1len,
    aud1per,
    aud1vol,
    aud1dat,
    noop,
    noop,
    aud2lch,
    aud2lcl,
    aud2len,
    aud2per,
    aud2vol,
    aud2dat,
    noop,
    noop,
    aud3lch,
    aud3lcl,
    aud3len,
    aud3per,
    aud3vol,
    aud3dat,
    noop,
    noop,
    bpl1pth,
    bpl1ptl,
    bpl2pth,
    bpl2ptl,
    bpl3pth,
    bpl3ptl,
    bpl4pth,
    bpl4ptl,
    bpl5pth,
    bpl5ptl,
    bpl6pth,
    bpl6ptl,
    bpl7pth,
    bpl7ptl,
    bpl8pth,
    bpl8ptl,
    bplcon0,
    bplcon1,
    bplcon2,
    bplcon3,
    bpl1mod,
    bpl2mod,
    bplcon4,
    wordWrite,
    bpl1dat,
    bpl2dat,
    bpl3dat,
    bpl4dat,
    bpl5dat,
    bpl6dat,
    bpl7dat,
    bpl8dat,
    spr0pth,
    spr0ptl,
    spr1pth,
    spr1ptl,
    spr2pth,
    spr2ptl,
    spr3pth,
    spr3ptl,
    spr4pth,
    spr4ptl,
    spr5pth,
    spr5ptl,
    spr6pth,
    spr6ptl,
    spr7pth,
    spr7ptl,
    spr0pos,
    spr0ctl,
    wordWrite,
    wordWrite,
    spr1pos,
    spr1ctl,
    wordWrite,
    wordWrite,
    spr2pos,
    spr2ctl,
    wordWrite,
    wordWrite,
    spr3pos,
    spr3ctl,
    wordWrite,
    wordWrite,
    spr4pos,
    spr4ctl,
    wordWrite,
    wordWrite,
    spr5pos,
    spr5ctl,
    wordWrite,
    wordWrite,
    spr6pos,
    spr6ctl,
    wordWrite,
    wordWrite,
    spr7pos,
    spr7ctl,
    spr7data,
    wordWrite,
    color00,
    color01,
    color02,
    color03,
    color04,
    color05,
    color06,
    color07,
    color08,
    color09,
    color10,
    color11,
    color12,
    color13,
    color14,
    color15,
    color16,
    color17,
    color18,
    color19,
    color20,
    color21,
    color22,
    color23,
    color24,
    color25,
    color26,
    color27,
    color28,
    color29,
    color30,
    color31,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    vtotal,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    beamcon0,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    noop,       //Not sure why the Copper writes to this address during boot?
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    fmode,
    noop
};

void eclock_execute(Chipset_t* chipset){
    
    /*
    The function is called every DMA slot, which on a real Amiga lasts 280ns.
    This would be about 3,571,428hz (i.e. 3 million times per second), which is 5 times the eclock frequency
    So this function counts down from 5, and then increments the eClock, at a rate of aproximately 715,909hz. 
     
     */
    internal.eClockCounter -=1;
    
    if(internal.eClockCounter<0){
        internal.eClockCounter = 4;
        
        //CIA
        CIAExecute(&CIAA);
        CIAExecute(&CIAB);
        
    }
    
}



