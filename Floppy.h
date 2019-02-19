//
//  Floppy.h
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

#ifndef Floppy_h
#define Floppy_h

#include <stdio.h>
#include "CIA.h"
#include "Chipset.h"


typedef struct{
    
    uint8_t ID;         // currently just the drive id code
    
    int diskReady;      // 0 = disk ready;
    int diskTrack;      // current track number; (40 tracks per side)
    int diskProtect;    // 0 = disk protected.
    int diskchange;     // 0 = no disk; 1 = disk inserted
    
    int diskMotor;      // 0 = motor On; 1 = Motor off
    int diskSelected;   // 0 = true
    int diskSide;       // 0 = top side (2 sides per disk)
    int diskDirection;  // 0 = towards centre
    //int diskStep;     // 0 = head is stepping
    
    int userInserted;   // 0 = disk ejected by user.
    int index;          // 6333 (words) per rotation.
    int indexTop;       // usually 6333
    int delay;          // drive motor spins too fast, delay to slow it down.
    int readStart;      // flag to indicate a disk read has started, only really for debugging.
    
    uint8_t data[80][2][11][512];
    uint16_t mfmTrack[11*544];
    uint8_t mfmData[80*2*11*1300];
}Floppy_t;


extern Floppy_t df0;
extern Floppy_t df1;
extern Floppy_t df2;
extern Floppy_t df3;


void floppyInsert(Floppy_t*);
void floppyEject(Floppy_t*);


uint8_t floppyDataRead(Floppy_t* disk);
void floppy_execute(Floppy_t*);



void floppy_init(Floppy_t*,int);
void floppyDisk_execute(Floppy_t* floppy,Chipset_t*, CIA_t*, CIA_t*); //Floppy disk emulation


#endif /* Floppy_h */

