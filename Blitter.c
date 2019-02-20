//
//  Blitter.c
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
//

#include "Blitter.h"


void blitterCycle(void){
    
}

void (*blitterOperation[]) (void) = {blitterCycle,blitterCycle};

int OblitterExecute(){
    
    static int cycle;
    
    //check if blitter DMA is on
    //Check if blitter needs to run
    
    blitterOperation[cycle]();
    
    
    return 0;
    
    
}
