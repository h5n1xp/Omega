//
//  Chipset.c
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

#include "Chipset.h"
#include "CIA.h"
#include "m68k.h"

Chipset_t chipset;
Internal_t internal;

void ChipsetInit(){
    
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
    

    
}

uint32_t OCS2ARGB(uint16_t color){
    
    uint32_t value  =  ((color <<12)& 0xF00000) | ((color <<8)&0xF000) | ((color << 4)&0xF0); // high nybble.
    value |= (value >> 4);// low nyble
    value  = value | 0xFF000000; //opaque alpha
    return value;
}

uint32_t EHB2ARGB(uint16_t color){
    
    uint32_t value  =  ((color <<11)& 0xF00000) | ((color <<7)&0xF000) | ((color << 3)&0xF0); // high nybble.
    value |= (value >> 4);// low nyble
    value  = value | 0xFF000000; //opaque alpha
    return value;
}



//***********************************************************************************************
//***********************************************************************************************
//***********************************************************************************************
//Nu Emualtor design functions ***************************************************************************


uint16_t noRead(void){
    printf("can't read a Write only Register\n");
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
    chipset.cop1lc = value;
    //printf("(32bit)Copper 1: %0x\n",chipset.cop1lc);
    //dissassembleCopperList(value, 256);
}

void cop2lchL(uint32_t value){  //
    
    if(value==0){
        return;
    }
    
    chipset.cop2lc = value;
    //printf("(32bit)Copper 2: %0x\n",chipset.cop2lc);
    //dissassembleCopperList(value, 256);
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
    printf("Need to implement Long Write\n");
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
    printf("16bit Write: Not implemented!\n");
}

void dskpth(uint16_t value){
    chipset.dskpt  = (value << 16) | (chipset.dskpt & 0x0000FFFF); //using the new 32bit register
}

void dskptl(uint16_t value){
    chipset.dskpt  =  value        | (chipset.dskpt & 0xFFFF0000); //using the new 32bit register
}

void dsklen(uint16_t value){
    static int dskDMALock = 0; //the dsklen register need to be written twice to update
    
    dskDMALock +=1;

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
    chipset.bltbpt =  value        | (chipset.bltbpt & 0xFFFF0000);//using the new 32bit register
}
void bltapth(uint16_t value){
    chipset.bltapt = (value << 16) | (chipset.bltapt & 0x0000FFFF); //using the new 32bit register
}
void bltaptl(uint16_t value){

    chipset.bltapt =  (chipset.bltapt & 0xFFFF0000);//using the new 32bit register
    chipset.bltapt = chipset.bltapt | value;

}
void bltdpth(uint16_t value){
    chipset.bltdpt = (value << 16) | (chipset.bltdpt & 0x0000FFFF); //using the new 32bit register
}
void bltdptl(uint16_t value){
    chipset.bltdpt =  value        | (chipset.bltdpt & 0xFFFF0000);//using the new 32bit register
}

void bltsize(uint16_t value){
    chipset.bltsizv = value >> 6;
    chipset.bltsizh = value & 63;

    if(chipset.bltsizv == 0){
        chipset.bltsizv=1024;
    }

    if(chipset.bltsizh == 0){
        chipset.bltsizh=64;
    }

    //use the old OCS register to keep a copy of the blit width...not needed anymore
    chipset.bltsize = chipset.bltsizh;

    //Set to blitter busy
    chipset.dmaconr =chipset.dmaconr | 16384;
}

