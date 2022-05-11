//
//  CIA.c
//  Omega2
//
//  Created by Matt Parsons on 28/03/2022.
//

#include "CIA.h"
#include "Memory.h"
#include "Floppy.h"

//Remove when we have an internal Memory allocation system
//#include <stdlib.h>



CIA_t* CIAState;
Chipset_t* ChipsetStateCIA;

void RunCIACycle(){
    
    //CIA A Timer A
    if(RAM24bit[CIA_ACRA] & 1){
        CIAState->ATA -= 1;
        
        if(CIAState->ATA == 65535){
            CIAState->ATA = CIAState->ATAPreScaler;
            
            //If we are oneshot mode, stop the timer
            if(RAM24bit[CIA_ACRA] & 8){
                RAM24bit[CIA_ACRA] = RAM24bit[CIA_ACRA] & 254; // Stop the Timer
            }
            
            //Check the interrupt mask
            if(CIAState->AICRMask & 1){
                RAM24bit[CIA_AICR] = RAM24bit[CIA_AICR] | 1;
                //ChipsetStateCIA->WriteWord[0x9C](32776); //Generate interrupt 3
            }
        }
        
        RAM24bit[CIA_ATALO] = CIAState->ATA & 255;
        RAM24bit[CIA_ATAHI] = (CIAState->ATA >> 8);
    }
    
    
    //CIA A Timer B
    if(RAM24bit[CIA_ACRB] & 1){
        CIAState->ATB -= 1;

        if(CIAState->ATB == 65535){
            CIAState->ATB = CIAState->ATBPreScaler;
            
            //If we are oneshot mode, stop the timer
            if(RAM24bit[CIA_ACRB] & 8){
                RAM24bit[CIA_ACRB] = RAM24bit[CIA_ACRB] & 254; // Stop the Timer
            }
            
            //Check the interrupt mask
            if(CIAState->AICRMask & 2){
                RAM24bit[CIA_AICR] = RAM24bit[CIA_AICR] | 2;
                //ChipsetStateCIA->WriteWord[0x9C](32776); //Generate interrupt 3
            }
        }
        
        RAM24bit[CIA_ATBLO] = CIAState->ATB & 255;
        RAM24bit[CIA_ATBHI] = (CIAState->ATB >> 8);
    }
    
    
    //CIA B Timer A
    if(RAM24bit[CIA_BCRA] & 1){
        CIAState->BTA -= 1;
        
        if(CIAState->BTA == 65535){
            CIAState->BTA = CIAState->BTAPreScaler;
            
            //If we are oneshot mode, stop the timer
            if(RAM24bit[CIA_BCRA] & 8){
                RAM24bit[CIA_BCRA] = RAM24bit[CIA_BCRA] & 254; // Stop the Timer
            }
            
            //Check the interrupt mask
            if(CIAState->BICRMask & 1){
                RAM24bit[CIA_BICR] = RAM24bit[CIA_BICR] | 1;
                //ChipsetStateCIA->WriteWord[0x9C](40960); //Generate interrupt 13
            }
        }
        
        RAM24bit[CIA_BTALO] = CIAState->BTA & 255;
        RAM24bit[CIA_BTAHI] = (CIAState->BTA >> 8) ;
    }
    
    //CIA B Timer B
    if(RAM24bit[CIA_BCRB] & 1){
        CIAState->BTB -= 1;
        
        if(CIAState->BTB == 65535){
            CIAState->BTB = CIAState->BTBPreScaler;
            
            //If we are oneshot mode, stop the timer
            if(RAM24bit[CIA_BCRB] & 8){
                RAM24bit[CIA_BCRB] = RAM24bit[CIA_BCRB] & 254; // Stop the Timer
            }
            
            //Check the interrupt mask
            if(CIAState->BICRMask & 2){
                RAM24bit[CIA_BICR] = RAM24bit[CIA_BICR] | 2;
                //ChipsetStateCIA->WriteWord[0x9C](40960); //Generate interrupt 13
            }
        }
        
        RAM24bit[CIA_BTBLO] = CIAState->BTB & 255;
        RAM24bit[CIA_BTBHI] = (CIAState->BTB >> 8);
    }
    
    //Need to check interrupts, if any of the flags are set, then we need to raise an interrupt with Paula...and keep doing it until the offending ICR cleared
    if(RAM24bit[CIA_AICR]){
        ChipsetStateCIA->WriteWord[0x9C](32776); //Generate interrupt 3
    }
    
    if(RAM24bit[CIA_BICR]){
        ChipsetStateCIA->WriteWord[0x9C](40960); //Generate interrupt 13
    }
    
}

