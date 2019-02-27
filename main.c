//
//  main.c
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

#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <SDL2/SDL.h>
#include "CPU.h"


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

//SDL_sem* cpuWait;
//SDL_cond* interruptFlag;



int disass = 0; //set to any value greter than 0 to turn on dissassembler



//run the CPU in a seprate thread... need to sort out interrupts first...
int thd68k(void* data){
    
    while(1){
        cpu_execute();
    }
    
}




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
    
    
    int size = (int)lseek(fd, 0, SEEK_END);
    
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
    
    
    //Floppy disk
    if(argc>2){
        printf("%s\n",argv[2]);
        fd = open(argv[2],O_RDWR);
    }else{
       
        fd = open("/Users/Shared/uae/WORKBENCH/WB-1.3.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/DLXP4.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/DISK1.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/Fright Night.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/JUGGLER.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/amiga_500a_Tutorial_Disk.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/CuAmiga15.adf",O_RDONLY);
        
        if(fd==0){
            printf("no disk in DF0:\n");
        }
        
    }
    
 

    ADF2MFM(fd, floppyInit(0));
    close(fd);
    
    //DF1
    fd = open("/Users/Shared/uae/WORKBENCH/EX-1_3.adf",O_RDONLY);
    ADF2MFM(fd,floppyInit(1));
    close(fd);
    
    
    
    
    
    //setup emulator
    cpu_init();
    
    hostInit();

    ChipsetInit();
    
    //SDL_Thread* clockThread = SDL_CreateThread(thdClock, "Clock", &sec);
    //SDL_DetachThread(clockThread);
    
    //cpuWait = SDL_CreateSemaphore(0);
    //SDL_Thread* cpuThread = SDL_CreateThread(thd68k, "CPU", 0);
    //SDL_DetachThread(cpuThread);
    
    
    
    Chipset_t* chipView = &chipset;
    Internal_t* internalView = &internal;
    
    printf("\nEntering main loop\n");
    int running =1;
    while(running){
        

        
        while (SDL_PollEvent(&host.event)) {
            switch (host.event.type) {
                case SDL_QUIT:
                    running=0;
                    break;
                    
                case SDL_KEYUP:
                    
                    
                    if(host.event.key.keysym.sym==SDLK_F1){
                        //Capture F1
                        break;
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_F2){
                        //Capture F2
                        break;
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_F3){
                        //Capture F2
                        break;
                    }
                    
                    releaseKey(host.event.key.keysym.sym);
                    break;
                    
                case SDL_KEYDOWN:
                    
                    if(host.event.key.keysym.sym==SDLK_ESCAPE){
                        running = 0;
                    }
                    
 
                    
                    if(host.event.key.keysym.sym==SDLK_F1){
                            floppyInsert(0);
                            printf("Disk inserted in df0:\n");
                        break;
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_F2){
                            floppyInsert(1);
                            printf("Disk inserted in df1:\n");
                        break;
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_F3){
                        toggleLEDs();
                        break;
                    }
                    
                    pressKey(host.event.key.keysym.sym);
                    
                    
                    
                    break;
                    
                default:
                    break;
            }
        }
    

        for(int cpuCycles =0;cpuCycles<200;++cpuCycles){
                
               // for(int systemCycles=0;systemCycles<4;++systemCycles){

                    checkInterrupt(&chipset);
                    dma_execute();
                    cpu_execute();

               //}

        }
        
        
    }
    
    
    
    
    SDL_Quit();
    return 0;
}







