//
//  Memory.c
//  The Omega Project
//  https://github.com/h5n1xp/Omega
//
//  Created by Matt Parsons on 02/02/2019.
//  Copyright © 2019 Matt Pasons. All rights reserved.
//  <h5n1xp@gmail.com>
//
//
//  This Source Code Form is subject to the terms of the
//  Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "Memory.h"
#include "Kick13.h"
#include "Chipset.h"
#include "CIA.h"



unsigned char low16Meg[16777216];

//Chipset_t* chipset = (Chipset_t*)&low16Meg[0xDFF000];

void loadROM(){
    
    for(int i=0;i<524288;++i){
         low16Meg[0xF80000+i] = kick13[i];
    }
}


unsigned int chipReadByte(unsigned int address){
    //ROM
    if(address>0xF80000){
        return low16Meg[address];
    }
    
    if(address>0xDFFFFF){
        return 0;
    }
    
    //Chipregs
    if(address>0xDFEFFF){
        address = (address - 0xDFF000) >> 1;
        //return ChipsetReadByte(&chipset, address);
        return getChipReg8[address]();
    }
    
    //Slow RAM
    if(address>0xBFFFFF){
        return low16Meg[address];
    }
    
    //CIA A
    if(address>=0xBFE001){
        address = (address - 0xBFE001) >> 8;
        return CIARead(&CIAA,address);
    }
    
    //CIA B
    if(address>=0xBFD000){
        address = (address - 0xBFD000) >> 8;
        return CIARead(&CIAB,address);
    }
    
    //24bit fast ram
    if(address>0x1FFFFF){
        return 0;
    }
    
    //RAM
    address &=CHIPTOP;
    return low16Meg[address];
}
unsigned int chipReadWord(unsigned int address){
    //ROM
    if(address>0xF7FFFF){
        //return READ_WORD(chipset.rom,address-0xF80000);
        uint16_t value = *(uint16_t*)&low16Meg[address];
        value = (value << 8) | (value >> 8);
        return value;
        
    }
    
    if(address>0xDFFFFF){
        return 0;
    }
    
    //Chipregs
    if(address>0xDFEFFF){
        address = (address - 0xDFF000) >> 1;
        return getChipReg16[address]();
        //return ChipsetRead(&chipset, address);
    }
    
    //Slow RAM
    if(address>0xBFFFFF){
        
#ifdef NOSLOWRAM
        //Chipset shadow - incomplete address decoding
        address &=0xFFF;
        //return ChipsetRead(&chipset, address >> 1);
        return getChipReg16[address >> 1]();
#endif
        
        //return READ_WORD(chipset.chipram,address);
        uint16_t value = *(uint16_t*)&low16Meg[address];
        value = (value << 8) | (value >> 8);
        return value;
    }
    
    //CIA A
    if(address>=0xBFE001){
        address = (address - 0xBFE001) >> 8;
        return CIARead(&CIAA,address);
    }
    
    //CIA B
    if(address>=0xBFD000){
        address = (address - 0xBFD000) >> 8;
        return CIARead(&CIAB,address);
    }
    
    //24bit fast ram
    if(address>0x1FFFFF){
        return 0;
    }
    
    //RAM - Incomplete address decding means chip addresses below 2meg wrap around
    address &=CHIPTOP;
    
    //return READ_WORD(chipset.chipram,address);
    uint16_t value = *(uint16_t*)&low16Meg[address];
    value = (value << 8) | (value >> 8);
    return value;
}
unsigned int chipReadLong(unsigned int address){
    
    //ROM
    if(address>0xF7FFFF){
        //return READ_LONG(chipset.rom,address-0xF80000);
        uint32_t value = *(uint32_t*)&low16Meg[address];
        value = ((value << 8) & 0xFF00FF00 ) | ((value >> 8) & 0xFF00FF );
        return value << 16 | value >> 16;
    }
    
    //Chipregs
    if(address>0xDFEFFF){
        address = (address - 0xDFF000) >> 1;
        //return ChipsetReadLong(&chipset, address);
        return getChipReg32[address]();
    }
    
    //Slow RAM
    if(address>0xBFFFFF){
        //return READ_LONG(chipset.chipram,address);
        uint32_t value = *(uint32_t*)&low16Meg[address];
        value = ((value << 8) & 0xFF00FF00 ) | ((value >> 8) & 0xFF00FF );
        return value << 16 | value >> 16;
    }
    
    //CIA A
    if(address>=0xBFE001){
        address = (address - 0xBFE001) >> 8;
        return CIARead(&CIAA,address);
    }
    
    //CIA B
    if(address>=0xBFD000){
        address = (address - 0xBFD000) >> 8;
        return CIARead(&CIAB,address);
    }
    
    //24bit fast ram
    if(address>0x1FFFFF){
        return 0;
    }
    
    //RAM - Incomplete address decding means chip addresses below 2meg wrap around
    address &=CHIPTOP;
    
    //return READ_LONG(chipset.chipram,address);
    uint32_t value = *(uint32_t*)&low16Meg[address];
    value = ((value << 8) & 0xFF00FF00 ) | ((value >> 8) & 0xFF00FF );
    return value << 16 | value >> 16;
}

