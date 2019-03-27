//
//  CPU.c
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

#include <stdio.h>
#include "CPU.h"
#include "m68k.h"

#include "Memory.h"

#include "CIA.h"


unsigned char* _emulatorMemory;

//#define NOSLOWRAM

//#define CHIPTOP  0x3FFFF // for 256K Chip
//#define CHIPTOP  0x7FFFF // for 512K
//#define CHIPTOP  0xFFFFF // for 1Meg
//#define CHIPTOP 0x1FFFFF // for 2Meg

/* Called when the CPU pulses the RESET line */
void cpu_pulse_reset(void){
    uint32_t pc = m68k_get_reg(NULL, M68K_REG_PC);
    uint32_t sp = m68k_get_reg(NULL, M68K_REG_SP);

    printf("PC:%0x6\n",pc);
    printf("SP:%0x6\n",sp);
    
    printf("Machine reset...\n");
    
    //clear RAM (don't wipe long at address 0 as that signals a Guru occured)
    for(int i=4;i<0x1FFFFF;i++){
        low16Meg[i]=0;
    }
    ChipsetInit();
    m68k_set_reg(M68K_REG_PC, 0xF80002); //start executing at the ROM + 2
    
}


void cpu_init(){

    //set CIA IDs
    CIAInit(&CIAA, 32776);
    CIAInit(&CIAB, 40960);
    
    m68k_init();
    m68k_set_cpu_type(M68K_CPU_TYPE_68000); //Pretend to be an A500 for now...
    m68k_set_reg(M68K_REG_PC, 0xF80002); //start executing at the ROM + 2

}

void cpu_execute(){
 
/*
    int* dis = &disass;
    uint32_t pc = m68k_get_reg(NULL, M68K_REG_PC);
    uint32_t A0 = m68k_get_reg(NULL, M68K_REG_A0);
    uint32_t A1 = m68k_get_reg(NULL, M68K_REG_A1);
    uint32_t A2 = m68k_get_reg(NULL, M68K_REG_A2);
    uint32_t A3 = m68k_get_reg(NULL, M68K_REG_A3);
    uint32_t A4 = m68k_get_reg(NULL, M68K_REG_A4);
    uint32_t A5 = m68k_get_reg(NULL, M68K_REG_A5);
    uint32_t A6 = m68k_get_reg(NULL, M68K_REG_A6);
    uint32_t A7 = m68k_get_reg(NULL, M68K_REG_A7);
    uint32_t D0 = m68k_get_reg(NULL, M68K_REG_D0);
    uint32_t D1 = m68k_get_reg(NULL, M68K_REG_D1);
    uint32_t D2 = m68k_get_reg(NULL, M68K_REG_D2);
    uint32_t D3 = m68k_get_reg(NULL, M68K_REG_D3);
    uint32_t D4 = m68k_get_reg(NULL, M68K_REG_D4);
    uint32_t D5 = m68k_get_reg(NULL, M68K_REG_D5);
    uint32_t D6 = m68k_get_reg(NULL, M68K_REG_D6);
    uint32_t D7 = m68k_get_reg(NULL, M68K_REG_D7);
    
 
    if(pc==0xFC6CFC){
       // printf("Trap\n");
        //disass =1;
    }
    
    uint32_t* guru = low16Meg;
 */
    
    m68k_execute(16);

}




void checkInterrupt(Chipset_t* chipset){
    
    if(chipset->intenar & 0x4000){ // if master interrupt switch is enabled
        
        uint16_t intMask = chipset->intreqr & chipset->intenar; //only set the int level, if bits are enabled
        
        if(intMask !=0){
            
            m68k_end_timeslice();
            
            if(intMask & 8192){  // External int
                m68k_set_irq(6);
            }else if(intMask & 4096){ // Disk sync
                m68k_set_irq(5);
            }else if(intMask & 2048){ // Serial receive buffer full
                m68k_set_irq(5);
            }else if(intMask & 1024){ // Audio 3
                m68k_set_irq(4);
            }else if(intMask & 512){  // Audio 2
                m68k_set_irq(4);
            }else if(intMask & 256){  // Audio 1
                m68k_set_irq(4);
            }else if(intMask & 128){  // Audio 0
                m68k_set_irq(4);
            }else if(intMask & 64){   // Blitter finished
                m68k_set_irq(3);
            }else if(intMask & 32){   // VBL
                m68k_set_irq(3);
            }else if(intMask & 16){   // Copper
                m68k_set_irq(3);
            }else if(intMask & 8){    // Ports, IO and timers
                m68k_set_irq(2);
            }else if(intMask & 4){    // Software int
                m68k_set_irq(1);
            }else if(intMask & 2){    // Disk block finished
                m68k_set_irq(1);
            }else if(intMask & 1){    // Serial Transmit buffer empty
                m68k_set_irq(1);
            }
            
        }else{
            m68k_set_irq(0);    // no interupt pending
        }
        
    }else{
        m68k_set_irq(0);    // no interupt if master INT disabled
    }
    
}




