//
//  CIA.h
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

#ifndef CIA_h
#define CIA_h

#include <stdio.h>
#include "Chipset.h"

typedef struct{
    unsigned char Name[2];
    uint8_t  pra;
    uint8_t  prb;   //read port
    uint8_t  prbw;  //write  port
    uint8_t	 ddra;
    uint8_t	 ddrb;
     int32_t ta;
     int32_t tb;
    uint32_t tod;
    uint8_t	 sdr;
    uint8_t	 icr;
    uint8_t	 cra;
    uint8_t	 crb;
    
    uint16_t taLatch;
    uint16_t tbLatch;
    uint32_t todAlarm;
    uint8_t  icrMask;
    uint16_t chipInt;    // CIA A = 32776; CIA B = 40960
    
    int irqLatch;
    int prbChanged; // flag to indicate the CPU has altered a value in this register.
    
}CIA_t;

extern CIA_t CIAA;
extern CIA_t CIAB;
extern int disass;

void CIAInit(CIA_t*,uint16_t);    //reset the CIA and set INTREQ value;
void CIAWrite(CIA_t*,int,uint8_t);//Data,byte address,data
uint8_t CIARead(CIA_t*,int);      //Data,byteaddress
void CIAExecute(CIA_t*);      //returns INTREQ value if interupt, or 0 for nothing.

void CIATODEvent(CIA_t*);         //called every TOD event

void CIAIndex(CIA_t*);      //called every floppy disk rotation

void keyboardInt();

#endif /* CIA_h */

