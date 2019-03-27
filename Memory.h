//
//  Memory.h
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

#ifndef Memory_h
#define Memory_h

//#define NOSLOWRAM
//#define THREADED_CPU


//#define CHIPTOP  0x3FFFF // for 256K Chip
//#define CHIPTOP  0x7FFFF // for 512K
//#define CHIPTOP  0xFFFFF // for 1Meg
#define CHIPTOP 0x1FFFFF // for 2Meg

extern unsigned char low16Meg[16777216]; // only the DMA should use this

void loadROM();

unsigned int chipReadByte(unsigned int address);
unsigned int chipReadWord(unsigned int address);
unsigned int chipReadLong(unsigned int address);

void chipWriteByte(unsigned int address,unsigned int value);
void chipWriteWord(unsigned int address,unsigned int value);
void chipWriteLong(unsigned int address,unsigned int value);

#endif /* Memory_h */
