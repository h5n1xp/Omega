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
    
    void (*WriteWord[512])(uint16_t value);

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
    uint32_t Colour[64];    //Eventually I will support EHB
    uint8_t* chipram;
    uint32_t VSTOP;
    
    uint64_t DMACycles;
    uint32_t DMAFreq;
    uint64_t EClockcycles;
    
    //Host Framebuffer
    uint32_t* frameBuffer;
    int32_t frameBufferPitch;
    uint32_t framebufferIndex;
    int32_t  bitplaneFetchCountdown;
    uint32_t needsRedraw;
    
    //Host Mouse
    int32_t mouseXrel;
    int32_t mouseYrel;
    
}Chipset_t;

void InitChipset(void* chipram, void* memory);

void WriteChipsetByte(unsigned int address, unsigned int value);
void WriteChipsetWord(unsigned int address, unsigned int value);
void WriteChipsetLong(unsigned int address, unsigned int value);

void CheckInterrupts(void);

uint32_t IncrementVHPOS(void);

#endif /* Chipset_h */
