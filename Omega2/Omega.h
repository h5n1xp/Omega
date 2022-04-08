//
//  Header.h
//  Omega2
//
//  Created by Matt Parsons on 30/03/2022.
//

#ifndef Header_h
#define Header_h

#include <stdint.h>

typedef struct{
    
    uint8_t chipRAM[2097152];
    uint8_t ZIISpace[8388608];
    
    //2031616 bytes here ideal for use by Omega Emulator internal function tables etc as only used by PCMCIA...
    uint8_t CIAState[524288];       // 512Kb for CIA State  (Way too much)
    uint8_t Chipstate[524288];      // 512Kb for Chipset state (adjust as needed)
    uint8_t Reserved1[983040];      // Free for other uses
    
    uint8_t CIASpace[65536];
    uint8_t SlowRAM[1572864];
    uint8_t Reserved2[131072];
    uint8_t IDEController[131072];
    uint8_t RealTimeClock[65536];
    uint8_t Reserved3[65536];       // Used for A3000/A4000 SCSI
    uint8_t MainBoardResources[65536];
    uint8_t CustomChipRegisters[65536];
    uint8_t AutoConfigSpace[1048576];
    uint8_t ExtendedROM[524288];
    uint8_t KickstartROM[524288];
} Omega_t;


#endif /* Header_h */
