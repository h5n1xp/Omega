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
    printf("Do nossing!!");
}

void woopdiePoop(EventQueue_t* queue, void* data){
   // printf("this took a cycle");
    CreateEvent(queue, woopdiePoop, data, 2);
}








void ExecuteChipsetCycles(EventQueue_t* queue, int count){
    for(int i = 0; i < count; ++i){
        AdvanceCycle(queue);
    }
}

uint32_t* pixelBuffer;

void RunOmega(SDL_Renderer* renderer,SDL_Texture* framebuffer,Omega_t* omega, uint64_t* cycleCount,SDL_Event* event,int* running){
   
/*
    for(int l=0;l<1;++l){
        //CheckInterrupts();
        *cycleCount += m68k_execute(1);
        //printf("Ol");
    }
     
     DMAExecute(omega->Chipstate, pixelBuffer);
     FloppyExecute();
 */
   
    uint32_t cc = m68k_execute(24);
    FloppyExecute();
    
    /*
    for(int i=0;i<cc/10;++i){
        RunCIACycle();
    }
    */

    for(int i=0; i<cc/2; ++i){
        CheckInterrupts();
        DMAExecute(omega->Chipstate, pixelBuffer);

    
    
    if( ((Chipset_t*)omega->Chipstate)->VBL == 1){
        uint32_t* palette = ((Chipset_t*)omega->Chipstate)->Colour;
        SDL_SetRenderDrawColor(renderer, palette[0] >> 16, (palette[0] >> 8) & 255, palette[0] & 255, 255);
        SDL_RenderClear(renderer);
        
        if( ((Chipset_t*)omega->Chipstate)->needsRedraw){
            SDL_UnlockTexture(framebuffer);
            SDL_RenderCopy(renderer, framebuffer, NULL, NULL);
            SDL_LockTexture(framebuffer, NULL,(void**)&((Chipset_t*)omega->Chipstate)->frameBuffer, &((Chipset_t*)omega->Chipstate)->frameBufferPitch);
            ((Chipset_t*)omega->Chipstate)->needsRedraw = 0;
        }
        
        SDL_RenderPresent(renderer);
        ((Chipset_t*)omega->Chipstate)->VBL = 0;
        //SDL_Delay(16);
        
        
        while (SDL_PollEvent(event)) {
            
            switch (event->type) {
                case SDL_QUIT:
                    *running = 0;
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
                    
                    releaseKey(event->key.keysym.sym);
                    //printf("Key Up: %c",event.key.keysym.sym);
                    break;
                    
                case SDL_KEYDOWN:
                    
                    /*
                    if(host.event.key.keysym.sym==167){
                        running = 0;
                    }
                    */
                     
                    if(event->key.keysym.sym==SDLK_F1){
                            //floppyInsert(0);
                        *cycleCount = 1;
                        break;
                    }
                    
                    if(event->key.keysym.sym==SDLK_F2){
                            //floppyInsert(1);
                        *cycleCount = 4096;
                        if(*cycleCount < 100){*cycleCount = 1;};
                        break;
                    }
                    
                    if(event->key.keysym.sym==SDLK_F3){
                        //floppyInsert(2);
                        
                        int fd = open("/Users/Shared/Dump.txt",O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
                        write(fd,omega->chipRAM, 524288);
                        close(fd);
                        
                        break;
                    }
                    /*
                    if(host.event.key.keysym.sym==SDLK_F4){
                        floppyInsert(3);
                        break;
                    }
                    
                    if(host.event.key.keysym.sym==SDLK_F5){
                        toggleLEDs();
                        break;
                    }
                    */
                    
                    pressKey(event->key.keysym.sym);
                    //printf("%c",event->key.keysym.sym);
                    break;
                
                default:
                    // printf("%d",event.key.keysym.sym);
                break;
                   
            }

        }
        
        int button = SDL_GetRelativeMouseState(&((Chipset_t*)omega->Chipstate)->mouseXrel, &((Chipset_t*)omega->Chipstate)->mouseYrel);
        
        
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
    

}























char temp[256];


int main(int argc, const char * argv[]) {
    printf("Welcome to my world!\n");
    
    if(16777216 - sizeof(Omega_t) != 0){
        printf("Error!!! Memory allocation Wrong!");
    }
    
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Omega2 v0.01",
                                   0,//window X position
                                   0,//window Y position
                                   800, 600,
                                   SDL_WINDOW_RESIZABLE);// | SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    
    //SDL_ShowCursor(SDL_DISABLE);
    SDL_Texture* framebuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 800, 312);
    
    
    //Debugging Window
    SDL_Window* debugWin = SDL_CreateWindow("Omega2 - ChipRAM Map",
                                              0,//window X position
                                              700,//window Y position
                                              800, 660,
                                              SDL_WINDOW_RESIZABLE);// | SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Renderer* debugRen = SDL_CreateRenderer(debugWin, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_TextInit(debugRen);
    SDL_Texture* debugbuffer = SDL_CreateTexture(debugRen, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 800, 660);

    uint32_t* pixelsT; // Destination RGBA Buffer
    int32_t pitchT = 800 * 4; //bytes per RGBA buffer line
    SDL_LockTexture(debugbuffer, NULL, (void**)&pixelsT, &pitchT);
    for(int i=0; i <524288; ++i){
        pixelsT[i] = 0xFFFF0000;    //Set DEbug Screen to red
    }
    SDL_UnlockTexture(debugbuffer);
    SDL_RenderCopy(debugRen, debugbuffer, NULL, NULL);
    SDL_RenderPresent(debugRen);
    

    
    //create a new queue
    EventQueue_t* queue = CreateQueue();

    
    //Create first real event
    CreateEvent(queue, NoOperation, NULL, 0);
    CreateEvent(queue, woopdiePoop, NULL, 1);

    
    //Init Omega
    Omega_t* omega = InitRAM(0);
    ((Chipset_t*)omega->Chipstate)->frameBufferPitch = 800 * 4;
    SDL_LockTexture(framebuffer, NULL, (void**)&((Chipset_t*)omega->Chipstate)->frameBuffer, &((Chipset_t*)omega->Chipstate)->frameBufferPitch);
    

    
    
    //Load ROM From Disk
    int fd = -1;
    //fd = open("/Users/Shared/Kick13.rom",O_RDONLY);
    //fd = open("/Users/Shared/Kick3.rom",O_RDONLY);
    //fd = open("/Users/Shared/DiagROM1",O_RDONLY);
    //fd = open("/Users/Shared/Kick2.rom",O_RDONLY);
    //fd = open("/Users/Shared/DiagROM2",O_RDONLY);
    if(fd > 0){
        int size = (int)lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
        read(fd, omega->KickstartROM, size);
        printf("BootROM Loaded OK!\n");
    }
    
    
    FloppyInit();
    m68k_init();
    m68k_set_cpu_type(M68K_CPU_TYPE_68000); //Pretend to be an A500 for now...
    m68k_pulse_reset();
    
    
    
    
    SDL_Event event;
    int running = 1;
    
    uint64_t cycleCount = 4096;
    

    
    //StartExecution(queue);
    while (running) {

    
 
        int debugRefreshRate = cycleCount; // 4096 * 16
        for(int i = 0;i < debugRefreshRate; ++i){
            RunOmega(renderer,framebuffer,omega,&cycleCount,&event,&running);
        }
        
    
        
        //Debug
        uint32_t* pixels; // Destination RGBA Buffer
        int32_t pitch = 800 * 4; //bytes per RGBA buffer line
        uint32_t* buffer = (uint32_t*)omega->chipRAM;
        SDL_LockTexture(debugbuffer, NULL, (void**)&pixels, &pitch);
        for(int i=0; i <524288; ++i){
            pixels[i] = buffer[i] | 0xFF000000;
        }
        SDL_UnlockTexture(debugbuffer);
        SDL_RenderCopy(debugRen, debugbuffer, NULL, NULL);
        
        uint16_t* Temp16 = 0;
        int ty = 2;
        uint32_t* p = (uint32_t*)&omega->chipRAM[0xDFF0E0];
        sprintf(temp, "BPL1PT - %d",*p);
        SDL_Print(temp,1,ty);
        
        ty += 16;
        p = (uint32_t*)&omega->chipRAM[0xDFF0E4];
        sprintf(temp, "BPL2PT - %d",*p);
        SDL_Print(temp,1,ty);
        
        ty += 16;
        p = (uint32_t*)&omega->chipRAM[0xDFF0E8];
        sprintf(temp, "BPL3PT - %d",*p);
        SDL_Print(temp,1,ty);
        
        ty += 16;
        p = (uint32_t*)&omega->chipRAM[0xDFF0EC];
        sprintf(temp, "BPL4PT - %d",*p);
        SDL_Print(temp,1,ty);
        
        ty += 16;
        p = (uint32_t*)&omega->chipRAM[0xDFF0F0];
        sprintf(temp, "BPL5PT - %d",*p);
        SDL_Print(temp,1,ty);
        
        ty += 16;
        Temp16 = (uint16_t*)&omega->chipRAM[0xDFF092];
        sprintf(temp, "DFFSTRT - %d",*Temp16);
        SDL_Print(temp,1,ty);
        
        ty += 16;
        Temp16 = (uint16_t*)&omega->chipRAM[0xDFF094];
        sprintf(temp, "DFFSTOP - %d",*Temp16);
        SDL_Print(temp,1,ty);
        
        ty += 16;
        Temp16 = (uint16_t*)&omega->chipRAM[0xDFF100];
        sprintf(temp, "BPLCON0 - 0x%x",*Temp16);
        SDL_Print(temp,1,ty);
        
        ty += 16;
        Temp16 = (uint16_t*)&omega->chipRAM[0xDFF108];
        sprintf(temp, "BPLMOD - %d",*Temp16);
        SDL_Print(temp,1,ty);
        
        ty += 16;
        Temp16 = (uint16_t*)&omega->chipRAM[0xDFF08E];
        sprintf(temp, "DIWSTRT - 0x%x",*Temp16);
        SDL_Print(temp,1,ty);
        
        ty += 16;
        Temp16 = (uint16_t*)&omega->chipRAM[0xDFF090];
        sprintf(temp, "DIWSTOP - 0x%x",*Temp16);
        SDL_Print(temp,1,ty);
        
        ty += 16;
        Temp16 = (uint16_t*)&omega->chipRAM[0xDFF080];
        sprintf(temp, "COPLOC1 - 0x%x",*Temp16);
        SDL_Print(temp,1,ty);
        
        ty += 16;
        Temp16 = (uint16_t*)&omega->chipRAM[0xDFF084];
        sprintf(temp, "COPLOC2 - 0x%x",*Temp16);
        SDL_Print(temp,1,ty);
        
        ty += 16;
        if( omega->chipRAM[0xDFF002] & 64){
            SDL_Print("Blitter Busy",1,ty);
        }else{
            SDL_Print("Blitter Idle",1,ty);
        }
        
        ty += 16;
        if( omega->chipRAM[0xBFE001] & 2){
            SDL_Print("LED Off",1,ty);
        }else{
            SDL_Print("LED On",1,ty);
        }

        ty += 32;
        *Temp16 = CIAState->ATA;
        sprintf(temp, "CIA A: TA - %d",*Temp16);
        SDL_Print(temp,1,ty);
        ty += 16;
        *Temp16 = CIAState->ATB;
        sprintf(temp, "CIA A: TB - %d",*Temp16);
        SDL_Print(temp,1,ty);
        ty += 16;
        *Temp16 = omega->chipRAM[0xBFEE01];
        sprintf(temp, "CIA A: CRA - %d",*Temp16);
        SDL_Print(temp,1,ty);
        ty += 16;
        *Temp16 = omega->chipRAM[0xBFEF01];
        sprintf(temp, "CIA A: CRB - %d",*Temp16);
        SDL_Print(temp,1,ty);
        
        ty += 32;
        *Temp16 = CIAState->ATA;
        sprintf(temp, "CIA B: TA - %d",*Temp16);
        SDL_Print(temp,1,ty);
        ty += 16;
        *Temp16 = CIAState->BTB;
        sprintf(temp, "CIA B: TB - %d",*Temp16);
        SDL_Print(temp,1,ty);

        
        ty += 32;
        uint32_t time = ((Chipset_t*)omega->Chipstate)->DMACycles / ((Chipset_t*)omega->Chipstate)->DMAFreq;
        sprintf(temp, "Emulator Time: %d:%d",time/60,time%60);
        SDL_Print(temp,1,ty);
  
        ty += 16;
        sprintf(temp, "Debug Cycle: %d",cycleCount);
        SDL_Print(temp,1,ty);
        SDL_RenderPresent(debugRen);

        
    }
    
    
    
    
    
    /*
    float time = (SDL_GetPerformanceCounter() - queue->startTime);
    float freq = SDL_GetPerformanceFrequency();
    float microTime = time * 1000000.0;
    printf("%f micro seconds\n",microTime / freq);
    printf("%f micro seconds per cycle (%f MHz)",(microTime / freq)/queue->currentCycle,(1/((time / freq)/queue->currentCycle))/1000000);
    */

    return 0;
}
