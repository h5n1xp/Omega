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

#include "CPU.h"
#include "Chipset.h"
#include "CIA.h"
#include "Floppy.h"
#include "image.h"

Host_t host;
SDL_atomic_t cpuWait;










































































const uint8_t keyMapping[] ={
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x41,
    0x42,
    0x0,
    0x0,
    0x0,
    0x44,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x45,
    0x0,
    0x0,
    0x0,
    0x0,
    0x40,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x2A,
    0x0,
    0x0,
    0x0,
    0x0,
    0x38,
    0xB,
    0x39,
    0x3A,
    0xA,
    0x01,
    0x02,
    0x03,
    0x04,
    0x05,
    0x06,
    0x07,
    0x08,
    0x09,
    0x0,
    0x29,
    0x52,
    0xC,
    0x54,
    0x55,
    0x56,
    0x57,
    0x58,
    0x59,
    0x46,   //F11 -> Del    //These 2 keys are mapped this way for laptops without Del and Help keys
    0x5F,   //F12 -> Help
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x4E,
    0x4F,
    0x4D,
    0x4C,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x1A,
    0x2B,
    0x1B,
    0x0,
    0x0,
    0x0,
    0x20,
    0x35,
    0x33,
    0x22,
    0x12,
    0x23,
    0x24,
    0x25,
    0x17,
    0x26,
    0x27,
    0x28,
    0x37,
    0x36,
    0x18,
    0x19,
    0x10,
    0x13,
    0x21,
    0x14,
    0x16,
    0x34,
    0x11,
    0x32,
    0x15,
    0x31,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x30,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x63,
    0x60,
    0x64,
    0x66,
    0x0,
    0x61,
    0x65,
    0x67,
    0x0,
    0x0,
    0x0,
    0x0
};

int LEDActive = 1;
int keyMask = 0;       //This will flag when Ctrl-Amiga-Amiga has been pressed to trigger a reset 


void pressKey(uint16_t keyCode){
    CIAWrite(&CIAA, 0xC, ~(keyMapping[keyCode]<<1) );      //place value in register
    keyboardInt();  //raise Serial port interrupt flag
    //printf("keyCode: %d -> %02x (down)\n",keyCode,keyMapping[keyCode]);
    
    //Ctrl
    if(keyCode==224){
        keyMask |=0x1;
    }
    
    //Left Amiga
    if(keyCode==227){
        keyMask |=0x2;
    }
    
    //Right Amiga
    if(keyCode==231){
        keyMask |=0x4;
    }
    
    //Reset key combination pressed
    if(keyMask==0x7){
        cpu_pulse_reset();
    }
    
}

void releaseKey(uint16_t keyCode){
    CIAWrite(&CIAA, 0xC, ~((keyMapping[keyCode] << 1) | 1) ); //place value in register, with key up flag
    keyboardInt();
    //printf("keyCode: %d -> %d (up)\n",keyCode,keyMapping[keyCode]);
    
    //Ctrl
    if(keyCode==224){
        keyMask &=0x6;
    }
    
    //Left Amiga
    if(keyCode==227){
        keyMask &=0x5;
    }
    
    //Right Amiga
    if(keyCode==231){
        keyMask &=0x3;
    }
    
}

uint32_t HAM6(uint8_t hamPixel,uint32_t previousPixel){
    
    int control = hamPixel >> 6;
    
    switch(control){
        case 0:
            return 0;
            break;
    }
    
    return 0;
}