void bltsizv(uint16_t value){
    chipset.bltsizv = value;
}
void bltsizh(uint16_t value){
    chipset.bltsizh = value;
    chipset.bltsize = chipset.bltsizh;   //use the old OCS register to keep a copy of the blit width.
    
    //Set to blitter busy
    chipset.dmaconr =chipset.dmaconr | 16384;
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

void cop1lch(uint16_t value){
    chipset.cop1lc = (chipset.cop1lc & 0x0000FFFF) | ((value & 31) <<16); //only 5 bits as this is a weird ECS/OCS hibrid
}
void cop1lcl(uint16_t value){
    chipset.cop1lc = (chipset.cop1lc & 0xFFFF0000) |  (value & 65535);
    printf("(16bit)Copper 1: %0x\n",chipset.cop1lc);
}
void cop2lch(uint16_t value){
    chipset.cop2lc = (chipset.cop2lc & 0x0000FFFF) | ((value & 31) <<16); //only 5 bits as this is a weird ECS/OCS hibrid
}
void cop2lcl(uint16_t value){
    chipset.cop2lc = (chipset.cop2lc & 0xFFFF0000) |  (value & 65535);
    printf("(16bit)Copper 2: %0x\n",chipset.cop2lc);
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

void diwstrt(uint16_t value){
    chipset.diwstrt = value;
    
}

void diwstop(uint16_t value){
    chipset.diwstop = value;
    
}


void ddfstrt(uint16_t value){
    chipset.ddfstrt = value;
}

void ddfstop(uint16_t value){
    chipset.ddfstop = value;
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
    
}

void intreq(uint16_t value){
    if( (value & 32768) ==0){
        chipset.intreqr = chipset.intreqr ^ (chipset.intreqr & value);
    }else{
        chipset.intreqr = chipset.intreqr | (value  & 32767);
    }
    
    //experimental Interrupt handler
    chipset.intreqr &= (chipset.intenar | 0x4000) ;
}

void adkcon(uint16_t value){
    if( (value & 32768) ==0){
        chipset.adkconr = chipset.adkconr ^ (chipset.adkconr & value);
    }else{
        chipset.adkconr = chipset.adkconr | (value  & 32767);
    }
}

void aud0vol(uint16_t value){
    chipset.aud0vol = value;
}

void aud3vol(uint16_t value){
    chipset.aud0vol = value;
}

void bpl1pth(uint16_t value){
    chipset.bpl1pt = (value << 15) | (chipset.bpl1pt & 0x00007FFF); //this is only shifted by 15 because all addresses are word aligend
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
    chipset.bplcon0 = value;
    
    int planes = (value >> 12) & 7 ;
    
    switch(planes){
        case 0: internal.bitplaneMask  = 0x00;break;
        case 1: internal.bitplaneMask  = 0x01;break;
        case 2: internal.bitplaneMask  = 0x03;break;
        case 3: internal.bitplaneMask  = 0x07;break;
        case 4: internal.bitplaneMask  = 0x0F;break;
        case 5: internal.bitplaneMask  = 0x1F;break;
        default: internal.bitplaneMask = 0x0;break;
    }
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

void noop(uint16_t value){
    
}

//***********************************************************************************************





uint32_t noReadL(void){
    printf("can't do a 32bit read from Register yet \n");
    return 0;
}


uint32_t vposrL(){  //0x2
    return chipset.vposr << 16 | chipset.vhposr;
}

uint8_t noReadB(void){
    printf("can't do an 8bit read from Register yet \n");
    return 0;
}

uint8_t dmaconrB(){ // 0x1
    return chipset.dmaconr >> 8; // only read the top 8bits
}

uint8_t vhposrB(){  // 0x3
    return chipset.vhposr >> 8; //internal.vPos;  //vPos is the equilivent to the top byte
}

uint8_t joy0datB(){ // 0x5
    return chipset.joy0dat >> 8; // only read the top 8bits
}

uint8_t joy1datB(){ // 0x6
    return chipset.joy1dat >> 8; // only read the top 8bits
}

uint8_t potgorB(){ //  0xB
    return chipset.potinp >> 8; // only read the top 8bits
}

uint8_t intenarB(){ //0xE
    return chipset.intenar >> 8;
}

uint8_t (*getChipReg8[])() = {
    noReadB,
    dmaconrB,
    noReadB,
    vhposrB,
    noReadB,
    joy0datB,
    joy1datB,
    noReadB,
    noReadB,
    noReadB,
    noReadB,
    potgorB,
    noReadB,
    noReadB,
    intenarB,
    noReadB
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
    longWrite,
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
    longWrite,
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
    aud0perL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    aud1perL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    aud2perL,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
    longWrite,
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
    wordWrite,
    noop,
    wordWrite,
    wordWrite,
    noop,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
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
    wordWrite,
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
    wordWrite,
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
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    aud0vol,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    aud3vol,
    wordWrite,
    wordWrite,
    wordWrite,
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
    wordWrite,
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
    wordWrite,
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
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
    wordWrite,
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
    wordWrite,
    noop
};



void checkInterrupt(Chipset_t* chipset){
    
    int intreqr =chipset->intreqr;
    
    
    if(chipset->intenar & 0x4000){    // if master interrupt switch is enabled
      
        if(intreqr & 0x1){
            m68k_set_irq(1); // Serial Transmit buffer empty
        }
          
        if(intreqr & 0x2){
            m68k_set_irq(1); // Disk block finished
        }
        
        if(intreqr & 0x4){
            m68k_set_irq(1); // Software int
        }
        
        if(intreqr & 0x8){
            m68k_set_irq(2); // Ports, IO and timers
        }
        
        if(intreqr & 0x10){
            m68k_set_irq(3); // Copper
        }
        
        if(intreqr & 0x20){
            m68k_set_irq(3); // VBL
        }
        
        if(intreqr & 0x40){
            m68k_set_irq(3); // Blitter finished
        }
        
        if(intreqr & 0x80){
            m68k_set_irq(4); // Audio 0
        }
        
        if(intreqr & 0x100){
            m68k_set_irq(4); // Audio 1
        }
        
        if(intreqr & 0x200){
            m68k_set_irq(4); // Audio 2
        }
        
        if(intreqr & 0x400){
            m68k_set_irq(4); // Audio 3
        }
        
        if(intreqr & 0x800){
            m68k_set_irq(5); // Serial receive buffer full
        }
        
        if(intreqr & 0x1000){
            m68k_set_irq(5); // Disk sync
        }
        
        if(intreqr & 0x2000){
            m68k_set_irq(6); // External Int
        }
    
    }
    
    return;
    
    // The old interrupt handling code... works the same as the above code, no idea which is better :-/
    
    if(chipset->intenar & 16384){ // if master interrupt switch is enabled
        
        uint16_t intMask = chipset->intreqr & chipset->intenar; //only set the int level, if bits are enabled
        
        if(intMask !=0){
        
            if(intMask & 8192){  // External int
                m68k_set_irq(6);
            }else if(intMask & 4096){ // Disk sync
                m68k_set_irq(5);
            }else if(intMask & 2048){ // Serial receive buffer full
                m68k_set_irq(5);
            }else if(intMask & 1024){ // Audio 3
                m68k_set_irq(4);
            }else if(intMask & 512){  // Audio 2
                m68k_set_irq(4);
            }else if(intMask & 256){  // Audio 1
                m68k_set_irq(4);
            }else if(intMask & 128){  // Audio 0
                m68k_set_irq(4);
            }else if(intMask & 64){   // Blitter finished
                m68k_set_irq(3);
            }else if(intMask & 32){   // VBL
                m68k_set_irq(3);
            }else if(intMask & 16){   // Copper
                m68k_set_irq(3);
            }else if(intMask & 8){    // Ports, IO and timers
                m68k_set_irq(2);
            }else if(intMask & 4){    // Software int
                m68k_set_irq(1);
            }else if(intMask & 2){    // Disk block finished
                m68k_set_irq(1);
            }else if(intMask & 1){    // Serial Transmit buffer empty
                m68k_set_irq(1);
            }
        }
    }
    
}

void eclock_execute(Chipset_t* chipset){
    
    internal.eClockCounter -=1;
    
    if(internal.eClockCounter<0){
        internal.eClockCounter = 6; //with a 200line display aproximates the Eclock frequency
        
        //CIA
        CIAExecute(&CIAA);
        CIAExecute(&CIAB);
        
    }
    
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
        delay = 5;
        */
        
        
        if(chipset->bltcon1 & 1){
            //Line Mode
            
            //printf("Blitter Line Mode\n");
            
            int octCode = (chipset->bltcon1 >> 2) & 7;
            int length = chipset->bltsizv;
            int inc1 = chipset->bltamod; // 4(dy - dx)
            int D = (int16_t)chipset->bltapt;     // start value of 4dy - 2dx
            uint16_t* chipramW = internal.chipramW;
            
            int planeAddr = chipset->bltcpt & 0x1FFFFE;//word address
            
            int planeMod  = chipset->bltcmod;
            int inc2 = chipset->bltbmod;// 4dy
            int d=0;

            int startPixel = chipset->bltcon0 >>12;
            
            int oneDot = (chipset->bltcon1 >>1) & 1;    // I don't support one dot mode yet
            
            if(oneDot==1){
                printf("No Single H-line mode yet\n");
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
            

            //printf("Blitter Copy Mode\n");
            
            //Area Copy Blitter
            //printf("blit: A-%06x B-%06x C-%06x D-%06x W-%d H-%d\n",chipset->bltapt,chipset->bltbpt,chipset->bltcpt,chipset->bltdpt,chipset->bltsizh,chipset->bltsizv);
            
            int useMask = chipset->bltcon0 >> 8;
            int shiftA = chipset->bltcon0 >> 12;
            int shiftB = chipset->bltcon1 >> 12;
            int minterm = chipset->bltcon0 & 255;
            int xIncrement = 1;
            int fillmode = (chipset->bltcon1 & 0x18) >> 3; // 1= Inclusive Fill, 2 = Exclusive Fill
        
            
            //if descend mode is on.
            if((chipset->bltcon1 & 2)){
                xIncrement = -1;
                
                if(fillmode>0){
                    printf("NO FILL MODE YET!!");
                }
                
            }
            
            
            //all pointers are word addressed and masked for 2meg only
            int apt=(chipset->bltapt >> 1);
            int bpt=(chipset->bltbpt >> 1);
            int cpt=(chipset->bltcpt >> 1);
            int dpt=(chipset->bltdpt >> 1);
            
            
            int amod=chipset->bltamod / 2; //these are signed so need to be divided not shifted
            int bmod=chipset->bltbmod / 2;
            int cmod=chipset->bltcmod / 2;
            int dmod=chipset->bltdmod / 2;
            
            int adat=chipset->bltadat;
            int bdat=chipset->bltbdat;
            int cdat=chipset->bltcdat;
            
            uint16_t afwm = chipset->bltafwm;
            uint16_t alwm = chipset->bltalwm;
            
            int sizeh = chipset->bltsizh;
            int sizev = chipset->bltsizv;
            int lastHWord=sizeh - 1;
            
        
            
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
                    
                    //shifting section
                    if(xIncrement==-1){
                        
                            channelA = (previousA >> (16-shiftA)) | (channelA << shiftA);
                            channelB = (previousB >> (16-shiftB)) | (channelB << shiftB);
                        
                    }else{
                        
                            channelA = (previousA << (16-shiftA)) | (channelA >> shiftA);
                            channelB = (previousB << (16-shiftB)) | (channelB >> shiftB);
                        
                    }
                    previousA = adat;
                    previousB = bdat;
                    
                    
                    channelD = logicFunction(minterm, channelA, channelB, cdat);
                    
                    //Zero Flag
                    if(channelD==0){
                        chipset->dmaconr = chipset->dmaconr | 0x2000;   //set zero flag
                    }else{
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
            
            chipset->dmaconr = chipset->dmaconr & 0xBFFF; //clear blitter busy bit
            putChipReg16[INTREQ](0x8040); // generate an interrupt!
            return;
        }
    }
    
}

