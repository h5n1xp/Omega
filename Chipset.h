//
//  Chipset.h
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

#ifndef Chipset_h
#define Chipset_h

#include <stdio.h>
#include <stdint.h>

#define CHIPBASE 0xDFF000
#define BLTDDAT  0x0
#define DMACONR  0x1
#define VPOSR    0x2
#define VHPOSR   0x3
#define DSKDATR  0x4
#define JOY0DAT  0x5
#define JOY1DAT  0x6
#define CLXDAT   0x7
#define ADKCONR  0x8
#define POT0DAT  0x9
#define POT1DAT  0xA
#define POTINP   0xB
#define SERDATR  0xC
#define DSKBYTR  0xD
#define INTENAR  0xE
#define INTREQR  0xF
#define DSKPTH  0x10
#define DSKPTL  0x11
#define DSKLEN  0x12
#define DSKDAT  0x13
#define REFPTR  0x14
#define VPOSW   0x15
#define VHPOSW  0x16
#define COPCON  0x17
#define SERDAT  0x18
#define SERPER  0x19
#define POTGO   0x1A
#define JOYTEST 0x1B
#define STREQU  0x1C
#define STRVBL  0x1D
#define STRHOR  0x1E
#define STRLONG 0x1F
#define BLTCON0 0x20
#define BLTCON1 0x21
#define BLTAFWM 0x22
#define BLTALWM 0x23
#define BLTCPTH 0x24
#define BLTCPTL 0x25
#define BLTBPTH 0x26
#define BLTBPTL 0x27
#define BLTAPTH 0x28
#define BLTAPTL 0x29
#define BLTDPTH 0x2A
#define BLTDPTL 0x2B
#define BLTSIZE 0x2C
#define BLTCON0L 0x2D
#define BLTSIZV 0x2E
#define BLTSIZH 0x2F
#define BLTCMOD 0x30
#define BLTBMOD 0x31
#define BLTAMOD 0x32
#define BLTDMOD 0x33
#define BLTCDAT 0x38
#define BLTBDAT 0x39
#define BLTADAT 0x3A
#define SPRHDAT 0x3C
#define BPLHDAT 0x3D
#define DENISEID 0x3E
#define DSKSYNC 0x3F
#define COP1LCH 0x40
#define COP1LCL 0x41
#define COP2LCH 0x42
#define COP2LCL 0x43
#define COPJMP1 0x44
#define COPJMP2 0x45
#define COPINS  0x46
#define DIWSTRT 0x47
#define DIWSTOP 0x48
#define DDFSTRT 0x49
#define DDFSTOP 0x4A
#define DMACON  0x4B
#define CLXCON  0x4C
#define INTENA  0x4D
#define INTREQ  0x4E
#define ADKCON  0x4F
#define AUD0LCH 0x50
#define AUD0LCL 0x51
#define AUD0LEN 0x52
#define AUD0PER 0x53
#define AUD0VOL 0x54
#define AUD0DAT 0x55
#define AUD1LCH 0x58
#define AUD1LCL 0x59
#define AUD1LEN 0x5A
#define AUD1PER 0x5B
#define AUD1VOL 0x5C
#define AUD1DAT 0x5D
#define AUD2LCH 0x60
#define AUD2LCL 0x61
#define AUD2LEN 0x62
#define AUD2PER 0x63
#define AUD2VOL 0x64
#define AUD2DAT 0x65
#define AUD3LCH 0x68
#define AUD3LCL 0x69
#define AUD3LEN 0x6A
#define AUD3PER 0x6B
#define AUD3VOL 0x6C
#define AUD3DAT 0x6D
#define BPL1PTH 0x70
#define BPL1PTL 0x71
#define BPL2PTH 0x72
#define BPL2PTL 0x73
#define BPL3PTH 0x74
#define BPL3PTL 0x75
#define BPL4PTH 0x76
#define BPL4PTL 0x77
#define BPL5PTH 0x78
#define BPL5PTL 0x79
#define BPL6PTH 0x7A
#define BPL6PTL 0x7B
#define BPL7PTH 0x7C
#define BPL7PTL 0x7D
#define BPL8PTH 0x7E
#define BPL8PTL 0x7F
#define BPLCON0 0x80
#define BPLCON1 0x81
#define BPLCON2 0x82
#define BPLCON3 0x83
#define BPL1MOD 0x84
#define BPL2MOD 0x85
#define BPLCON4 0x86
#define CLXCON2 0x87
#define BPL1DAT 0x88
#define BPL2DAT 0x89
#define BPL3DAT 0x8A
#define BPL4DAT 0x8B
#define BPL5DAT 0x8C
#define BPL6DAT 0x8D
#define BPL7DAT 0x8E
#define BPL8DAT 0x8F
#define SPR0PTH 0x90
#define SPR0PTL 0x91
#define SPR1PTH 0x92
#define SPR1PTL 0x93
#define SPR2PTH 0x94
#define SPR2PTL 0x95
#define SPR3PTH 0x96
#define SPR3PTL 0x97
#define SPR4PTH 0x98
#define SPR4PTL 0x99
#define SPR5PTH 0x9A
#define SPR5PTL 0x9B
#define SPR6PTH 0x9C
#define SPR6PTL 0x9D
#define SPR7PTH 0x9E
#define SPR7PTL 0x9F
#define SPR0POS 0xA0
#define SPR0CTL 0xA1
#define SPR0DATA 0xA2
#define SPR0DATB 0xA3
#define SPR1POS 0xA4
#define SPR1CTL 0xA5
#define SPR1DATA 0xA6
#define SPR1DATB 0xA7
#define SPR2POS 0xA8
#define SPR2CTL 0xA9
#define SPR2DATA 0xAA
#define SPR2DATB 0xAB
#define SPR3POS 0xAC
#define SPR3CTL 0xAD
#define SPR3DATA 0xAE
#define SPR3DATB 0xAF
#define SPR4POS 0xB0
#define SPR4CTL 0xB1
#define SPR4DATA 0xB2
#define SPR4DATB 0xB3
#define SPR5POS 0xB4
#define SPR5CTL 0xB5
#define SPR5DATA 0xB6
#define SPR5DATB 0xB7
#define SPR6POS 0xB8
#define SPR6CTL 0xB9
#define SPR6DATA 0xBA
#define SPR6DATB 0xBB
#define SPR7POS 0xBC
#define SPR7CTL 0xBD
#define SPR7DATA 0xBE
#define SPR7DATB 0xBF
#define COLOR00 0xC0
#define COLOR01 0xC1
#define COLOR02 0xC2
#define COLOR03 0xC3
#define COLOR04 0xC4
#define COLOR05 0xC5
#define COLOR06 0xC6
#define COLOR07 0xC7
#define COLOR08 0xC8
#define COLOR09 0xC9
#define COLOR10 0xCA
#define COLOR11 0xCB
#define COLOR12 0xCC
#define COLOR13 0xCD
#define COLOR14 0xCE
#define COLOR15 0xCF
#define COLOR16 0xD0
#define COLOR17 0xD1
#define COLOR18 0xD2
#define COLOR19 0xD3
#define COLOR20 0xD4
#define COLOR21 0xD5
#define COLOR22 0xD6
#define COLOR23 0xD7
#define COLOR24 0xD8
#define COLOR25 0xD9
#define COLOR26 0xDA
#define COLOR27 0xDB
#define COLOR28 0xDC
#define COLOR29 0xDD
#define COLOR30 0xDE
#define COLOR31 0xDF
#define HTOTAL  0xE0
#define HSSTOP  0xE1
#define HBSTRT  0xE2
#define HBSTOP  0xE3
#define VTOTAL  0xE4
#define VSSTOP  0xE5
#define VBSTRT  0xE6
#define VBSTOP  0xE7
#define SPRHSTRT 0xE8
#define SPRHSTOP 0xE9
#define BPLHSTRT 0xEA
#define BPLHSTOP 0xEB
#define HHPOSW  0xEC
#define HHPOSR  0xED
#define BEAMCON0 0xEE
#define HSSTRT  0xEF
#define VSSTRT  0xF0
#define HCENTER 0xF1
#define DIWHIGH 0xF2
#define BPLHMOD 0xF3
#define SPRHPTH 0xF4
#define SPRHPTL 0xF5
#define BPLHPTH 0xF6
#define BPLHPTL 0xF7
#define FMODE   0xFE
#define NOP     0xFF

