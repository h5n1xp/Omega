//
//  Host.c
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

#include "Host.h"

#include "Chipset.h"
#include "CIA.h"
#include "Floppy.h"

Host_t host;

void hostDisplay(){
    
    int x;
    int y;
    CIAA.pra |= 64;
    chipset.potinp |= 1024;
    int button = SDL_GetRelativeMouseState(&x, &y);
    
    //left mouse
    if(button == 1){
        //printf("%X: %d,%d\n",button,x,y);
        CIAA.pra &= 63;
    }
    
    //right mouse
    if(button == 4){
        //printf("%X: %d,%d\n",button,x,y);
        chipset.potinp &=0xFBFF;
    }
    
    uint8_t tx = chipset.joy0dat & 255;
    uint8_t ty = chipset.joy0dat >> 8;
    
    tx +=(x*2);
    ty +=(y*2);
    
    chipset.joy0dat = (ty << 8 | (tx & 255));
    
    int pitch = 640*4;
    int winX;
    int winY;
    SDL_GetWindowSize(host.window, &winX, &winY);
    
    //Draw Amiga graphics
    SDL_UnlockTexture(host.playfield);
    
    SDL_Rect oops ={0,0,640,200};
    if(internal.bitplaneMask ==0){
        SDL_SetRenderDrawColor(host.renderer, (internal.palette[0]>>16)&255, (internal.palette[0]>>8)&255, (internal.palette[0])&255, 255);
            SDL_RenderClear(host.renderer);
    }

    SDL_RenderCopy(host.renderer, host.playfield, &oops,NULL);
    
    //Power Light
    switch ((CIAA.pra &3)) {
        case 0:SDL_SetRenderDrawColor(host.renderer, 255, 255, 31, 255);break;
        case 1:SDL_SetRenderDrawColor(host.renderer, 31, 31, 31, 255);break;
        case 2:SDL_SetRenderDrawColor(host.renderer, 182, 182, 31, 255);break;
        case 3:SDL_SetRenderDrawColor(host.renderer, 31, 31, 31, 255);break;
    }
    
    SDL_Rect pLED ={winX-55,winY-40,50,7};
    SDL_RenderFillRect(host.renderer, &pLED);
    
    //Floppy Light
    if( (CIAB.prb & 128) == 0){
        SDL_SetRenderDrawColor(host.renderer, 255, 255, 31, 255);
    }else{
        SDL_SetRenderDrawColor(host.renderer, 31, 31, 31, 255);
    }
    
    SDL_Rect dLED ={winX-55,winY-20,50,7};
    SDL_RenderFillRect(host.renderer, &dLED);
    
    
    //Commodore Logo
    SDL_Rect clo={winX-55,winY-100,50,50};
    SDL_RenderCopy(host.renderer, host.commodoreLogo, NULL, &clo);
    
    //Spinny disk
    SDL_Rect spin={5,winY-60,50,50};
    double angle = ((double)df0.index/6400.0)*365.0;
    SDL_RenderCopyEx(host.renderer, host.spinner, NULL, &spin,angle,NULL,SDL_FLIP_NONE);
    
    //Disk head
    SDL_SetRenderDrawColor(host.renderer, 255, 255, 255, 255);
    SDL_Rect head = {29,(winY-59)+(df0.diskTrack/6),3,3};
    SDL_RenderFillRect(host.renderer,&head);
    
    //render display
    SDL_RenderPresent(host.renderer);
    SDL_LockTexture(host.playfield, NULL, &host.pixels, &pitch);

    
    
}

void planar2chunky(uint32_t* pixBuff,uint32_t* palette,uint16_t plane1, uint16_t plane2,uint16_t plane3,uint16_t plane4,uint16_t plane5,int delta){ //delta =8 hires, 16 = lores
    
    //Fetch and display 2 bytes of bitplane data (16 pixels)
    uint32_t colourIndex1 = 0;
    uint32_t colourIndex2 = 0;
    
    for(int j=7;j>-1;--j){
        
        //build 8 chunky pixels from a byte of bitplane data - Don't need to byte swap as the lump below does that
        colourIndex1 =                  (plane1>>j) & 1;
        colourIndex1 = colourIndex1 | (((plane2>>j) & 1) << 1);
        colourIndex1 = colourIndex1 | (((plane3>>j) & 1) << 2);
        colourIndex1 = colourIndex1 | (((plane4>>j) & 1) << 3);
        colourIndex1 = colourIndex1 | (((plane5>>j) & 1) << 4);
        
        int k=j+8;
        //build 8 chunky pixels from a byte of bitplane data
        colourIndex2 =                  (plane1>>k) & 1;
        colourIndex2 = colourIndex2 | (((plane2>>k) & 1) << 1);
        colourIndex2 = colourIndex2 | (((plane3>>k) & 1) << 2);
        colourIndex2 = colourIndex2 | (((plane4>>k) & 1) << 3);
        colourIndex2 = colourIndex2 | (((plane5>>k) & 1) << 4);
        
        
        pixBuff[host.FBCounter] = internal.palette[colourIndex1];           // use SDL texture buffer
        pixBuff[host.FBCounter+delta] = internal.palette[colourIndex2];// use SDL texture buffer
        host.FBCounter +=1;
        
        if(delta==16){
            pixBuff[host.FBCounter] = internal.palette[colourIndex1];           // use SDL texture buffer
            pixBuff[host.FBCounter+delta] = internal.palette[colourIndex2];// use SDL texture buffer
            //
            host.FBCounter +=1;
        }
        
        
    }
    
    
}


void sprite2chunky(uint32_t* pixBuff,uint32_t* palette, int x,uint16_t plane1, uint16_t plane2,int delta){
    
    uint32_t colourIndex1 = 0;
    uint32_t colourIndex2 = 0;
    int counter = x;
    
    //using the x value on the counter enables me to check if the sprite is out of bounds and clip it.
    
    for(int j=7;j>-1;--j){
        
        //build 8 chunky pixels from a byte of bitplane data - Don't need to byte swap as the lump below does that
        colourIndex1 =                  (plane1>>j) & 1;
        colourIndex1 = colourIndex1 | (((plane2>>j) & 1) << 1);
        
        
        int k=j+8;
        //build 8 chunky pixels from a byte of bitplane data
        colourIndex2 =                  (plane1>>k) & 1;
        colourIndex2 = colourIndex2 | (((plane2>>k) & 1) << 1);
        
        if(colourIndex1>0 && counter<640){
            pixBuff[counter] = palette[colourIndex1];
        }
        if(colourIndex2>0 && (counter+delta)<640){
            pixBuff[counter + delta] = palette[colourIndex2];
        }
        counter +=1;
        
        if(delta==16){
            
            if(colourIndex1>0 && counter<640){
                pixBuff[counter] = palette[colourIndex1];
            }
            if(colourIndex2>0 && (counter+delta)<640){
                pixBuff[counter + delta] = palette[colourIndex2];
            }
            counter +=1;
            
            
        }
        
    }
    
    
}