// ******************** Pin signals

//The VBL Counter
void CIAATOD(void){
    
    if(CIAState->ATODLatch == 1){
        return;
    }
    
    CIAState->ATOD += 1;
    //if( CIAState->ATOD == 16777216){
    //    CIAState->ATOD = 0;
    //}
    CIAState->ATOD &= 0xFFFFFF; //wrap to 0
    
    RAM24bit[CIA_ATODL] =  CIAState->ATOD & 255;
    RAM24bit[CIA_ATODM] = (CIAState->ATOD >> 8) & 255;
    RAM24bit[CIA_ATODH] = (CIAState->ATOD >>16) & 255;
    
    //Check Alarm
    if(CIAState->AICRMask & 4){
        
        if (CIAState->ATOD == CIAState->ATODAlarm){
            RAM24bit[0xBFED01] = RAM24bit[0xBFED01] | 4;
            ChipsetStateCIA->WriteWord[0x9C](32776); //Generate interrupt 3
        }
    }
    
    //printf("%d Frames (%d Seconds)\n",CIAState->ATOD,CIAState->ATOD/60);
}



//The Horizontal line counter Counter
void CIABTOD(void){
    
    if(CIAState->BTODLatch == 1){
        return;
    }
    
    CIAState->BTOD += 1;
   // if( CIAState->BTOD == 16777216){
   //     CIAState->BTOD = 0;
   // }
    
    CIAState->BTOD &= 0xFFFFFF; //wrap to 0
    
    RAM24bit[CIA_BTODL] =  CIAState->BTOD & 255;
    RAM24bit[CIA_BTODM] = (CIAState->BTOD >> 8) & 255;
    RAM24bit[CIA_BTODH] = (CIAState->BTOD >>16) & 255;
    
    //Check Alarm
    if(CIAState->BICRMask & 4){
        
        if (CIAState->BTOD == CIAState->BTODAlarm){
            RAM24bit[CIA_BICR] = RAM24bit[CIA_BICR] | 4;
            ChipsetStateCIA->WriteWord[0x9C](40960); //Generate interrupt 13
        }
    }
}

void SetTrack0(void){
    RAM24bit[CIA_APRA] = RAM24bit[CIA_APRA] & 0xEF;
    return;
}

void ClearTrack0(void){
    RAM24bit[CIA_APRA] = RAM24bit[CIA_APRA] | 0x10;
    return;
}


void SetDriveReady(void){

    RAM24bit[CIA_APRA] = RAM24bit[CIA_APRA] & 0xDF;
    
}

void ClearDriveReady(void){

    RAM24bit[CIA_APRA] = RAM24bit[CIA_APRA] | 0x20;
}

void SetDriveFull(void){

    RAM24bit[CIA_APRA] = RAM24bit[CIA_APRA] | 0x4;                // High when inserted
}

void SetDriveEmpty(void){

    RAM24bit[CIA_APRA] =  RAM24bit[CIA_APRA] & 0xFB;              // Low when empty
    
}


void DiskIndexPulse(){

    if(CIAState->BICRMask & 0x10){
            RAM24bit[CIA_BICR] = RAM24bit[CIA_BICR] | 0x10;
            ChipsetStateCIA->WriteWord[0x9C](40960); //Generate interrupt 13
    }
}

