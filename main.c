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



void encodeBlock(uint8_t* source, uint8_t* destination,int size){

    for(int i=0;i<size;i++){
        destination[i] = (source[i] >> 1) & 0x55;
        destination[i+size] = (source[i]) & 0x55;
    }

}



uint8_t addClockBits(uint8_t previous, uint8_t value) {
    // Clear all previously set clock bits
    value &= 0x55;
    
    // Compute clock bits (clock bit values are inverted)
    uint8_t lShifted = (value << 1);
    uint8_t rShifted = (value >> 1) | (previous << 7);
    uint8_t cBitsInv = lShifted | rShifted;
    
    // Reverse the computed clock bits
    uint64_t cBits = cBitsInv ^ 0xAA;
    
    // Return original value with the clock bits added
    return value | cBits;
}

void expA2M(int fd, uint8_t* mfm){

    int size =(int) lseek(fd, 0, SEEK_END);
    
    //512 bytes per sector
    int sectors = size/512;
    
    //22 sectors per track (each side 11 sectors)
    int tracks = (sectors / 22);
    
    //each track is 12798 bytes in size, multiplied by 2 because there are 2 sides
    //int mfmSize = tracks * (12798 * 2);
    
    uint8_t adf[size];
   // uint8_t mfm[mfmSize];
    
    uint8_t lowlevelSector[544]; //bytes per low level sector
    
    lseek(fd, 0, SEEK_SET);
    read(fd, adf, size);

    
    int count = 0;
    int s = 0;
    
    for(int track = 0;track<tracks;track++){
        for(int side=0;side<2;side++){
            for(int sector=0;sector<11;sector++){
                
                //int secCountDown = 11 - sector;
                
                
                //printf("%d: Track %d, Side: %d, Sector %d (%d)\n",s,track,side,sector,secCountDown);
                
                //Build the sector
                lowlevelSector[0] = 0x0;
                lowlevelSector[1] = 0x0;
                
                lowlevelSector[2] = 0xA1; // will be a sync mark
                lowlevelSector[3] = 0xA1; // will be a sync mark
                
                //sector info
                
                lowlevelSector[4] = 0xFF;
                lowlevelSector[5] = track << 1 | side;
                lowlevelSector[6] = sector;
                lowlevelSector[7] = 11 - sector;
                
                //Sector label
                lowlevelSector[8]  = 0x0;
                lowlevelSector[9]  = 0x0;
                lowlevelSector[10] = 0x0;
                lowlevelSector[11] = 0x0;
                
                lowlevelSector[12] = 0x0;
                lowlevelSector[13] = 0x0;
                lowlevelSector[14] = 0x0;
                lowlevelSector[15] = 0x0;
                
                lowlevelSector[16] = 0x0;
                lowlevelSector[17] = 0x0;
                lowlevelSector[18] = 0x0;
                lowlevelSector[19] = 0x0;
                
                lowlevelSector[20] = 0x0;
                lowlevelSector[21] = 0x0;
                lowlevelSector[22] = 0x0;
                lowlevelSector[23] = 0x0;
                
                
                //data
                for(int i=0;i<512;++i){
                    lowlevelSector[32+i] = adf[i+(count*512)];
                    
                }
                

                
                //***************
                
                //Encode

                mfm[s+0] = 0xAA;
                mfm[s+1] = 0xAA;
                mfm[s+2] = 0xAA;
                mfm[s+3] = 0xAA;
                
                mfm[s+4] = 0x44;
                mfm[s+5] = 0x89;
                mfm[s+6] = 0x44;
                mfm[s+7] = 0x89;
                
                //info
                encodeBlock(&lowlevelSector[4], &mfm[s+8], 4); // adds 8 bytes
                
                //Disklabel
                encodeBlock(&lowlevelSector[8], &mfm[s+16], 16);//adds 32 bytes
                
                //Data section
                encodeBlock(&lowlevelSector[32], &mfm[s+64], 512);
                
                
                //Header checksum
                uint8_t hcheck[4] = { 0, 0, 0, 0 };
                for(unsigned i = 8; i < 48; i += 4) {
                    hcheck[0] ^= mfm[s+i];
                    hcheck[1] ^= mfm[s+i+1];
                    hcheck[2] ^= mfm[s+i+2];
                    hcheck[3] ^= mfm[s+i+3];
                }
                
                lowlevelSector[24] = hcheck[0];
                lowlevelSector[25] = hcheck[1];
                lowlevelSector[26] = hcheck[2];
                lowlevelSector[27] = hcheck[3];
                
                //header checksum
                encodeBlock(&lowlevelSector[24], &mfm[s+48], 4); //adds 8 bytes
                
                
                // Data checksum
                uint8_t dcheck[4] = { 0, 0, 0, 0 };
                for(unsigned i = 64; i < 1088; i += 4) {
                    dcheck[0] ^= mfm[s+i];
                    dcheck[1] ^= mfm[s+i+1];
                    dcheck[2] ^= mfm[s+i+2];
                    dcheck[3] ^= mfm[s+i+3];
                }
                
                lowlevelSector[28] = dcheck[0];
                lowlevelSector[29] = dcheck[1];
                lowlevelSector[30] = dcheck[2];
                lowlevelSector[31] = dcheck[3];
                
                //Encode Data checksum
                encodeBlock(&lowlevelSector[28], &mfm[s+56], 4); //adds 8 bytes
                
                
                
                //Add clocking bits
                for(int i=8;i<1088;i++){
                    uint8_t previous = mfm[s+i-1];
                    
                    mfm[s+i] = addClockBits(previous,mfm[s+i]);
                    //mfm[s+i] = clocking(previous, mfm[s+i]);
                    
                }
                
                s += 1088;    //Why not 1088, which is the size of the data we've produced
                count +=1;
                
            
            }
            
            //Add clocking bits to the track gap
            mfm[s]   = addClockBits(mfm[s-1],0);
            mfm[s+1] = 0xA8;
            mfm[s+2] = 0x55;
            mfm[s+3] = 0x55;
            mfm[s+4] = 0xAA;
            
            for(int i=5;i<700;i++){
                uint8_t previous = mfm[s+i-1];
                
                mfm[s+i] = addClockBits(previous,0);
                //mfm[s+i] = clocking(previous, mfm[s+i]);
                
            }
            
            s += 830;   //pad track to make 12798 bytes to meet the ADF-EXT spec.
            //printf("\n");
        }

    }
     //printf("loaded");

}



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
        fd = open("/Users/Shared/uae/DosUae/DLXP4.ADF",O_RDONLY);
        //fd = open("/Users/Shared/uae/DosUae/CuAmiga15.adf",O_RDONLY);
        
        if(fd==0){
            printf("no disk in DF0:\n");
        }
        
    }
    
    //size = lseek(fd, 0, SEEK_END);
    //lseek(fd, 2004, SEEK_SET);
    //read(fd, df0.mfmData, size);
    
    //printf("Floppy size: %d\n",size);
 

    
