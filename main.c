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


int disass = 0; //set to any value greter than 0 to turn on dissassembler



//run the CPU in a seprate thread... need to sort out interrupts first...
int thd68k(void* data){
    
    SDL_Delay(500);
    printf("CPU Running\n");
    
    while(1){
        cpu_execute();
        //SDL_Delay(1);
    }
    
}




int main(int argc, char * argv[]) {
    
    
    //load rom from disk if need be
    int fd=0;
    
    if(argc>1){
        fd = open(argv[1],O_RDWR);
    }else{
        fd = open("/Users/Shared/uae/roms/Kick2.rom",O_RDONLY); //"/Users/Shared/uae/roms/Kick12.rom" "/Users/Shared/uae/roms/DiagROM"  "/Users/Shared/uae/roms/Kick3.rom"
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
    fd=0;
    

    if(low16Meg[0xF80000]==0x11){
        printf("ROM loaded\n");
    }else{
        printf("Not an Amiga ROM\n");
        return 0;
    }
    
    //Floppy disk
    if(argc>2){
        fd = open(argv[2],O_RDWR);
        printf("%s\n",argv[2]);
    }else{
        
        //fd = open("/Users/Shared/uae/WORKBENCH/WB-1.3.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/WORKBENCH/WB-2_05.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/Blitz2-1.ADF",O_RDONLY);
        fd = open("/Users/Shared/uae/DosUae/Blitz2-1-2.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/DLXP4.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/DISK1.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/Fright Night.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/JUGGLER.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/amiga_500a_Tutorial_Disk.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/CuAmiga15.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/LEMM1.ADF",O_RDONLY);
        //fd =open("/Users/Shared/uae/ADF/Defender of the Crown (1986)(Mindscape)(Disk 1 of 2).adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/ADF/AmigaTestKit.adf",O_RDONLY);
    }
    
    //fd = open("/Users/Shared/uae/WORKBENCH/raw3.adf",O_RDONLY);
    
 
    if(fd<1){
        printf("No disk in DF0:\n");
    }else{
        
        //int size =(int) lseek(fd, 0, SEEK_END);
        //lseek(fd, 2004, SEEK_SET);
        //read(fd, floppyInit(0), size);
        
        ADF2MFM(fd, floppyInit(0));
        close(fd);
        printf("DF0: Loaded (press F1 to insert disk)\n");
    }
    fd=0;
    
    //DF1
    //Floppy disk
    if(argc>3){
        printf("%s\n",argv[3]);
        fd = open(argv[3],O_RDWR);
    }else{
        
        //fd = open("/Users/Shared/uae/WORKBENCH/WB-1.3.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/WORKBENCH/EX-2_05.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/DLXP4.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/DISK1.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/Fright Night.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/JUGGLER.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/amiga_500a_Tutorial_Disk.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/af025_1991_08_d025.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/CuAmiga15.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/WORKBENCH/EX-1_3.adf",O_RDONLY);
        fd = open("/Users/Shared/uae/DosUae/Blitz2-2.ADF",O_RDONLY);
        //fd =open("/Users/Shared/uae/ADF/Defender of the Crown (1986)(Mindscape)(Disk 2 of 2).adf",O_RDONLY);
    }
    
    
    if(fd<1){
        printf("No disk in DF1:\n");
    }else{
        ADF2MFM(fd,floppyInit(1));
        close(fd);
        printf("DF1: Loaded (press F2 to insert disk)\n");
    }
    fd=0;
    
    
    //DF2
    //Floppy disk
    if(argc>4){
        printf("%s\n",argv[4]);
        fd = open(argv[4],O_RDWR);
    }else{
        
        //fd = open("/Users/Shared/uae/WORKBENCH/WB-1.3.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/DLXP4.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/DISK1.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/Fright Night.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/JUGGLER.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/amiga_500a_Tutorial_Disk.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/af025_1991_08_d025.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/CuAmiga15.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/WORKBENCH/EX-1_3.adf",O_RDONLY);
        fd = open("/Users/Shared/uae/DosUae/Blitz2-3.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/ADF/Pro Tracker v2.3 (1993)(Noxious).adf",O_RDONLY);
        
    }
    
    
    if(fd<1){
        printf("No disk in DF2:\n");
    }else{
        ADF2MFM(fd,floppyInit(2));
        close(fd);
        printf("DF2: Loaded (press F3 to insert disk)\n");
    }
    fd=0;
    
    //DF2
    //Floppy disk
    if(argc>5){
        printf("%s\n",argv[5]);
        fd = open(argv[5],O_RDWR);
    }else{
        
        //fd = open("/Users/Shared/uae/WORKBENCH/WB-1.3.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/DLXP4.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/DISK1.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/Fright Night.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/JUGGLER.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/amiga_500a_Tutorial_Disk.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/af025_1991_08_d025.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/CuAmiga15.adf",O_RDONLY);
        //fd = open("/Users/Shared/uae/WORKBENCH/EX-1_3.adf",O_RDONLY);
        fd = open("/Users/Shared/uae/DosUae/Blitz2-4.ADF",O_RDONLY);
    }
    
    
    if(fd<1){
        printf("No disk in DF3:\n");
    }else{
        ADF2MFM(fd,floppyInit(3));
        close(fd);
        printf("DF3: Loaded (press F4 to insert disk)\n");
    }
    fd=0;
    
    //setup emulator
    hostInit();
    cpu_init();
    ChipsetInit();
    
    //SDL_Thread* clockThread = SDL_CreateThread(thdClock, "Clock", &sec);
    //SDL_DetachThread(clockThread);
    
    //cpuWait = SDL_CreateSemaphore(0);
    //SDL_Thread* cpuThread = SDL_CreateThread(thd68k, "CPU", 0);
    //SDL_DetachThread(cpuThread);
    
    
    
    Chipset_t* chipView = &chipset;
    Internal_t* internalView = &internal;
    
    
    printf("\nEntering main loop\n");
    int running = 1;
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
                        //Capture F3
                        break;
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_F4){
                        //Capture F4
                        break;
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_F5){
                        //Capture F5
                        break;
                    }
                    
                    
                    releaseKey(host.event.key.keysym.sym);
                    break;
                    
                case SDL_KEYDOWN:
                    
                    if(host.event.key.keysym.sym==167){
                        running = 0;
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_F1){
                            floppyInsert(0);
                        break;
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_F2){
                            floppyInsert(1);
                        break;
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_F3){
                        floppyInsert(2);
                        break;
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_F4){
                        floppyInsert(3);
                        break;
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_F5){
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
            

            dma_execute();
            cpu_execute();

        }
        
        
    }
    
    
    
    
    SDL_Quit();
    return 0;
}