// ************ Registers
void ASetPRA(uint8_t value){
    
    RAM24bit[CIA_APRA] = (RAM24bit[CIA_APRA] & 0xFC) | (value & 0x03); // The top bits are unwritable.
    
    return;
}

void ASetPRB(uint8_t value){
    RAM24bit[CIA_APRB] = value;
    return;
}

void ASetDDRA(uint8_t value){
    RAM24bit[CIA_ADDRA] = value;
    return;
}

void ASetDDRB(uint8_t value){
    RAM24bit[CIA_ADDRB] = value;
    return;
}

void ASetTALO(uint8_t value){
    CIAState->ATAPreScaler = (CIAState->ATAPreScaler & 65280) | value;
    
    //If timer not running then load in to register
    if( (RAM24bit[CIA_ACRA] & 1) == 0){
        CIAState->ATA = (CIAState->ATA & 0xFF00) | value;
    }
    
    return;
}

void ASetTAHI(uint8_t value){
    CIAState->ATAPreScaler = (CIAState->ATAPreScaler & 255) | value << 8;
    
    if(RAM24bit[CIA_ACRA] & 8){
        CIAState->ATA = CIAState->ATAPreScaler;
        RAM24bit[CIA_ACRA] = RAM24bit[CIA_ACRA] | 1;
    }
    
    if( (RAM24bit[CIA_ACRA] & 1) == 0){
        CIAState->ATA = CIAState->ATAPreScaler;
    }
    
    return;
}

void ASetTBLO(uint8_t value){
    CIAState->ATBPreScaler = (CIAState->ATBPreScaler & 65280) | value;
    
    //If timer not running then load in to register
    if( (RAM24bit[CIA_ACRB] & 1) == 0){
        CIAState->ATB = (CIAState->ATB & 0xFF00) | value;
    }
    
    return;
}

void ASetTBHI(uint8_t value){
    CIAState->ATBPreScaler = (CIAState->ATBPreScaler & 255) | value << 8;
    
    if(RAM24bit[CIA_ACRB] & 8){
        CIAState->ATB = CIAState->ATBPreScaler;
        RAM24bit[CIA_ACRB] = RAM24bit[CIA_ACRB] | 1;
    }
    
    if( (RAM24bit[CIA_ACRB] & 1) == 0){
        CIAState->ATB = CIAState->ATAPreScaler;
    }
    
    return;
}

void ASetTODLO(uint8_t value){
    
    if(RAM24bit[CIA_ACRB] & 128){
        CIAState->ATODAlarm = (CIAState->ATODAlarm & 16776960) | value;
        return;
    }
    
    CIAState->ATOD = (CIAState->ATOD & 16776960) | value;
    RAM24bit[CIA_ATODL] = value;
    CIAState->ATODLatch = 0;
    return;
}

void ASetTODMID(uint8_t value){
    
    if(RAM24bit[CIA_ACRB] & 128){
        CIAState->ATODAlarm = (CIAState->ATODAlarm & 16711935) | value << 8;
        return;
    }
    
    CIAState->ATODLatch = 1;
    CIAState->ATOD = (CIAState->ATOD & 16711935) | value << 8;
    RAM24bit[CIA_ATODM] = value;

    return;
}

void ASetTODHI(uint8_t value){
    
    if(RAM24bit[CIA_ACRB] & 128){
        CIAState->ATODAlarm = (CIAState->ATODAlarm & 65535) | value << 16;
        return;
    }
    
    CIAState->ATODLatch = 1;
    CIAState->ATOD = (CIAState->ATOD & 65535) | value << 16;
    RAM24bit[CIA_ATODH] = value;
    return;
}