//Emulator functions

unsigned int cpu_read_byte(unsigned int address){
    
    //If Address > 0xFFFFFF (the low16Meg), then access the BCM2837 chip
    //Not needed if running on a real RaspberryPi. just allow the CPU to access the memory.
    
    //maskout 32bit address for now
    address &=0xFFFFFF;
    
    return chipReadByte(address);
    
}

unsigned int cpu_read_word(unsigned int address){
    
    //If Address > 0xFFFFFF (the low16Meg), then access the BCM2837 chip
    //Not needed if running on a real RaspberryPi. just allow the CPU to access the memory.
    
    return chipReadWord(address);
    
}

unsigned int cpu_read_long(unsigned int address){
    
    //If Address > 0xFFFFFF (the low16Meg), then access the BCM2837 chip
    //Not needed if running on a real RaspberryPi. just allow the CPU to access the memory.
    
    //maskout 32bit address for now
    address &=0xFFFFFF;
    
    return chipReadLong(address);
    
}


void cpu_write_byte(unsigned int address,unsigned int value){
    
    //If Address > 0xFFFFFF (the low16Meg), then access the BCM2837 chip
    //Not needed if running on a real RaspberryPi. just allow the CPU to access the memory.
    
    chipWriteByte(address, value);
    
}

void cpu_write_word(unsigned int address,unsigned int value){
    
    //If Address > 0xFFFFFF (the low16Meg), then access the BCM2837 chip
    //Not needed if running on a real RaspberryPi. just allow the CPU to access the memory.
    
    chipWriteWord(address, value);
    
}

void cpu_write_long(unsigned int address,unsigned int value){
    
    //If Address > 0xFFFFFF (the low16Meg), then access the BCM2837 chip
    //Not needed if running on a real RaspberryPi. just allow the CPU to access the memory.
    
    chipWriteLong(address, value);

}






// Called when the CPU acknowledges an interrupt
int cpu_irq_ack(int level)
{
    return level;
    /*
     switch(level)
     {
     case 0:
     return 0;//nmi_device_ack();
     case 1:
     return 0;// input_device_ack();
     case 2:
     return 0;// output_device_ack();
     }
     */
    return M68K_INT_ACK_SPURIOUS;
}


unsigned int cpu_read_word_dasm(unsigned int address){
    if(address > 16777216)
        printf("Disassembler attempted to read word from address %x", address);
    return cpu_read_word(address);
}

unsigned int cpu_read_long_dasm(unsigned int address){
    if(address > 16777216)
        printf("Dasm attempted to read long from address %x", address);
    return cpu_read_long(address);
}





/* Disassembler */

void make_hex(char* buff, unsigned int pc, unsigned int length){
    char* ptr = buff;
    
    for(;length>0;length -= 2)
    {
        sprintf(ptr, "%04x", cpu_read_word_dasm(pc));
        pc += 2;
        ptr += 4;
        if(length > 2)
            *ptr++ = ' ';
    }
}

void disassemble_program(){
    unsigned int pc;
    unsigned int instr_size;
    char buff[100];
    char buff2[100];
    
    pc = cpu_read_long_dasm(4);
    
    while(pc <= 0x16e)
    {
        instr_size = m68k_disassemble(buff, pc, M68K_CPU_TYPE_68000);
        make_hex(buff2, pc, instr_size);
        printf("%03x: %-20s: %s\n", pc, buff2, buff);
        pc += instr_size;
    }
    fflush(stdout);
}



void cpu_instr_callback(){

    
    
    // The following code will print out instructions as they are executed
    static char buff[100];
    static char buff2[100];
    static unsigned int pc;
    static unsigned int instr_size;
    
    pc = m68k_get_reg(NULL, M68K_REG_PC);
    instr_size = m68k_disassemble(buff, pc, M68K_CPU_TYPE_68000);
    
    
    
    if(disass<1){
        return;
    }
    
    
    
    make_hex(buff2, pc, instr_size);
        
    //fprintf(globalFD,"E %03x: %-20s: %s\n", pc, buff2, buff);
    printf("E %03x: %-20s: %s\n", pc, buff2, buff);
    fflush(stdout);
    //SDL_Delay(75);

    
    
    
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                            //
//                                                                                                            //
//                                                                                                            //
//                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
