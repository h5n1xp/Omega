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


/* UAE-1ADF (ADF_EXT2)
 * W reserved
 * W number of tracks (default 2*80=160)
 *
 * W reserved
 * W type, 0=normal AmigaDOS track, 1 = raw MFM (upper byte = disk revolutions - 1)
 * L available space for track in bytes (must be even)
 * L track length in bits
 *
 */


#ifndef Floppy_h
#define Floppy_h

#include <stdio.h>
#include "CIA.h"
#include "Chipset.h"



typedef struct{
    uint8_t pra;
    uint8_t prb;
    
    int idMode;
    int index;
    int track;
    int side;
    uint8_t mfmData[12798*80*2]; //trackSize * tracks * sides
}Fd_t;

extern Fd_t df[4];

void floppyEject();
uint8_t floppyDataRead();
void floppyInsert(int);
uint8_t* floppyInit(int);
void floppyState();
void floppySetState();


void encodeBlock(uint8_t* source, uint8_t* destination,int size);
uint8_t addClockBits(uint8_t previous, uint8_t value);
void ADF2MFM(int fd, uint8_t* mfm);





#endif /* Floppy_h */