/*
    uint32_t buffer[65536];
    
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
                
                printf("%d: Track %d, Side: %d, Sector: %d (%d)\n",i,track,head,(info >>8) & 255,info & 255);
                
                for(int j=60;j<512+64;++j){
                    
                    if(j>492 && j<523){
                        char ch =((dat[i+j] & 0x55) << 1) | (dat[512+i+j] & 0x55);
                        //printf("%c",ch);
                    }
                    
                }
                //printf("\n");

            }
            
        }
        
        printf("\n");
    }
 */
    
// Experimental ADF to MFM loader

    expA2M(fd,df0.mfmData);
    close(fd);
    
    
    
    
    //setup emulator
    cpu_init();
    

    
    SDL_Init(SDL_INIT_EVERYTHING);
    host.window = SDL_CreateWindow("Omega v0.5",
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
    
    
    
    Chipset_t* chipView = &chipset;
    Internal_t* internalView = &internal;
    
    printf("\nEntering main loop\n");
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
                    
                    releaseKey(host.event.key.keysym.sym);
                    break;
                    
                case SDL_KEYDOWN:
                    
                    if(host.event.key.keysym.sym==SDLK_ESCAPE){
                        running = 0;
                    }
                    
 
                    
                    if(host.event.key.keysym.sym==SDLK_F1){
                        if(df0.data[0] !=0){
                            df0.userInserted = 1;
                            printf("Disk inserted in df0:\n");
                        }
                        break;
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_F2){
                        toggleLEDs();
                        break;
                    }
                    
                    pressKey(host.event.key.keysym.sym);
                    
                    
                    
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







