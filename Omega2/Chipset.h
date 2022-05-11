//
//  Chipset.h
//  Omega2
//
//  Created by Matt Parsons on 29/03/2022.
//
//
//


#ifndef Chipset_h
#define Chipset_h

#include <stdio.h>
#include <stdint.h>

typedef struct{
    int VPOS;
    uint16_t* data; // incrementing pointer sprite data.
    int stop;   // stop VPOS value, when reached the sprite stops being drawn to screen
} SpriteInternal_t;

typedef struct{
    
    void (*WriteWord[513])(uint16_t value);

    // !!!!!!!WARNING!!!!
    // All L and H registers are swapped in memory
    //
    // The in memory registers are stored in little endian format for speed, but some need to be stored in big endian.
    // below are little endian copies of the Registers as the in memory versions need to be big endian for direct reading by the 68K
    uint16_t  BLTDDAT;
    uint16_t  DMACONR;
    uint32_t  VHPOS;
    uint16_t  DSKDATR;
    uint16_t  JOY0DAT;
    uint16_t  JOY1DAT;
    uint16_t  CLXDAT ;
    uint16_t  ADKCONR;
    uint16_t  POT0DAT;
    uint16_t  POT1DAT;
    uint16_t  POTGOR ;
    uint16_t  SERDATR;
    uint16_t  DSKBYTR;
    uint16_t  INTENAR;
    uint16_t  INTREQR;
    
    uint32_t CopperPC;
    uint32_t CopperState;
    uint16_t CopperIR1;
    uint16_t CopperIR2;
    uint32_t VBL;
    
    uint32_t bitplaneFetchActive;
    uint32_t Colour[32];
    uint8_t* chipram;
    uint32_t VSTOP;
    uint32_t HSTART;
    uint32_t HSTOP;
    uint8_t PixelBuffer[16];
    
    uint64_t DMACycles;
    uint32_t DMAFreq;
    uint64_t EClockcycles;
    
    //Internal Helper Variables
    uint32_t hires;
    uint32_t planeCount;
    
    //Host Framebuffer
    uint32_t* frameBuffer;
    int32_t frameBufferPitch;
    uint32_t* FrameBufferLine;
    uint32_t FrameBufferLineIndex;
    //uint32_t framebufferIndex;          //I don't think I need this anymore
    int HPOS;   //Lores pixel position
    int32_t  displayCountdown;
    int32_t  bitplaneFetchCountdown;
    uint32_t DisplayPositionAdjust;
    uint32_t needsRedraw;
    
    //Sprite
    SpriteInternal_t sprite[8];
    
    //Host Mouse
    int32_t mouseXrel;
    int32_t mouseYrel;
    
}Chipset_t;

void InitChipset(void* chipram, void* memory);

void WriteChipsetByte(unsigned int address, unsigned int value);
void WriteChipsetWord(unsigned int address, unsigned int value);
void WriteChipsetLong(unsigned int address, unsigned int value);

//void Write24bitByte()

void CheckInterrupts(void);

uint32_t IncrementVHPOS(void);

void DecodeCopperList(int list);

#endif /* Chipset_h */
