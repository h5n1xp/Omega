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

/*

 cycle 0:   set up blitter;
            if(mode = copy mode){
                cycle = 0;
            }
            if(mode = fill mode){
                cycle = 5;
            }
            if(mode = line mode){
                cycle = -1
            }
 
 cycle 1:  if(A needs load){
                load A;
                cycle = 2;
            }else if{B needs to load){
                load B;
                cycle = 3;
            } else if{C needs to load){
                Load C;
                cycle = 4;
            }
 
 cycle 2:  if{B needs to load){
                load B;
                cycle = 3;
            } else if{C needs to load){
                Load C;
                cycle = 4;
            }
 
 cycle 3:  if{C needs to load){
                Load C;
                cycle = 4;
            }
 
 cycle 4:   mask A;
            Shift A;
            Shift B;
            apply logic functions
            set clear zero flag
            if(Store D needed){
                Store D;
            }
            update v and h counters
            cycle = 1 ; //repeat the operation
 
            if(v & h == end conditon{
                clear blitter busy flag
                Generate interrupt;
                cycle = 0;  // rest the blitter state
            }
 cycle 5:   fill mode
 
 
 */


#include "Blitter.h"

void blitterCycle(void){
    
}

void (*blitterOperation[]) (void) = {blitterCycle,blitterCycle};

int OblitterExecute(){
    
    static int cycle;
    
    //check if blitter DMA is on
    //Check if blitter needs to run (blitter busy flag is set)
    
    blitterOperation[cycle]();
    
    
    return 0;
    
    
}