void ASetSDR(uint8_t value){
    RAM24bit[CIA_ASDR] = value;
    
    if( (CIAState->AICRMask & 8)){
        RAM24bit[CIA_AICR] = RAM24bit[CIA_AICR] | 0x8;
        ChipsetStateCIA->WriteWord[0x9C](32776); //Generate interrupt 3
    }
        
    return;
}

void ASetICR(uint8_t value){
    
    //RAM24bit[0xBFED01]
    
    if(value & 128){
        CIAState->AICRMask = CIAState->AICRMask | (value & 127);
    }else{
        CIAState->AICRMask = CIAState->AICRMask ^ (value & CIAState->AICRMask); // clear requested bits
    }
    
    return;
}

void ASetCRA(uint8_t value){
    
    if(value & 0x26){
        printf("Trap Unhandled bits\n");
    }
    
    //Load the prescalervalue into the timer
    if(value & 0x10){
        CIAState->ATA = CIAState->ATAPreScaler;
        value = value & 0xEF;
    }
    
    RAM24bit[CIA_ACRA] = value;
    return;
}

void ASetCRB(uint8_t value){
    
    if(value & 0x66){
        printf("Trap Unhandled bits\n");
    }
    
    //Load the prescalervalue into the timer
    if(value & 0x10){
        CIAState->ATB = CIAState->ATBPreScaler;
        value = value & 0xEF;
    }
    
    RAM24bit[CIA_ACRB] = value;
    return;
}



void BSetPRA(uint8_t value){
    RAM24bit[CIA_BPRA] = value;
    return;
}

void BSetPRB(uint8_t value){
    RAM24bit[CIA_BPRB] = value;
    
    return;
}

void BSetDDRA(uint8_t value){
    RAM24bit[CIA_BDDRA] = value;
    return;
}

void BSetDDRB(uint8_t value){
    RAM24bit[CIA_BDDRB] = value;
    return;
}

void BSetTALO(uint8_t value){
    CIAState->BTAPreScaler = (CIAState->BTAPreScaler & 65280) | value;
    
    if( (RAM24bit[CIA_BCRA] & 1) == 0){
        CIAState->BTA = CIAState->BTAPreScaler;
    }
    
    return;
}

void BSetTAHI(uint8_t value){
    CIAState->BTAPreScaler = (CIAState->BTAPreScaler & 255) | value << 8;
    
    if(RAM24bit[CIA_BCRA] & 8){
        CIAState->BTA = CIAState->BTAPreScaler;
        RAM24bit[CIA_BCRA] = RAM24bit[CIA_BCRA] | 1;
    }
    
    if( (RAM24bit[CIA_BCRA] & 1) == 0){
        CIAState->BTA = CIAState->BTAPreScaler;
    }
    
    return;
}

void BSetTBLO(uint8_t value){
    CIAState->BTBPreScaler = (CIAState->BTBPreScaler & 65280) | value;
    
    if( (RAM24bit[CIA_BCRB] & 1) == 0){
        CIAState->BTB = CIAState->BTBPreScaler;
    }
    
    return;
}

void BSetTBHI(uint8_t value){
    CIAState->BTBPreScaler = (CIAState->BTBPreScaler & 255) | value << 8;
    
    if(RAM24bit[CIA_BCRB] & 8){
        CIAState->BTB = CIAState->BTBPreScaler;
        RAM24bit[CIA_BCRB] = RAM24bit[CIA_BCRB] | 1;
    }
    
    if( (RAM24bit[CIA_BCRB] & 1) == 0){
        CIAState->BTB = CIAState->BTBPreScaler;
    }
    
    return;
}

void BSetTODLO(uint8_t value){

    if(RAM24bit[CIA_BCRB] & 128){
        CIAState->BTODAlarm = (CIAState->BTODAlarm & 16776960) | value;
        return;
    }
    
    CIAState->BTOD = (CIAState->BTOD & 16776960) | value;
    RAM24bit[CIA_BTODL] = value;
    CIAState->BTODLatch = 0;
    return;
}

