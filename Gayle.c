//
//  Gayle.c
//  Omega
//
//  Created by Matt Parsons on 06/03/2019.
//  Copyright © 2019 Matt Parsons. All rights reserved.
//

#include "Gayle.h"

#define CLOCKBASE 0xDC0000


void writeGayleB(unsigned int address, unsigned int value){
    
    if(address>=CLOCKBASE){
        
        printf("Write Byte to clock 0x%06x (0x%06x)\n",address,value);
        return;
    }
    
    printf("Write Byte to Gayle Space 0x%06x (0x%06x)\n",address,value);
    
}

void writeGayle(unsigned int address, unsigned int value){
 
    if(address>=CLOCKBASE){
        
        printf("Write to clock 0x%06x (0x%06x)\n",address,value);
        return;
    }
    
    printf("Write to Gayle Space 0x%06x (0x%06x)\n",address,value);
    
}

void writeGayleL(unsigned int address, unsigned int value){
    
    if(address>=CLOCKBASE){
        
        printf("Write Long to clock 0x%06x (0x%06x)\n",address,value);
        return;
        
    }
    
    printf("Write Long to Gayle Space 0x%06x (0x%06x)\n",address,value);
    
}

uint8_t readGayleB(unsigned int address){
    
    if(address>=CLOCKBASE){
        
        printf("Read Byte From clock 0x%06x\n",address);
        return 0xFF;
        
    }
    
    printf("Read Byte From Gayle Space 0x%06x\n",address);
    return 0xFF;
}

uint16_t readGayle(unsigned int address){
    
    if(address>=CLOCKBASE){
        
        printf("Read From clock 0x%06x\n",address);
        return 0x8000;
        
    }
    
    printf("Read From Gayle Space 0x%06x\n",address);
    return 0x8000;
}

uint32_t readGayleL(unsigned int address){
    
    static int count = 0;
    
    if(address>=CLOCKBASE){
        
        printf("Read Long From clock 0x%06x\n",address);
        
        if(count==0){
            count +=1;
            return 1309036038;
        }
        
        return 1309101575;
        
        
    }

    printf("Read Long From Gayle Space 0x%06x\n",address);
    return 0x8000;
}