void chipWriteByte(unsigned int address,unsigned int value){   //ROM
    if(address>0xF80000){
        return;
    }
    
    //Chipregs
    if(address>0xDFEFFF){
        address = (address - 0xDFF000) >> 1;
        //ChipsetWrite(&chipset, address,value); // Does any software byte write to the custom chips?
        return;
    }
    
    //Slow RAM
    if(address>0xBFFFFF){
        low16Meg[address] = value; return;
    }
    
    //CIA A
    if(address>=0xBFE001){
        address = (address - 0xBFE001) >> 8;
        CIAWrite(&CIAA,address,value);return;
    }
    
    //CIA B
    if(address>=0xBFD000){
        address = (address - 0xBFD000) >> 8;
        CIAWrite(&CIAB,address,value);return;
    }
    
    //24bit fast ram
    if(address>0x1FFFFF){
        return;
    }
    
    //RAM - Incomplete address decding means chip addresses below 2meg wrap around
    address &=CHIPTOP;
    low16Meg[address] = value;
}

void chipWriteWord(unsigned int address,unsigned int value){
    //ROM
    if(address>0xF80000){
        return;
    }
    
    //Chipregs
    if(address>0xDFEFFF){
        address = (address - 0xDFF000) >> 1;
        putChipReg16[address](value);
        return;
    }
    
    //SLow RAM
    if(address>0xBFFFFF){
        
        //printf("Write %d Chipset Shadow @ %06x\n",value,address);
        
        /*
         if(address==0xDBF09){
         return ChipsetWrite(&chipset, (address &0xFFF) >> 1,value);
         }
         */
        
#ifdef NOSLOWRAM
        //Chipset shadow - incomplete address decoding
        address &=0xFFF;
        //ChipsetWrite(&chipset, address >> 1,value); return;
        putChipReg16[address >> 1](value);
#endif
        
        //WRITE_WORD(chipset.chipram,address,value);return;
        uint16_t* dest = (uint16_t*)&low16Meg[address];
        value = (value << 8) | (value >> 8);
        *dest = value;return;
    }
    
    //CIA A
    if(address>=0xBFE001){
        return;
    }
    
    //CIA B
    if(address>=0xBFD000){
        return;
    }
    
    //24bit fast ram
    if(address>0x1FFFFF){
        return;
    }
    
    //RAM - Incomplete address decding means chip addresses below 2meg wrap around
    address &=CHIPTOP;
    
    //WRITE_WORD(chipset.chipram,address,value);
    uint16_t* dest = (uint16_t*)&low16Meg[address];
    value = (value << 8) | (value >> 8);
    *dest = value;return;

}
void chipWriteLong(unsigned int address,unsigned int value){
    //ROM
    if(address>0xF80000){
        return;
    }
    
    //Chipregs
    if(address>0xDFEFFF){
        address = (address - 0xDFF000) >> 1;
        putChipReg32[address](value);
        return;
    }
    
    //Slow RAM
    if(address>0xBFFFFF){
        //WRITE_LONG(chipset.chipram,address,value);
        uint32_t* dest = (uint32_t*)&low16Meg[address];
        value = ((value << 8) & 0xFF00FF00 ) | ((value >> 8) & 0xFF00FF );
        value = value << 16 | value >> 16;
        *dest = value; return;
    }
    
    //CIA A
    if(address>=0xBFE001){
        return;
    }
    
    //CIA B
    if(address>=0xBFD000){
        return;
    }
    
    //24bit fast ram
    if(address>0x1FFFFF){
        return;
    }
    
    //RAM - Incomplete address decding means chip addresses below 2meg wrap around
    address &=CHIPTOP;
    
    //WRITE_LONG(chipset.chipram,address,value);
    uint32_t* dest = (uint32_t*)&low16Meg[address];
    value = ((value << 8) & 0xFF00FF00 ) | ((value >> 8) & 0xFF00FF );
    value = value << 16 | value >> 16;
    *dest = value; return;

}




