//
//  main.c
//  The Omega Project
//  https://github.com/h5n1xp/Omega
//
//  Created by Matt Parsons on 02/02/2019.
//  Copyright © 2019 Matt Pasons. All rights reserved.
//  <h5n1xp@gmail.com>
//
//
//  This Source Code Form is subject to the terms of the
//  Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <SDL2/SDL.h>
#include "CPU.h"
#include "image.h"

#include "Host.h"
#include "Memory.h"
#include "DMA.h"
#include "Chipset.h"
#include "CIA.h"
#include "Floppy.h"

#include "m68k.h"

#define PAL 1  // PAL  system
//define PAL 0 // NTSC system

int screenWidth = 640;
int screenHeight = 200;

SDL_sem* cpuWait;
SDL_cond* interruptFlag;



int disass = 0; //set to any value greter than 0 to turn on dissassembler





int main(int argc, const char * argv[]) {
    
    
    //load rom from disk if need be
    int fd=0;
    
    if(argc>1){
        fd = open(argv[1],O_RDWR);
    }else{
        fd = open("/Users/Shared/uae/roms/Kick13.rom",O_RDONLY); //"/Users/Shared/uae/roms/Kick12.rom" "/Users/Shared/uae/roms/DiagROM"  "/Users/Shared/uae/roms/Kick3.rom"
        if(fd<1){
            printf("Usage: zorro <kickstart.rom> <raw adf image>\n");
        return 0;
        }
        
    }
    
    
    int size = lseek(fd, 0, SEEK_END);
    
    //hack to load 256k roms
    if(size==0x40000){
        lseek(fd, 0, SEEK_SET);
        read(fd, &low16Meg[0xF80000], size);
        lseek(fd, 0, SEEK_SET);
        read(fd, &low16Meg[0xFC0000], size);
    }else{
    
        lseek(fd, 0, SEEK_SET);
        read(fd, &low16Meg[0xF80000], size);
    }
    
    close(fd);
    
    
    floppy_init(&df0, 0);
    floppy_init(&df1, 1);
    floppy_init(&df2, 2);
    floppy_init(&df3, 3);
    
    //Floppy disk
    if(argc>2){
        printf("%s\n",argv[2]);
        fd = open(argv[2],O_RDWR);
    }else{
       
        //fd = open("/Users/Shared/uae/WORKBENCH/WB-1.3.adf",O_RDONLY);
        fd = open("/Users/Shared/uae/WORKBENCH/raw2.adf",O_RDONLY);
        //fd = open("/Users/matt/Documents/WHDLoad Collection/rawread/test2.adf",O_RDONLY);
        if(fd==0){
            printf("no disk in DF0:\n");
        }
        
    }
    
    size = lseek(fd, 0, SEEK_END);
    lseek(fd, 2000, SEEK_SET);
    read(fd, df0.mfmData, size);
    
    printf("Floppy size: %d\n",size);
 
    
/*
    uint8_t* dat = df0.mfmData;
    for(int h=0;h<size;h +=12798){
        

        for(int i=h;i<h+12798;++i){
             int test = dat[i] <<24 | dat[i+1] <<16 | dat[i+2] << 8 | dat[i+3];
            
            
            
            if(test==0x44894489){
                
                int odd  = dat[i+4] <<24 | dat[i+5] <<16 | dat[i+6] << 8 | dat[i+7];
                int even  = dat[i+8] <<24 | dat[i+9] <<16 | dat[i+10] << 8 | dat[i+11];
                int info = ((odd & 0x55555555) << 1) | (even & 0x55555555);
                
                int track = (info >> 17) & 127;
                int head  = (info >> 16) & 1;
                
                printf("Track %d, Side: %d, Sector: %d (%d)\n",track,head,(info >>8) & 255,info & 255);
                
                for(int j=60;j<512+64;++j){
                    
                    if(j>492 && j<523){
                        char ch =((dat[i+j] & 0x55) << 1) | (dat[512+i+j] & 0x55);
                        //printf("%c",ch);
                    }
                    
                }

            }
            
        }
        
        printf("\n");
    }
 */
    
    close(fd);

    
    //setup emulator
    cpu_init();
    

    
    SDL_Init(SDL_INIT_EVERYTHING);
    host.window = SDL_CreateWindow("Omega v0.1",
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
    
    
    
    ChipsetInit();
    
    int running =1;
    
    int sec;
    
    //SDL_Thread* clockThread = SDL_CreateThread(thdClock, "Clock", &sec);
    //SDL_DetachThread(clockThread);
    
    //cpuWait = SDL_CreateSemaphore(0);
    //SDL_Thread* cpuThread = SDL_CreateThread(thd68k, "CPU", 0);
    //SDL_DetachThread(cpuThread);
    
    
    
    
    
    printf("\nEntering main loop\n");
    while(running){
        

        
        while (SDL_PollEvent(&host.event)) {
            switch (host.event.type) {
                case SDL_QUIT:
                    running=0;
                    break;
                    
                case SDL_KEYDOWN:
                    
                    if(host.event.key.keysym.sym==SDLK_ESCAPE){
                        running = 0;
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_z){
                        printf("Disk Now Valid\n");
                        df0.indexTop=6333;
                        
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_q){
                            printf("**! Reset !**\n");
                            cpu_pulse_reset();
                            chipset.dmaconr = 0;
                            internal.copperPC = 0;
                        
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_a){
                        if(df0.data[0] !=0){
                            
                            df0.userInserted = 1 - df0.userInserted; //toggle ejecting
                            if(df0.userInserted==1){
                                printf("**!Disk inserted in df0:!**\n");
                            }else{
                                printf("**!Disk removed from df0:!**\n");
                                df0.diskchange=0;
                            }
                        }
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_F1){
                        if(df0.data[0] !=0){
                            df0.userInserted = 1;
                            printf("Disk inserted in df0:\n");
                        }
                        
                    }
                    break;
                    
                default:
                    break;
            }
        }
    

        for(int cpuCycles =0;cpuCycles<20;++cpuCycles){
                
                for(int systemCycles=0;systemCycles<4;++systemCycles){

                    checkInterrupt(&chipset);
                    dma_execute();
                    cpu_execute();
                }
            

            //floppy_execute(&df0);
            floppyDisk_execute(&df0,&chipset, &CIAA, &CIAB);
            //floppyDisk_execute(&df1,&chipset, &CIAA, &CIAB);
            //floppyDisk_execute(&df2,&chipset, &CIAA, &CIAB);
            //floppyDisk_execute(&df3,&chipset, &CIAA, &CIAB);

        }
        
        
    }
    
    
    
    
    SDL_Quit();
    return 0;
}