extern void (*putChipReg8[])(uint8_t);
extern void (*putChipReg16[])(uint16_t);
extern void (*putChipReg32[])(uint32_t);  

extern uint8_t (*getChipReg8[])(void);
extern uint16_t (*getChipReg16[])(void);
extern uint32_t (*getChipReg32[])(void);

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
    
}Chipset_t;

typedef struct{
    
    //internal variables
    uint32_t copperPC;
    int copperCycle;
    uint16_t IR1;
    uint16_t IR2;
    uint16_t comparisonMask;    //comparison mask for wait and skip
    uint16_t LOF; //LOF bit
    
    int audio0Countdown;
    int audio1Countdown;
    int audio2Countdown;
    int audio3Countdown;
    
    int vPos;
    int hPos;
    uint16_t* chipramW; //word addressed
    uint32_t palette[64]; //internally store the palette colours in rgba32, 32 colours (+32 extra half bright)
    int bitplaneMask;
    
    int eClockCounter;  //aproximates a 700,000Hz timer

    //Blitter Internal data
    int useMask;
    int shiftA;
    int shiftB;
    int minterm;
    int xIncrement;
    int fillmode;
    
    //all pointers are word addressed and masked for 2meg only
    int apt;
    int bpt;
    int cpt;
    int dpt;
    
    //these are signed so need to be divided not shifted
    int amod;
    int bmod;
    int cmod;
    int dmod;
    
 //   int lastHWord;
    int sizeh;
    
}Internal_t;


extern Chipset_t  chipset;
extern Internal_t internal;

void ChipsetInit();


void sprite_execute(Chipset_t*);
void eclock_execute(Chipset_t*);

uint16_t logicFunction(int minterm,uint16_t wordA, uint16_t wordB, uint16_t wordC);

#endif /* Chipset_h */

