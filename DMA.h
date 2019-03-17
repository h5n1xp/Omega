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

void waitFreeSlot();

void evenCycle(void);
void oddCycle(void);
void dramCycle(void);
void diskCycle(void);

void audio0Cycle(void);
void audio1Cycle(void);
void audio2Cycle(void);
void audio3Cycle(void);


void spriteCycle(void);


void plane4(void);
void plane6(void);
void plane2(void);
void plane3(void);
void plane5(void);

void loresPlane1(void);
void hiresPlane1(void);

void drawBlank();

void displayLineReset(void);
void setDisplayMode(int); // 0 = lores, 1 = highres

int blitterCopyCycle();
int blitterLineCycle();

#endif /* DMA_h */
