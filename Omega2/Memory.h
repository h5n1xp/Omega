//
//  Memory.h
//  Omega2
//
//  Created by Matt Parsons on 28/03/2022.
//

#ifndef Memory_h
#define Memory_h

#include <stdio.h>
#include <stdint.h>
#include "Chipset.h"
#include "Omega.h"

#define READBYTE(address) RAM24bit[address]
#define WRITEBYTE(address, value) RAM24bit[address] = value

#define READWORD(address) *(int16_t*)&RAM24bit[address]
#define WRITEWORD(address, value) *(int16_t*)&RAM24bit[address] = value

#define READLONG(address) *(int32_t*)&RAM24bit[address]
#define WRITELONG(address, value) *(int32_t*)&RAM24bit[address] = value

extern char* regNames[];
extern uint8_t* RAM24bit;
enum DataSize{m68kByte, m68kWord, m68kLong};
enum DataDirection{m68kRead, m68kWrite};

unsigned int cpu_read_byte(unsigned int address);
unsigned int cpu_read_word(unsigned int address);
unsigned int cpu_read_long(unsigned int address);
void cpu_write_byte(unsigned int address, unsigned int value);
void cpu_write_word(unsigned int address, unsigned int value);
void cpu_write_long(unsigned int address, unsigned int value);
void cpu_pulse_reset(void);
void cpu_set_fc(unsigned int fc);
int  cpu_irq_ack(int level);
void cpu_instr_callback(void);

unsigned int cpu_read_word_dasm(unsigned int address);
unsigned int cpu_read_long_dasm(unsigned int address);

Omega_t* InitRAM(int RAM32bitSize);
void BigEndianWrite(unsigned int address, enum DataSize size, unsigned int value);

void printCPUContext(void);

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
    
}CSWindow_t;

#endif /* Memory_h */
