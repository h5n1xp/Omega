//
//  Floppy.h
//  Omega2
//
//  Created by Matt Parsons on 05/04/2022.
//

#ifndef Floppy_h
#define Floppy_h

#include <stdio.h>
#include "Chipset.h"

void FloppyInit(void);
void FloppyReset(void);

void FloppyInsert(int number, uint8_t* ADFdata);
void FloppyEject(int number);

void FloppyReadTrack(uint32_t DSKPTR, int len);

void FloppyCycle(void);

void FloppyExecute(Chipset_t* Chipstate);

#endif /* Floppy_h */
