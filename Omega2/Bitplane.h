//
//  Bitplane.h
//  Omega2
//
//  Created by Matt Parsons on 28/04/2022.
//

#ifndef Bitplane_h
#define Bitplane_h

#include <stdio.h>
#include "Chipset.h"


void BitplaneExecute(Chipset_t* ChipsetState);
void BitplaneExecuteHires(Chipset_t* ChipsetState);
void BitplaneExecuteLores(Chipset_t* ChipsetState);

#endif /* Bitplane_h */