void BSetTODMID(uint8_t value){

    if(RAM24bit[CIA_BCRB] & 128){
        CIAState->BTODAlarm = (CIAState->BTODAlarm & 16711935) | value << 8;
        return;
    }
    

    CIAState->BTOD = (CIAState->BTOD & 16711935) | value << 8;
    RAM24bit[CIA_BTODM] = value;
    CIAState->BTODLatch = 1;
    return;
}

void BSetTODHI(uint8_t value){

    
    if(RAM24bit[CIA_BCRB] & 128){
        CIAState->BTODAlarm = (CIAState->BTODAlarm & 65535) | value << 16;
        return;
    }
    

    CIAState->BTOD = (CIAState->BTOD & 65535) | value << 16;
    RAM24bit[CIA_BTODH] = value;
    CIAState->BTODLatch = 1;
    return;
}


void BSetSDR(uint8_t value){
    RAM24bit[CIA_BSDR] = value;
    return;
}

void BSetICR(uint8_t value){
    
    if(value & 128){
        CIAState->BICRMask = CIAState->BICRMask | (value & 127);
    }else{
        CIAState->BICRMask = CIAState->BICRMask ^ (value & CIAState->BICRMask); // clear requested bits
    }

    return;
}

void BSetCRA(uint8_t value){
    
    if(value & 0x26){
        printf("Trap Unhandled bits\n");
    }
    
    //Load the prescalervalue into the timer
    if(value & 0x10){
        CIAState->BTA = CIAState->BTAPreScaler;
        value = value & 0xEF;
    }
    
    RAM24bit[CIA_BCRA] = value;
    return;
}

void BSetCRB(uint8_t value){
    
    if(value & 0x66){
        printf("Trap Unhandled bits\n");
    }
    
    //Load the prescalervalue into the timer
    if(value & 0x10){
        CIAState->BTB = CIAState->BTBPreScaler;
        value = value & 0xEF;
    }
    
    RAM24bit[CIA_BCRB] = value;
    return;
}

void NoCIAREG(uint8_t value){
    return;
}

void InitCIA(void* chipstate, void* memory){
 
    //CIAState = &CIAInternal;//calloc(1, sizeof(CIAState));
    CIAState = memory;//calloc(1, sizeof(CIAState));
    ChipsetStateCIA = chipstate;
    
    uint8_t* temp = memory;
    for(int i=0; i<262144; ++i){
        temp[i]=0;
    }
    
    CIAState->Write[0] = ASetPRA;
    CIAState->Write[1] = ASetPRB;
    CIAState->Write[2] = ASetDDRA;
    CIAState->Write[3] = ASetDDRB;
    CIAState->Write[4] = ASetTALO;
    CIAState->Write[5] = ASetTAHI;
    CIAState->Write[6] = ASetTBLO;
    CIAState->Write[7] = ASetTBHI;
    CIAState->Write[8] = ASetTODLO;
    CIAState->Write[9] = ASetTODMID;
    CIAState->Write[10] = ASetTODHI;
    CIAState->Write[11] = NoCIAREG;
    CIAState->Write[12] = ASetSDR;
    CIAState->Write[13] = ASetICR;
    CIAState->Write[14] = ASetCRA;
    CIAState->Write[15] = ASetCRB;

    CIAState->Write[16] = BSetPRA;
    CIAState->Write[17] = BSetPRB;
    CIAState->Write[18] = BSetDDRA;
    CIAState->Write[19] = BSetDDRB;
    CIAState->Write[20] = BSetTALO;
    CIAState->Write[21] = BSetTAHI;
    CIAState->Write[22] = BSetTBLO;
    CIAState->Write[23] = BSetTBHI;
    CIAState->Write[24] = BSetTODLO;
    CIAState->Write[25] = BSetTODMID;
    CIAState->Write[26] = BSetTODHI;
    CIAState->Write[27] = NoCIAREG;
    CIAState->Write[28] = BSetSDR;
    CIAState->Write[29] = BSetICR;
    CIAState->Write[30] = BSetCRA;
    CIAState->Write[31] = BSetCRB;
    
    CIAState->ATAPreScaler = 65535;
    CIAState->ATBPreScaler = 65535;
    CIAState->BTAPreScaler = 65535;
    CIAState->BTBPreScaler = 65535;
    
    //Set initial CIA State
    ChipsetStateCIA->chipram[CIA_APRA] = 0xFF;  // set CIA A PRA lines to "Overlay" | LED Dim | Floppy Not Ready | Disk Inserted | LMB Up | RMB Up
    ChipsetStateCIA->chipram[CIA_BPRB] = 0xEF; // set CIA B PRB lines low, DF0 Selected
    
    CIAState->ATA = 0;
    CIAState->ATB = 0;
    CIAState->BTA = 0;
    CIAState->BTB = 0;
    CIAState->ATODAlarm = 0xFFFFFF;
    CIAState->BTODAlarm = 0x0;      //Start Alarm at 0
    
    ASetCRA(0);
    ASetCRB(0);
    BSetCRA(0);
    BSetCRB(0);
    
    CIAState->AICRMask = 0xFF;
    CIAState->BICRMask = 0xFF;
    
    ASetSDR(0);
    

    
}