void hostInit(){
   
    SDL_Init(SDL_INIT_EVERYTHING);
    host.window = SDL_CreateWindow("Omega v0.52",
                                   0,//window X position
                                   0,//window Y position
                                   640, 400,
                                   SDL_WINDOW_RESIZABLE);// | SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    host.renderer = SDL_CreateRenderer(host.window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(host.renderer,SDL_BLENDMODE_BLEND);
    
    SDL_ShowCursor(SDL_DISABLE);
    //SDL_SetRelativeMouseMode(SDL_ENABLE);
    
    
    host.playfield = SDL_CreateTexture(host.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 640, 400);
    
    
    
    
    ///* Commodore Logo
    host.commodoreLogo  = SDL_CreateTexture(host.renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
    int pitch = 50*4;
    void* pixels;
    SDL_LockTexture(host.commodoreLogo , NULL, &pixels, &pitch);
    
    uint32_t* pixel =pixels;
    uint32_t* longLog =(uint32_t*) clogo;
    for(int index=0;index<2500;index +=1){
        pixel[index]=longLog[index];
    }
    SDL_UnlockTexture(host.commodoreLogo);
    SDL_SetTextureBlendMode(host.commodoreLogo, SDL_BLENDMODE_BLEND);
    //*/
    
    
    ///* Floppy disk image
    host.spinner  = SDL_CreateTexture(host.renderer, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STREAMING, 50, 50);
    pitch = 50*4;
    pixels =0;
    SDL_LockTexture(host.spinner , NULL, &pixels, &pitch);
    
    pixel =pixels;
    longLog =(uint32_t*) floppyImage;
    for(int index=0;index<2500;index +=1){
        pixel[index]=longLog[index];
    }
    SDL_UnlockTexture(host.spinner);
    SDL_SetTextureBlendMode(host.spinner, SDL_BLENDMODE_BLEND);
    //*/

    
}

void toggleLEDs(){
    LEDActive = 1 - LEDActive;
    
    //Capture mouse if no leds
    if(LEDActive==1){
        SDL_SetRelativeMouseMode(SDL_DISABLE);
    }else{
        SDL_SetRelativeMouseMode(SDL_ENABLE);
    }
}

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
    
    if(LEDActive==1){
    
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
    
        
        SDL_Rect spin={5,winY-60,50,50};
        double angle = ((double)df[0].index/6400.0)*365.0;
        SDL_Rect head = {29,(winY-59)+(df[0].cylinder/6),3,3};
        
        if(df[0].pra & 0x4){
            //Spinny disk
            SDL_RenderCopyEx(host.renderer, host.spinner, NULL, &spin,angle,NULL,SDL_FLIP_NONE);
    
            //Disk head
            SDL_SetRenderDrawColor(host.renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(host.renderer,&head);
        }
        
        if(df[1].pra & 0x4){
            //Spinny disk
            spin.x +=50;
            angle = ((double)df[1].index/6400.0)*365.0;
            SDL_RenderCopyEx(host.renderer, host.spinner, NULL, &spin,angle,NULL,SDL_FLIP_NONE);
        
            //Disk head
            SDL_SetRenderDrawColor(host.renderer, 255, 255, 255, 255);
            head.x +=50;
            head.y = (winY-59)+(df[1].cylinder/6);
            SDL_RenderFillRect(host.renderer,&head);
        }
        
        if(df[2].pra & 0x4){
            //Spinny disk
            spin.x +=50;
            angle = ((double)df[2].index/6400.0)*365.0;
            SDL_RenderCopyEx(host.renderer, host.spinner, NULL, &spin,angle,NULL,SDL_FLIP_NONE);
        
            //Disk head
            SDL_SetRenderDrawColor(host.renderer, 255, 255, 255, 255);
            head.x +=50;
            head.y = (winY-59)+(df[2].cylinder/6);
            SDL_RenderFillRect(host.renderer,&head);
        }
        
        if(df[3].pra & 0x4){
            //Spinny disk
            spin.x +=50;
            angle = ((double)df[3].index/6400.0)*365.0;
            SDL_RenderCopyEx(host.renderer, host.spinner, NULL, &spin,angle,NULL,SDL_FLIP_NONE);
        
            //Disk head
            SDL_SetRenderDrawColor(host.renderer, 255, 255, 255, 255);
            head.x +=50;
            head.y = (winY-59)+(df[3].cylinder/6);
            SDL_RenderFillRect(host.renderer,&head);
        }
    }
    
    //render display
    SDL_RenderPresent(host.renderer);
    SDL_LockTexture(host.playfield, NULL, &host.pixels, &pitch);

    
    
}


void hiresPlanar2Chunky(uint32_t* pixBuff,uint32_t* palette,uint16_t plane1, uint16_t plane2,uint16_t plane3,uint16_t plane4){
    
    int counter = host.FBCounter;
    
    //Fetch and display 2 bytes of bitplane data (16 pixels)
    uint32_t colourIndex1 = 0;
    uint32_t colourIndex2 = 0;
    
    for(int j=7;j>-1;--j){
        
        //build 8 chunky pixels from a byte of bitplane data - Don't need to byte swap as the lump below does that
        colourIndex1 =                  (plane1>>j) & 1;
        colourIndex1 = colourIndex1 | (((plane2>>j) & 1) << 1);
        colourIndex1 = colourIndex1 | (((plane3>>j) & 1) << 2);
        colourIndex1 = colourIndex1 | (((plane4>>j) & 1) << 3);

        
        int k=j+8;
        //build 8 chunky pixels from a byte of bitplane data
        colourIndex2 =                  (plane1>>k) & 1;
        colourIndex2 = colourIndex2 | (((plane2>>k) & 1) << 1);
        colourIndex2 = colourIndex2 | (((plane3>>k) & 1) << 2);
        colourIndex2 = colourIndex2 | (((plane4>>k) & 1) << 3);

        
        pixBuff[counter] = internal.palette[colourIndex1];
        pixBuff[counter+8] = internal.palette[colourIndex2];
        counter +=1;
        
    }
    
    
}

void loresPlanar2Chunky(uint32_t* pixBuff,uint32_t* palette,uint16_t plane1, uint16_t plane2,uint16_t plane3,uint16_t plane4,uint16_t plane5,uint16_t plane6){
    
    int counter = host.FBCounter;
    
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
        colourIndex1 = colourIndex1 | (((plane6>>j) & 1) << 5);
        
        int k=j+8;
        //build 8 chunky pixels from a byte of bitplane data
        colourIndex2 =                  (plane1>>k) & 1;
        colourIndex2 = colourIndex2 | (((plane2>>k) & 1) << 1);
        colourIndex2 = colourIndex2 | (((plane3>>k) & 1) << 2);
        colourIndex2 = colourIndex2 | (((plane4>>k) & 1) << 3);
        colourIndex2 = colourIndex2 | (((plane5>>k) & 1) << 4);
        colourIndex2 = colourIndex2 | (((plane6>>k) & 1) << 5);
        
        uint32_t colour1 = internal.palette[colourIndex1];
        uint32_t colour2 = internal.palette[colourIndex2];
        pixBuff[counter] = colour1;
        pixBuff[counter+16] = colour2;
        counter +=1;
        
        //Double up pixels since lores only has 320 pixels
        pixBuff[counter] = colour1;
        pixBuff[counter+16] = colour2;
        counter +=1;

        
        
    }
    
    
}

