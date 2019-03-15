//
//  Host.h
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

#ifndef Host_h
#define Host_h

#include <stdio.h>
#include "SDL2/SDL.h"

typedef struct{
    int FBCounter; //must be reset every VBL
    int vblCount;
    
    //SDL specific
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* commodoreLogo;
    SDL_Texture* playfield;
    SDL_Texture* spinner;
    SDL_Event event;
    
    void* pixels; //pointer to physical display frame buffer
}Host_t;

extern Host_t host;
extern SDL_atomic_t cpuWait;

void hiresPlanar2Chunky(uint32_t* pixBuff,uint32_t* palette,uint16_t plane1, uint16_t plane2,uint16_t plane3,uint16_t plane4);
void loresPlanar2Chunky(uint32_t* pixBuff,uint32_t* palette,uint16_t plane1, uint16_t plane2,uint16_t plane3,uint16_t plane4,uint16_t plane5,uint16_t plane6);
void loresHAM2Chunky(uint32_t* pixBuff,uint32_t* palette,uint16_t plane1, uint16_t plane2,uint16_t plane3,uint16_t plane4,uint16_t plane5,uint16_t plane6);

void sprite2chunky(uint32_t* pixBuff,uint32_t* palette, int x,uint16_t plane1, uint16_t plane2,int delta);

void pressKey(uint16_t keyCode);
void releaseKey(uint16_t keyCode);

void hostInit();

void hostDisplay();

void toggleLEDs();

#endif /* Host_h */
