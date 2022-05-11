//
//  main.c
//  Omega2
//
//  Created by Matt Parsons on 25/03/2022.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "SDL2/SDL.h"

#include "EventQueue.h"
#include "m68k.h"
#include "Chipset.h"
#include "CIA.h"
#include "DMA.h"
#include "Floppy.h"

#include "Omega.h"

//File handling
#include <fcntl.h>
#include <unistd.h>

#include "SDLText.h"




void NoOperation(EventQueue_t* queue, void* data){
    //Burn a cycle and return
    printf("Do nothing!!");
}

void doSomething(EventQueue_t* queue, void* data){
   // printf("this took a cycle");
    CreateEvent(queue, doSomething, data, 2);
}

void ExecuteChipsetCycles(EventQueue_t* queue, int count){
    for(int i = 0; i < count; ++i){
        AdvanceCycle(queue);
    }
}


// Assumes little endian
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
}

// Assumes little endian
void sprintBits(char* buff, size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            sprintf(buff,"%u", byte);
        }
    }
}

uint32_t* pixelBuffer;


SDL_sem *sem;

int CPUThread(void* data){
    
    while(1){
        m68k_execute(4096);
        SDL_SemWait(sem);
    }
    
}


int main(int argc, const char * argv[]) {
    printf("Omega V2\n");
    
    if(16777216 - sizeof(Omega_t) != 0){
        printf("Error!!! Memory allocation Wrong!");
    }
    

    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Omega2 v0.01",
                                   0,//window X position
                                   0,//window Y position
                                   800, 624,
                                   SDL_WINDOW_RESIZABLE);// | SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    
    SDL_ShowCursor(SDL_DISABLE);
    SDL_Texture* framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 800, 312); //350 should be 312
    SDL_TextInit(renderer);



    
    //create a new queue
    EventQueue_t* queue = CreateQueue();

    //Create first real event
    CreateEvent(queue, NoOperation, NULL, 0);
    CreateEvent(queue, doSomething, NULL, 1);

    
    
    //Init Omega
    Omega_t* omega = InitRAM(0);
    Chipset_t* chipstate = (Chipset_t*)omega->Chipstate;
    chipstate->frameBufferPitch = 800 * 4;
    SDL_LockTexture(framebuffer, NULL, (void**)&chipstate->frameBuffer, &chipstate->frameBufferPitch);
    
    
    // Datatype size checks for debugging
    printf("Internal Chipset Data structure size: %d bytes\n",sizeof(Chipset_t));
    printf("Internal CIA Data structure size: %d bytes\n",sizeof(CIA_t));
    
    
    //Load ROM From Disk
    int fd = -1;
    //fd = open("/Users/Shared/Bootstrap.rom",O_RDONLY);
    //fd = open("/Users/Shared/Kick07.rom",O_RDONLY);
    //fd = open("/Users/Shared/Kick10.rom",O_RDONLY);
    //fd = open("/Users/Shared/Kick13.rom",O_RDONLY);
    //fd = open("/Users/Shared/Kick2.rom",O_RDONLY);
    fd = open("/Users/Shared/Kick3.rom",O_RDONLY);
    //fd = open("/Users/Shared/Kick31.rom",O_RDONLY);
    //fd = open("/Users/Shared/DiagROM1",O_RDONLY);
    //fd = open("/Users/Shared/DiagROM2",O_RDONLY);
    if(fd > 0){
        int size = (int)lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
        read(fd, omega->KickstartROM, size);
        
        if(size < 524288){
            lseek(fd, 0, SEEK_SET);
            read(fd, omega->KickstartROM+262144, size);
        }
        
        printf("BootROM Loaded OK!\n");
        close(fd);
    }

    
    fd = -1;
    uint8_t DF0[1024*1024];
    
    //fd = open("/Users/Shared/WB-1.3.adf",O_RDONLY);
    //fd = open("/Users/Shared/DISK1.ADF",O_RDONLY);
    //fd = open("/Users/Shared/LEMM1.ADF",O_RDONLY);
    //fd = open("/Users/Shared/MLM1.ADF",O_RDONLY);
    //fd = open("/Users/Shared/MP.ADF",O_RDONLY);
    //fd = open("/Users/Shared/DLXP4.ADF",O_RDONLY);
    //fd = open("/Users/Shared/DLXP3.ADF",O_RDONLY);
    //fd = open("/Users/Shared/WB31.ADF",O_RDONLY);
    
    fd = open("/Users/Shared/Blitz2-1.adf",O_RDONLY);
    
    if(fd > 0){
        int size = (int)lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
        read(fd, DF0, size);
        printf("ADF Loaded OK: %d bytes\n",size);
        close(fd);
    }
    
    
    uint8_t DF1[1024*1024];
    
    //fd = open("/Users/Shared/EX13.ADF",O_RDONLY);
    //fd = open("/Users/Shared/DISK1.ADF",O_RDONLY);
    //fd = open("/Users/Shared/DLXP4.ADF",O_RDONLY);
    //fd = open("/Users/Shared/bpl0dat1.adf",O_RDONLY);
    //fd = open("/Users/Shared/WB31.ADF",O_RDONLY);
    
    fd = open("/Users/Shared/Blitz2-2.adf",O_RDONLY);
    
    if(fd > 0){
        int size = (int)lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
        read(fd, DF1, size);
        printf("ADF Loaded OK: %d bytes\n",size);
        close(fd);
    }
    
    
    uint8_t DF2[1024*1024];
    //fd = open("/Users/Shared/DLXP4.ADF",O_RDONLY);
    
    fd = open("/Users/Shared/Blitz2-3.adf",O_RDONLY);
    
    if(fd > 0){
        int size = (int)lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
        read(fd, DF2, size);
        printf("ADF Loaded OK: %d bytes\n",size);
        close(fd);
    }
    
    uint8_t DF3[1024*1024];

    //fd = open("/Users/Shared/Blitz2-4.adf",O_RDONLY);
    fd = open("/Users/Shared/DISK1.ADF",O_RDONLY);
    //fd = open("/Users/Shared/DLXP4.ADF",O_RDONLY);
    
    if(fd > 0){
        int size = (int)lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
        read(fd, DF3, size);
        printf("ADF Loaded OK: %d bytes\n",size);
        close(fd);
    }
    
    FloppyInit();
    FloppyInsert(0, DF0);
    FloppyInsert(1, DF1);
    FloppyInsert(2, DF2);
    FloppyInsert(3, DF3);
    
    m68k_init();//M68K_CPU_TYPE_68EC020
    m68k_set_cpu_type(M68K_CPU_TYPE_68000); //Pretend to be an A500 for now...
    m68k_pulse_reset();
    
    
    SDL_Event event;
    int running = 1;
    
    //SDL_CreateThread(CPUThread, "CPU", NULL);


    
    //StartExecution(queue);
    while (running) {

        m68k_execute(128); //128 seems "Normal"
        //SDL_SemPost(sem);
        DMAExecute(omega->Chipstate, pixelBuffer);
    
        //The Emulator has signaled a new frame is reaady.
        if( ((Chipset_t*)omega->Chipstate)->VBL == 1){
            uint32_t* palette = ((Chipset_t*)omega->Chipstate)->Colour;
            SDL_SetRenderDrawColor(renderer, palette[0] >> 16, (palette[0] >> 8) & 255, palette[0] & 255, 255);
            SDL_RenderClear(renderer);
        
            if( ((Chipset_t*)omega->Chipstate)->needsRedraw){
                
                int x;
                int y;
                SDL_GetWindowSize(window, &x, &y);
                
                SDL_Rect framebufferRect = {0,0,x,y};
                SDL_UnlockTexture(framebuffer);
                SDL_RenderCopy(renderer, framebuffer, NULL, &framebufferRect);
                
                SDL_LockTexture(framebuffer, NULL,(void**)&((Chipset_t*)omega->Chipstate)->frameBuffer,&((Chipset_t*)omega->Chipstate)->frameBufferPitch);
                ((Chipset_t*)omega->Chipstate)->frameBuffer -= ((800 * 20) + 180); // Offset adjust (800 * 20) + 180 // 20 lines up, 180 pixels left
                
                ((Chipset_t*)omega->Chipstate)->needsRedraw = 0;
                
                
                char temp[512];
                int ty = 10;
                 /*
                uint16_t BPLCON1 = RAM24bit[0xDFF102];

                sprintf(temp, "BPLCON1: 0x%x",BPLCON1);
                SDL_Print(temp, 10, ty);

                sprintf(temp, "HSTART : %d",((Chipset_t*)omega->Chipstate)->HSTART);
                SDL_Print(temp, 10, ty+16);
                sprintf(temp, "HSTOP  : %d",((Chipset_t*)omega->Chipstate)->HSTOP);
                SDL_Print(temp, 10, ty+16+16);
                //sprintf(temp, "VSTOP  : %d",((Chipset_t*)omega->Chipstate)->VSTOP >> 8);
                //SDL_Print(temp, 10, ty+16+16+16);
                
                sprintf(temp, "DFFSTRT: %d",*(uint16_t*)&((Chipset_t*)omega->Chipstate)->chipram[0xDFF092]);
                SDL_Print(temp, 10, ty+16+16+16+16);
                sprintf(temp, "DFFSTOP: %d",*(uint16_t*)&((Chipset_t*)omega->Chipstate)->chipram[0xDFF094]);
                SDL_Print(temp, 10, ty+16+16+16+16+16);
                */
                uint32_t VBR = m68k_get_reg(NULL, M68K_REG_VBR);
                sprintf(temp, "VBR: %d",VBR);
                SDL_Print(temp, 10, ty+16+16+16+16+16+16);
                
                SDL_RenderPresent(renderer);
            }
        
   
            ((Chipset_t*)omega->Chipstate)->VBL = 0;
       
        
        
        while (SDL_PollEvent(&event)) {
            
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                    
                case SDL_KEYUP:
                    
                    /*
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
                    */
                    
                    releaseKey(event.key.keysym.sym);
                    //printf("Key Up: %c",event.key.keysym.sym);
                    break;
                    
                case SDL_KEYDOWN:
                    
                   
                    if(event.key.keysym.sym==167){
                        running = 0;
                    }
                    
                     
                    if(event.key.keysym.sym==SDLK_F1){
                            //floppyInsert(0);
                        //*cycleCount = 1;
                        FloppyInsert(0, DF0);
                       
                        
                        break;
                    }
                    
                    if(event.key.keysym.sym==SDLK_F2){
                        printf("COPLIST1:\n");
                        DecodeCopperList(1);
                        break;
                    }
                    
                    if(event.key.keysym.sym==SDLK_F3){
                        //floppyInsert(2);
                        printf("COPLIST2:\n");
                        DecodeCopperList(2);
                        break;
                        
                        int fd = open("/Users/Shared/Dump.txt",O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
                        write(fd,omega->chipRAM, 524288);
                        close(fd);
                        
                        break;
                    }
                    
                  
                     /*
                    if(host.event.key.keysym.sym==SDLK_F4){
                        FloppyInsert(0, Workbench);
                        break;
                    }
                   
                    if(host.event.key.keysym.sym==SDLK_F5){
                        toggleLEDs();
                        break;
                    }
                    */
                    
                    pressKey(event.key.keysym.sym);
                    //printf("0x%x",event->key.keysym.sym);
                    break;
                
                default:
                    // printf("%d",event.key.keysym.sym);
                break;
                   
            }

        }

        int button = SDL_GetRelativeMouseState(&((Chipset_t*)omega->Chipstate)->mouseXrel, &((Chipset_t*)omega->Chipstate)->mouseYrel);
            
            uint16_t* JOY0DAT = (uint16_t*)&omega->chipRAM[0xDFF00A];
            
            uint16_t my = *JOY0DAT & 255;
            uint16_t mx = *JOY0DAT >> 8;
            
            mx +=((Chipset_t*)omega->Chipstate)->mouseXrel;
            my +=((Chipset_t*)omega->Chipstate)->mouseYrel;
            
            *JOY0DAT = (mx<<8) | (my & 255);
            
   
            
        switch(button){
            case 1:
                omega->chipRAM[0xBFE001] = omega->chipRAM[0xBFE001] & 0xBF; // left mouse button down;
                break;
            case 4:
               // omega->chipRAM[0xBFE001] = omega->chipRAM[0xBFE001] & 0x7F; // Right mouse button down;
                omega->chipRAM[0xDFF016] = omega->chipRAM[0xDFF016] & 0xFB; // Right mouse button down;
                break;
            case 5:
                omega->chipRAM[0xBFE001] = omega->chipRAM[0xBFE001] & 0xBF; // Left mouse button down;
                omega->chipRAM[0xDFF016] = omega->chipRAM[0xDFF016] & 0xFB; // Right mouse button down;
                break;
            default:
                omega->chipRAM[0xBFE001] = omega->chipRAM[0xBFE001] | 0xC0; // left mouse button up;
                omega->chipRAM[0xDFF016] = omega->chipRAM[0xDFF016] | 0x04; // Right mouse button down;
                break;
        }
    }
        
        
        
    }
    return 0;
}
