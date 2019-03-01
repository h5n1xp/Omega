//
//  Blitter.h
//  The Omega Project
//  https://github.com/h5n1xp/Omega
//
//  Created by Matt Parsons on 20/02/2019.
//  Copyright © 2019 Matt Parsons. All rights reserved.
//  <h5n1xp@gmail.com>
//
//
//  This Source Code Form is subject to the terms of the
//  Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef Blitter_h
#define Blitter_h

#include <stdio.h>
#include <stdint.h>
#include "Chipset.h"

//keep internal blitter state in one place.
typedef struct{
    uint16_t sizev;
}Blitter_t;

int OblitterExecute(void);

void blitter_execute(Chipset_t*);

#endif /* Blitter_h */