void WriteCIA(unsigned int address, uint8_t value){
    
    address = (address >> 8) & 31;
    CIAState->Write[address](value);
    
    FloppyCycle(); // Don't want to miss a signal...
    
    return;
}























































































































































































































































































































































//   ******************************* Keyboard handling ******************************************
const uint8_t keyMapping[] ={
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x41,
    0x42,
    0x0,
    0x0,
    0x0,
    0x44,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x45,
    0x0,
    0x0,
    0x0,
    0x0,
    0x40,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x2A,
    0x0,
    0x0,
    0x0,
    0x0,
    0x38,
    0xB,
    0x39,
    0x3A,
    0xA,
    0x01,
    0x02,
    0x03,
    0x04,
    0x05,
    0x06,
    0x07,
    0x08,
    0x09,
    0x50,   //F1
    0x51,   //F2
    0x52,   //F3
    0x53,   //F4
    0x54,   //F5
    0x55,   //F6
    0x56,   //F7
    0x57,   //F8
    0x58,   //F9
    0x59,   //F10
    0x46,   //F11 -> Del    //These 2 keys are mapped this way for laptops without Del and Help keys
    0x5F,   //F12 -> Help
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x4E,
    0x4F,
    0x4D,
    0x4C,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x1A,
    0x29, // should really be 2B, but have mapped to ;:
    0x1B,
    0x0,
    0x0,
    0x0,
    0x20,
    0x35,
    0x33,
    0x22,
    0x12,
    0x23,
    0x24,
    0x25,
    0x17,
    0x26,
    0x27,
    0x28,
    0x37,
    0x36,
    0x18,
    0x19,
    0x10,
    0x13,
    0x21,
    0x14,
    0x16,
    0x34,
    0x11,
    0x32,
    0x15,
    0x31,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x30,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x63,
    0x60,
    0x64,
    0x66,
    0x0,
    0x61,
    0x65,
    0x67,
    0x0,
    0x0,
    0x0,
    0x0
};

int keyMask = 0;       //This will flag when Ctrl-Amiga-Amiga has been pressed to trigger a reset

void pressKey(uint16_t keyCode){
    ASetSDR(~(keyMapping[keyCode]<<1) );      //place value in register

    //printf("keyCode: %d -> %02x (down)\n",keyCode,keyMapping[keyCode]);
    
    //Ctrl
    if(keyCode==224){
        keyMask |=0x1;
    }
    
    //Left Amiga
    if(keyCode==227){
        keyMask |=0x2;
    }
    
    //Right Amiga
    if(keyCode==231){
        keyMask |=0x4;
    }
    
    //Reset key combination pressed
    if(keyMask==0x7){
        cpu_pulse_reset();
    }
    
}