void loresHAM2Chunky(uint32_t* pixBuff,uint32_t* palette,uint16_t plane1, uint16_t plane2,uint16_t plane3,uint16_t plane4,uint16_t plane5,uint16_t plane6){
    int counter = host.FBCounter;
    
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
        colourIndex1 = colourIndex1 | (((plane6>>j) & 1) << 5);
        
        //check if we have a ham pixel
        if(colourIndex1 & 0xF0){
            
            int control = colourIndex1 >> 4;
            uint32_t colour = colourIndex1 & 0xF;
            colour = (colour << 4) | colour;
            uint32_t previous = pixBuff[counter-1];
            
            switch(control){
                case 1:
                    previous = (previous & 0xFFFFFF00) | colour;
                    break;
                case 2:
                    colour <<= 16;
                    previous = (previous & 0xFF00FFFF) | colour;
                    break;
                case 3:
                    colour <<= 8;
                    previous = (previous & 0xFFFF00FF) | colour;
                    break;
                    
                    
            }
            
            
            pixBuff[counter] = previous;
            counter +=1;
            pixBuff[counter] = previous;
            counter +=1;
            
            
        }else{
            uint32_t colour = internal.palette[colourIndex1];
            pixBuff[counter] = colour;
            counter +=1;
        
            //Double up pixels since lores only has 320 pixels
            pixBuff[counter] = colour;
            counter +=1;
        }
        
        
    }

    counter = host.FBCounter;
    for(int j=7;j>-1;--j){
        
        int k=j+8;
        //build 8 chunky pixels from a byte of bitplane data
        colourIndex2 =                  (plane1>>k) & 1;
        colourIndex2 = colourIndex2 | (((plane2>>k) & 1) << 1);
        colourIndex2 = colourIndex2 | (((plane3>>k) & 1) << 2);
        colourIndex2 = colourIndex2 | (((plane4>>k) & 1) << 3);
        colourIndex2 = colourIndex2 | (((plane5>>k) & 1) << 4);
        colourIndex2 = colourIndex2 | (((plane6>>k) & 1) << 5);
        
        //check if we have a ham pixel
        if(colourIndex2 & 0xF0){
            
            int control = colourIndex2 >> 4;
            uint32_t colour = colourIndex2 & 0xF;
            colour = (colour << 4) | colour;
            uint32_t previous = pixBuff[counter+15];
            
            switch(control){
                case 1:
                    previous = (previous & 0xFFFFFF00) | colour;
                    break;
                case 2:
                    colour <<= 16;
                    previous = (previous & 0xFF00FFFF) | colour;
                    break;
                case 3:
                    colour <<= 8;
                    previous = (previous & 0xFFFF00FF) | colour;
                    break;
                    
                    
            }
            
            
            pixBuff[counter+16] = previous;
            counter +=1;
            pixBuff[counter+16] = previous;
            counter +=1;
            
            
        }else{
            uint32_t colour = internal.palette[colourIndex2];
            pixBuff[counter+16] = colour;
            counter +=1;
            
            //Double up pixels since lores only has 320 pixels
            pixBuff[counter+16] = colour;
            counter +=1;
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

