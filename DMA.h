//
//  DMA.h
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

#ifndef DMA_h
#define DMA_h

#include <stdio.h>

void dma_execute();
int copperExecute();
int blitterExecute();

void evenCycle(void);
void oddCycle(void);
void dramCycle(void);
void diskCycle(void);
void audioCycle(void);
void spriteCycle(void);
void bitplaneCycle0_4(void);
void bitplaneCycle4_2(void);
void bitplaneCycle6_3(void);
void bitplaneCycle2_1(void);
void bitplaneCycle0_4(void);
void bitplaneCycle3_2(void);
void bitplaneCycle5_3(void);
void bitplaneCycle1(void);

int blitterCopyCycle();
int blitterLineCycle();

#endif /* DMA_h */