void releaseKey(uint16_t keyCode){
    ASetSDR(~((keyMapping[keyCode] << 1) | 1) ); //place value in register, with key up flag

    //printf("keyCode: %d -> %d (up)\n",keyCode,keyMapping[keyCode]);
    
    //Ctrl
    if(keyCode==224){
        keyMask &=0x6;
    }
    
    //Left Amiga
    if(keyCode==227){
        keyMask &=0x5;
    }
    
    //Right Amiga
    if(keyCode==231){
        keyMask &=0x3;
    }
    
}


//FLOPPY DISK

/*
 
 
 The following describes the interface under driver control.


 SEL1B-, SEL2B-, SEL3B-

    Select lines for the three external disk drives active low.


 TK0-

    A selected drive pulls this signal low whenever its read-write head
    is on track 00.


 RDY-

    When a disk drive's motor is on, this line indicates the selected
    disk is installed and rotating at speed. The driver ignores this
    signal. When the motor is off this is used as a  ID  data line. See
    below.


 WPRO- (Pin #14)

    A selected drive pulls this signal low whenever it has a
    write-protected diskette installed.


 INDEX- (Pin #22)

    A selected drive pulses this signal low once for each revolution of
    its motor.


 SIDEB- (Pin #13)

    The system drives this signal to all disk drives -- low for side 1,
    high for side 0.


 STEPB- (Pin #18)

    Pulsed to step the selected drive's head.


 DIRB (Pin #19)

    The system drives this signal high or low to tell the selected drive
    which way to step when the STEPB- pulse arrives. Low means step in
    (to higher-numbered track); high means step out.


 DKRD- (Pin #2)

    A selected drive will put out read data on this line.


 DKWDB- (Pin #17)

    The system drives write data to all disks via this signal. The data
    is only written when DKWEB- is active (low). Data is written only to
    selected drives.


 DKWEB- (Pin #16)

    This signal causes a selected drive to start writing data (provided
    by DKWDB-) onto the disk.


 CHNG- (Pin #11)

    A selected drive will drive this signal low whenever its internal
    "disk change" latch is set. This latch is set when the drive is first
    powered on, or whenever there is no diskette in the drive. To reset
    the latch, the system must select the drive, and step the head. Of
    course, the latch will not reset if there is no diskette installed.


 MTRXD- (Pin #8)

    This is the motor control line for all four disk drives. When the
    system wants to turn on a disk drive motor, it first deselects the
    drive (if selected), pulls MTRXD- low, and selects the drive. To turn
    the motor off, the system deselects the drive, pulls MTRXD- high, and
    selects the drive. The system will always set MTRXD- at least 1.4
    microseconds before it selects the drive, and will not change MTRXD-
    for at least 1.4 microseconds after selecting the drive. All external
    drives must have logic equivalent to a D flip-flop, whose D input is
    the MTRXD- signal, and whose clock input is activated by the
    off-to-on (high-to-low) transition of its SELxB- signal. As noted
    above, both the setup and hold times of MTRXD- with respect to SELxB-
    will always be at least 1.4 microseconds. The output of this
    flip-flop controls the disk drive motor. Thus, the system can control
    all four motors using only one signal on the cable (MTRXD-).


 DRESB- (Pin #10)

    This signal is a buffered version of the system reset signal. Three
    things can make it go active (low):

    *  System power-up (DRESB- will go low for approximately one
       second);

    *  System CPU executes a  RESET  instruction (DRESB- will go low for
       approximately 17 microseconds);

    *   Hard reset  from keyboard (lasts as long as keyboard reset is
       held down).

    External disk drives should respond to DRESB- by shutting off their
    motor flip-flops and write protecting themselves.


 A level of 3.75v or below on the 5v+ requires external disks to
 write-protect and reset the motor on line.
 
 */
