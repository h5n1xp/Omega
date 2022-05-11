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
} Omega_t __attribute__((packed));


#endif /* Header_h */

/*
 
 Sorry to bother you but I need to call upon your experience to help correct what I believe to be the flaw in my thinking, which is stopping me from moving forward with my Emulator project.

 So with version 1 of Omega, I think I accidentally stumbled upon some timing ratio of the CPU/Chipset/CIA which worked for kickstart 1.2/1.3, but never really worked for the newer Kickstarts. It was never a stated design goal of the emulator to be particularly compatible so I was never overly concerned about timing.

 With version 2 of Omega, I decided I wanted a higher degree of compatibility (with the intention of getting kickstart 3.1 to boot). So I am trying to make more of an effort to get the timing of the various components cycle accurate. But right now I can't get Kickstart 1.x to correctly set up it's boot screen copper list...
 */
