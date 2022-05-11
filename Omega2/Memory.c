//
//  Memory.c
//  Omega2
//
//  Created by Matt Parsons on 28/03/2022.
//

#include "Memory.h"
#include "kick12.h"
#include "kick13.h"
#include "m68k.h"
#include "CIA.h"
#include "Floppy.h"

#include <stdlib.h>

uint8_t* RAM24bit;

Omega_t omega;

int diss = 0;


char* regNames[] ={
    "BLTDDAT",
    "DMACONR",
    "VPOSR",
    "VHPOSR",
    "DSKDATR",
    "JOY0DAT",
    "JOY1DAT",
    "CLXDAT",
    "ADKCONR",
    "POT0DAT",
    "POT1DAT",
    "POTGOR",
    "SERDATR",
    "DSKBYTR",
    "INTENAR",
    "INTREQR",
    "DSKPTH",
    "DSKPTL",
    "DSKLEN",
    "DSKDAT",
    "REFPTR",
    "VPOSW",
    "VHPOSW",
    "COPCON",
    "SERDAT",
    "SERPER",
    "POTGO",
    "JOYTEST",
    "STREQU",
    "STRVBL",
    "STRHOR",
    "STRLONG",
    "BLTCON0",
    "BLTCON1",
    "BLTAFWM",
    "BLTALWM",
    "BLTCPTH",
    "BLTCPTL",
    "BLTBPTH",
    "BLTBPTL",
    "BLTAPTH",
    "BLTAPTL",
    "BLTDPTH",
    "BLTDPTL",
    "BLTSIZE",
    "BLTCON0L",
    "BLTSIZV",
    "BLTSIZH",
    "BLTCMOD",
    "BLTBMOD",
    "BLTAMOD",
    "BLTDMOD",
    "RESERVED00",
    "RESERVED01",
    "RESERVED02",
    "RESERVED03",
    "BLTCDAT",
    "BLTBDAT",
    "BLTADAT",
    "RESERVED04",
    "SPRHDAT",
    "RESERVED05",
    "DENISEID",
    "DSKSYNC",
    "COP1LCH",
    "COP1LCL",
    "COP2LCH",
    "COP2LCL",
    "COPJMP1",
    "COPJMP2",
    "COPINS",
    "DIWSTRT",
    "DIWSTOP",
    "DDFSTRT",
    "DDFSTOP",
    "DMACON",
    "CLXCON",
    "INTENA",
    "INTREQ",
    "ADKCON",
    "AUD0LCH",
    "AUD0LCL",
    "AUD0LEN",
    "AUD0PER",
    "AUD0VOL",
    "AUD0DAT",
    "RESERVED06",
    "RESERVED07",
    "AUD1LCH",
    "AUD1LCL",
    "AUD1LEN",
    "AUD1PER",
    "AUD1VOL",
    "AUD1DAT",
    "RESERVED08",
    "RESERVED09",
    "AUD2LCH",
    "AUD2LCL",
    "AUD2LEN",
    "AUD2PER",
    "AUD2VOL",
    "AUD2DAT",
    "RESERVED10",
    "RESERVED11",
    "AUD3LCH",
    "AUD3LCL",
    "AUD3LEN",
    "AUD3PER",
    "AUD3VOL",
    "AUD3DAT",
    "RESERVED12",
    "RESERVED13",
    "BPL1PTH",
    "BPL1PTL",
    "BPL2PTH",
    "BPL2PTL",
    "BPL3PTH",
    "BPL3PTL",
    "BPL4PTH",
    "BPL4PTL",
    "BPL5PTH",
    "BPL5PTL",
    "BPL6PTH",
    "BPL6PTL",
    "BPL7PTH",
    "BPL7PTL",
    "BPL8PTH",
    "BPL8PTL",
    "BPLCON0",
    "BPLCON1",
    "BPLCON2",
    "BPLCON3",
    "BPL1MOD",
    "BPL2MOD",
    "RESERVED14",
    "RESERVED15",
    "BPL1DAT",
    "BPL2DAT",
    "BPL3DAT",
    "BPL4DAT",
    "BPL5DAT",
    "BPL6DAT",
    "BPL7DAT",
    "BPL8DAT",
    "SPR0PTH",
    "SPR0PTL",
    "SPR1PTH",
    "SPR1PTL",
    "SPR2PTH",
    "SPR2PTL",
    "SPR3PTH",
    "SPR3PTL",
    "SPR4PTH",
    "SPR4PTL",
    "SPR5PTH",
    "SPR5PTL",
    "SPR6PTH",
    "SPR6PTL",
    "SPR7PTH",
    "SPR7PTL",
    "SPR0POS",
    "SPR0CTL",
    "SPR0DATA",
    "SPR0DATB",
    "SPR1POS",
    "SPR1CTL",
    "SPR1DATA",
    "SPR1DATB",
    "SPR2POS",
    "SPR2CTL",
    "SPR2DATA",
    "SPR2DATB",
    "SPR3POS",
    "SPR3CTL",
    "SPR3DATA",
    "SPR3DATB",
    "SPR4POS",
    "SPR4CTL",
    "SPR4DATA",
    "SPR4DATB",
    "SPR5POS",
    "SPR5CTL",
    "SPR5DATA",
    "SPR5DATB",
    "SPR6POS",
    "SPR6CTL",
    "SPR6DATA",
    "SPR6DATB",
    "SPR7POS",
    "SPR7CTL",
    "SPR7DATA",
    "SPR7DATB",
    "COLOR00",
    "COLOR01",
    "COLOR02",
    "COLOR03",
    "COLOR04",
    "COLOR05",
    "COLOR06",
    "COLOR07",
    "COLOR08",
    "COLOR09",
    "COLOR10",
    "COLOR11",
    "COLOR12",
    "COLOR13",
    "COLOR14",
    "COLOR15",
    "COLOR16",
    "COLOR17",
    "COLOR18",
    "COLOR19",
    "COLOR20",
    "COLOR21",
    "COLOR22",
    "COLOR23",
    "COLOR24",
    "COLOR25",
    "COLOR26",
    "COLOR27",
    "COLOR28",
    "COLOR29",
    "COLOR30",
    "COLOR31",
    "HTOTAL",
    "HSSTOP",
    "HBSTRT",
    "HBSTOP",
    "VTOTAL",
    "VSSTOP",
    "VBSTRT",
    "VBSTOP",
    "RESERVED16",
    "RESERVED17",
    "RESERVED18",
    "RESERVED19",
    "RESERVED20",
    "RESERVED21",
    "BEAMCON0",
    "HSSTRT",
    "VSSTRT",
    "HCENTER",
    "DIWHIGH",
    "RESERVED22",
    "RESERVED23",
    "RESERVED24",
    "RESERVED25",
    "RESERVED26",
    "RESERVED27",
    "RESERVED28",
    "RESERVED29",
    "RESERVED30",
    "RESERVED31",
    "RESERVED32",
    "RESERVED33",
    "NO-OP"
};



void BigEndianWrite(unsigned int address, enum DataSize size, unsigned int value){
    
    uint16_t* p16;
    uint32_t* p32;
    
    switch(size){
        case m68kByte:
            WRITEBYTE(address, value);
            return;
        case m68kWord:
             p16 = (uint16_t*) &RAM24bit[address];
            *p16 = (value >> 8) | (value << 8);
            return;
        case m68kLong:
             p32 = (uint32_t*) &RAM24bit[address];
            *p32 = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0xFF00FF);
            *p32 = *p32 << 16 | *p32 >> 16;
            return;
    }
    
}


uint32_t BigEndianRead(unsigned int address, enum DataSize size){
    
    uint16_t* p16;
    uint32_t* p;
    uint32_t retVal;
    
    switch(size){
        case m68kByte:
            return READBYTE(address);
        case m68kWord:
            p16 = (uint16_t*) &RAM24bit[address];
            uint16_t retVal16 = (*p16 >> 8) | (*p16 << 8);
            return retVal16;
        case m68kLong:
             p = (uint32_t*) &RAM24bit[address];
            retVal = ((*p << 8) & 0xFF00FF00) | ((*p >> 8) & 0xFF00FF);
            retVal = retVal << 16 | retVal >> 16;
            return retVal;
    }
    
}


uint32_t ZeroLong = 0;


unsigned int RAM24BitDespatch(uint32_t address, enum DataSize size,enum DataDirection direction, unsigned int value){

    if(address > 0xFFFFFF){
        address &= 0xFFFFFF;
        //return 0;
    }

    
    
    
    switch((address >> 21) & 0x7){
            
        case 0://ChipRAM
            
            //address = address & 0x7FFFF;
            
            switch(direction){
                case m68kWrite: BigEndianWrite(address, size, value); return 0;
                case m68kRead: return BigEndianRead(address, size);
            }
            break;
            
        case 1://ZII Space
            
            switch(direction){
                case m68kWrite: BigEndianWrite(address, size, value); return 0;
                case m68kRead: return BigEndianRead(address, size);
            }
            break;
            
            return 0;
            break;
        case 2://ZII Space
            return 0;
            break;
        case 3://ZII Space
            return 0;
            break;
        case 4://Unused
            return 0;
            break;
        case 5://Reserved - used by Emualtor
            
            //CIA at top of space
            switch(direction){
                case m68kWrite: WriteCIA(address, value); return 0;
                case m68kRead: return RAM24bit[address];
            }
            return 0;
            break;
            
        case 6:// ChipRegs
            

            //Slowram
            if(address < 0xD80000){ // 0xD80000)

                //We don't want slow RAM so the code below decodes the addesses as though there is no slow ram
                switch(direction){
                       
                    case m68kRead:
                       // printf("SlowRAM Read:0x%x\n",address);
                        address =  0xDFF000 | (address & 511);
                        uint16_t v1 =  BigEndianRead(address, size);
                        return v1;
                    case m68kWrite:
                        if((address & 0x1FF) == 0x9A){
                         //   printf("SlowRAM Write:0x%x\n",address);
                            WriteChipsetWord(address, value);
                        }
                        return 0;
                }
              
            }
            
            switch(direction){
                    
                case m68kRead:
                    //if(address == 0xDFF00A){
                    //    printf("Joy Read");
                    //}
                    return BigEndianRead(address, size);
                                        
                case m68kWrite:
                    
                    switch(size){
                        case m68kByte:
                            WriteChipsetByte(address, value);
                            return 0;
                        case m68kWord:
                            WriteChipsetWord(address, value);
                            return 0;
                        case m68kLong:
                            WriteChipsetLong(address, value);
                            return 0;
                    }
            }
            
            return 0;
            break;
        case 7:// ROM
            return BigEndianRead(address, size);
            break;
    }

    return 0;
    
}


unsigned int RAM24BitDespatchEXP(uint32_t address, enum DataSize size,enum DataDirection direction, unsigned int value){
    
   // m68k_end_timeslice();
    
    /*
    if(address ==0xDFF084){
        printf("Trap!!");
    }
    */
    
    
    if(address == 0){
        
        uint32_t* i = (uint32_t*)RAM24bit;
        
        if(value == 0x48454c50){
            ZeroLong = value;
            //printf("Error: %d\n",i);
        }else{
            ZeroLong = 0;
        }
        
 
        //printf("Zero? %c | %d\n",value,i);
    }
    
    
    if(ZeroLong != 0 && address == 0x100){
        printf("Error Code: %x",value);
        printf("\n");
    }
    
    
    if(address < 0x200000){
        //printf("ChipRAM\n");
        
        //256k RAM test
        //address = address & 0x3FFFF;
        
        //512K RAM test
        //address = address & 0x7FFFF;
        
        
        switch(direction){
            case m68kWrite: BigEndianWrite(address, size, value); return 0;
            case m68kRead: return BigEndianRead(address, size);
        }
        

    }
    
    
    if(address < 0xA00000){
        
        //2Meg Chipram
        /*
        address = address & 0x1FFFFF;
        
        
        switch(direction){
            case m68kWrite: BigEndianWrite(address, size, value); return 0;
            case m68kRead: return BigEndianRead(address, size);
        }
        */
        
        //printf("ZII FastRAM\n");
        return 0;
    }
    
    
    if(address < 0xBF0000){
        //printf("Reserved Space\n");
        return 0;
    }
    
    if(address < 0xC00000){
        //printf("CIA\n");
        
        //printf("CIA Write: %x - %d\n",address,value);
        
        switch(direction){
            case m68kWrite: WriteCIA(address, value); return 0;
            case m68kRead: return RAM24bit[address];
        }
    }
    


    

    if(address < 0xD80000){ // 0xD80000)
       
        /*
        //This looks like 1Mb of Slow RAM
        switch(direction){
            case m68kWrite:
                if(address == 0xD7F09A){
                    WriteChipsetWord(address, value);
                    return 0;
                }
                //printf("SlowRAM Write:0x%x\n",address);
                BigEndianWrite(address, size, value);
                return 0;
            case m68kRead:
                if(address == 0xD7F01C){
                    address =  0xDFF000 | (address & 511);
                    uint16_t v1 =  BigEndianRead(address, size);
                    return v1;
                }
                //printf("SlowRAM Read:0x%x\n",address);
                return BigEndianRead(address, size);
        }
        return 0;
        */
        
        //We don't want slow RAM so the code below decodes the addesses as though there is no slow ram
        switch(direction){
               
            case m68kRead:
               // printf("SlowRAM Read:0x%x\n",address);
                address =  0xDFF000 | (address & 511);
                uint16_t v1 =  BigEndianRead(address, size);
                return v1;
            case m68kWrite:
                if((address & 0x1FF) == 0x9A){
                    //printf("SlowRAM Write:0x%x\n",address);
                    WriteChipsetWord(address, value);
                }
                return 0;
        }
      
    }

    
    if(address < 0xDA0000){
        printf("Also Reserved\n");
        uint32_t PC = m68k_get_reg(NULL, M68K_REG_PC);
        diss = 1;
        return 0;
    }
    
    
    if(address < 0xDC0000){
        printf("IDE: %d\n",value);
        return 0;
        /*
         0xda0000    // Data
         0xda0006    // Error | Feature
         0xda000a    // Sector Count
         0xda000e    // Sector Number
         0xda0012    // Cylinder Low
         0xda0016    // Cylinder High
         0xda001a    // Device / Head
         0xda001e    // Status | Command
         0xda101a    // Control
         0xda8000    // Gayle Status
         0xda9000    // Gayle INTREQ
         0xdaa000    // Gayle INTENA
         0xdab000    // Gayle Config
         
         The "Gayle-check" is:

         write 0x00 to 0xde1000
         read byte 0xde1000 with bit 7 set
         read byte 0xde1000 with bit 7 set
         read byte 0xde1000 with bit 7 cleard
         read byte 0xde1000 with bit 7 set

         Then the Kickstart accepts Gayle and accesses HD. Works even with 37.300 (A600).
         */
        

    }
    
    if(address < 0xDD0000){
        printf("RTC\n");
        return 0;
    }
 
    if(address < 0xDE0000){
        printf("Reserved\n");
        return 0;
        /*
        The "Gayle-check" is:

        write 0x00 to 0xde1000
        read byte 0xde1000 with bit 7 set
        read byte 0xde1000 with bit 7 set
        read byte 0xde1000 with bit 7 cleard
        read byte 0xde1000 with bit 7 set

        Then the Kickstart accepts Gayle and accesses HD. Works even with 37.300 (A600).
         */
        
        /*
         
         0xda900 Gayle INTREQ:
         0x80 IDE
         0X02 IDE1ACK (Slave)
         0x01 IDE0ACK (Master)

         If a Interrupt (Level 2) occurs and it is caused by an IDE Device Gayle INTREQ IDE bit 7 is set. I'm not sure if, the corresponding IDExACK will be set. When done with interrupt handling these bits will be set to 0 by the device driver.

         0xdaa000 Gayle INTENA:
         0x80 IDE

         Setting bit 7 of Gayle INTENA enables ATA Interrupts.


         So your trace reads:
         > p00fb6b80t00daa000 1w 80808080
         Enable IDE Interrupts
         .
         .
         .
         > p00fb70d4t00da2018 1w a0a0a0a0
         Select device 0
         > p00fb70d8t00da2010 1r 34ff34ff
         Device's Cylinder Low register is 0x34. (Should have been 0 after reset)
         > P00fbcf98T00da9000 1r 7fffffff
         > P00fbcfa4T00daa000 1r 00000000
         We're here in card.resource's irq handler. Which checks for a PCMCIA Int.


         As usual for Amiga adresses are not fully decoded. Kickstart uses the following adresses for IDE. The above mentioned adresses are used by Linux. If you want the project to be compatible with Linux you should implement a similiar incomplete decoding.

         0xda2000 Data
         0xda2004 Error | Feature
         0xda2008 SectorCount
         0xda200c SectorNumber
         0xda2010 CylinderLow
         0xda2014 CylinderHigh
         0xda2018 Device/Head
         0xda201c Status | Command
         0xda3018 Control


         0xde1000's MSB should actually be interpreted as a 8 bit serial shift register, which reads 0xd0. Fat Garys (A3000,A4000) have the very same mechanism at 0xde1002 this register is called GaryID (see: http://www.thule.no/haynie/research/...ocs/a3000p.pdf).
         
         
         */
    }
    
    if(address < 0xDF0000){
        printf("Mainboard Resources.. whatever they are?\n");
        return 0;
    }
    
    if(address < 0xE00000){
       // printf("Chipset Registers\n");

        uint16_t debugReturn;
        
        switch(direction){
                
            case m68kRead: debugReturn = BigEndianRead(address, size);
                
                switch(size){
                    case m68kByte:
                        //diss = 0;
                        //printf("Chipset byte Read @ %s \t (0x%x) : %d\n",regNames[(address & 0x1FF)/2],address,debugReturn);
                       //printf("VHPOSR: 0x%x",BigEndianRead(address, m68kWord));
                       // printf("\n");
                        break;
                    case m68kWord:
                      //  printf("Chipset Word Read @ %s \t (0x%x) : %d\n",regNames[(address & 0x1FF)/2],address,debugReturn);
                        break;
                    case m68kLong:
                     //   printf("Chipset Long Read @ %s \t (0x%x) : %d\n",regNames[(address & 0x1FF)/2],address,debugReturn);
                        break;
                }
                return debugReturn;
                
            case m68kWrite:
                switch(size){
                    case m68kByte:
                        WriteChipsetByte(address, value);
                     //   printf("Chipset byte write @ %s \t (0x%x) : %d\n",regNames[(address & 0x1FF)/2],address, value);
                        return 0;
                    case m68kWord:
                        WriteChipsetWord(address, value);
                     //   printf("Chipset word write @ %s \t (0x%x) : %d\n",regNames[(address & 0x1FF)/2],address, value);
                        return 0;
                    case m68kLong:
                        WriteChipsetLong(address, value);
                     //   printf("Chipset long write @ %s \t (0x%x) : %d\n",regNames[(address & 0x1FF)/2],address, value);
                        return 0;
                }
        }
        
    }
    
    if(address < 0xE80000){
        printf("Reserved!\n");
        return 0;
    }
    
    if(address < 0xF00000){
        //printf("Autoconfig!\n");
        return 0;
    }
    
    
    if(address < 16777216){
        uint32_t rv = BigEndianRead(address, size);
        return rv;
    }
        
        
    //32bit address space... just ignore for now :-)
    switch(size){
        case m68kByte:
            //*(uint8_t*)address = (uint8_t)value;
            return 0;
        case m68kWord:
           // *(uint16_t*)address = (uint16_t)value;
            return 0;
        case m68kLong:
           // *(uint32_t*)address = (uint32_t)value;
            return 0;
    }
    
    
}




unsigned int cpu_read_byte(unsigned int address){
     
    //Two special cases for the CIA ICRs which need to be cleared on read
    if(address == 0xBFED01){
        uint8_t p = RAM24bit[address];
        WRITEBYTE(address, 0);
        return p;
    }
    
    if(address == 0xBFDD00){
        uint8_t p = RAM24bit[address];
        RAM24bit[address] = 0;
        return p;
    }
    
    return RAM24BitDespatch(address, m68kByte, m68kRead, 0);
}

unsigned int cpu_read_word(unsigned int address){
    uint16_t retVal = RAM24BitDespatch(address, m68kWord, m68kRead, 0);
    return retVal;
}

unsigned int cpu_read_long(unsigned int address){
    uint32_t retVal = RAM24BitDespatch(address, m68kLong, m68kRead, 0);
    return retVal;
}


void cpu_write_byte(unsigned int address, unsigned int value){
    RAM24BitDespatch(address, m68kByte, m68kWrite, value);
}

void cpu_write_word(unsigned int address, unsigned int value){
    RAM24BitDespatch(address,m68kWord, m68kWrite, value);
}

void cpu_write_long(unsigned int address, unsigned int value){
    RAM24BitDespatch(address, m68kLong, m68kWrite, value);
}


void cpu_pulse_reset(void){

    m68k_set_reg(M68K_REG_PC,4);
    m68k_set_reg(M68K_REG_D0,0);
    m68k_set_reg(M68K_REG_D1,0);
    m68k_set_reg(M68K_REG_D2,0);
    m68k_set_reg(M68K_REG_D3,0);
    m68k_set_reg(M68K_REG_D4,0);
    m68k_set_reg(M68K_REG_D5,0);
    m68k_set_reg(M68K_REG_D6,0);
    m68k_set_reg(M68K_REG_D7,0);
    m68k_set_reg(M68K_REG_A0,0);
    m68k_set_reg(M68K_REG_A1,0);
    m68k_set_reg(M68K_REG_A2,0);
    m68k_set_reg(M68K_REG_A3,0);
    m68k_set_reg(M68K_REG_A4,0);
    m68k_set_reg(M68K_REG_A5,0);
    m68k_set_reg(M68K_REG_A6,0);
    m68k_set_reg(M68K_REG_A7,0);
    
    //Set starting PC. The first thing the ROM does is set the Stack pointer so we don't have to.
    BigEndianWrite(0, m68kLong, 0);  // No stack pointer
    BigEndianWrite(4, m68kLong, 0xF80002);  // Start Execution at 0xF80002
    FloppyReset();
}

void cpu_set_fc(unsigned int fc){
    
}

int  cpu_irq_ack(int level){
    return 0;
}


unsigned int cpu_read_word_dasm(unsigned int address){
    return cpu_read_word(address);
}

unsigned int cpu_read_long_dasm(unsigned int address){
    return cpu_read_long(address);
}


Omega_t* InitRAM(int RAM32bitSize){
    
    RAM24bit = (uint8_t*)&omega;
    
    if(RAM24bit == NULL){
        printf("RAM Alloc Fail\n");
    }
    
    //Check memory
    for(int i = 0;i<sizeof(Omega_t); ++i){
        uint8_t val = i % 256;
        RAM24bit[i] = val;
    }
    
    //Ensure RAM is clear
    for(int i = 0;i<sizeof(Omega_t); ++i){
        uint8_t val = i % 256;
        
        if( RAM24bit[i] == val){
            RAM24bit[i] = 0x00;     // Why not fill with Junk?
        }else{
            printf("RAM ERROR!!!");
        }
    }
    
    //uint8_t* cr = malloc(sizeof(Chipset_t));
    //InitChipset(omega.chipRAM,cr);       // Chipset internals
    //uint8_t* ci = malloc(sizeof(CIA_t));
    //InitCIA(omega.Chipstate, ci);         // CIA Pair
    
    
    InitChipset(omega.chipRAM,omega.Chipstate);       // Chipset internals can have 512Kb (Needs chipram address as it can access chipram)
    InitCIA(omega.Chipstate, omega.CIAState);         // CIA Pair can have 512Kb (Needs Chipstate so it can call interrupts)

    //Clear the AutoConfig space
    for(int i=0xE80000;i<0xF80000;++i){
        RAM24bit[i] = 0x00;
    }
    
    //Fill chipram with junk for testing purpose
    for(int i=0;i<0x40000;++i){
        RAM24bit[i] = 0xFF;
    }
    for(int i=0x40000;i<0x200000;++i){
        RAM24bit[i] = 0x84;
    }
    
    printf("KickSize: %lu\n", sizeof(kick12));
    
    //copy ROM image into 24bit address space
    for(int i = 0;i<sizeof(kick12); ++i){
        RAM24bit[0xF80000 + i] = kick12[i];
    }
    
    
    cpu_pulse_reset();
    
    //Set RTC value...
    RAM24bit[0xDC0000] = 0x0;
    RAM24bit[0xDC0001] = 0xFF;
    RAM24bit[0xDC0002] = 0xFF;
    RAM24bit[0xDC0003] = 0x00;
    RAM24bit[0xDC0004] = 0xFF;
    RAM24bit[0xDC0005] = 0xFF;
    RAM24bit[0xDC0006] = 0xFF;
    RAM24bit[0xDC0007] = 0xFF;
    RAM24bit[0xDC0008] = 0xFF;
    RAM24bit[0xDC0009] = 0xFF;
    RAM24bit[0xDC000A] = 0xFF;
    RAM24bit[0xDC000B] = 0xFF;
    RAM24bit[0xDC000C] = 0xFF;
    RAM24bit[0xDC000D] = 0xFF;
    RAM24bit[0xDC000E] = 0xFF;
    RAM24bit[0xDC000F] = 0xFF;

    
    return &omega;
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

void disassemble_program(void){
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


typedef struct{
    uint32_t pc;
    uint32_t d0;
    uint32_t d1;
    uint32_t d2;
    uint32_t d3;
    uint32_t d4;
    uint32_t d5;
    uint32_t d6;
    uint32_t d7;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t a4;
    uint32_t a5;
    uint32_t a6;
    uint32_t a7;
    Chipset_t* chipset;
}contextI;


contextI CIII;





void cpu_instr_callback(void){
    
    int* bum = &diss;
    
    CIII.pc = m68k_get_reg(NULL, M68K_REG_PC);
    CIII.d0 = m68k_get_reg(NULL, M68K_REG_D0);
    CIII.d1 = m68k_get_reg(NULL, M68K_REG_D1);
    CIII.d2 = m68k_get_reg(NULL, M68K_REG_D2);
    CIII.d3 = m68k_get_reg(NULL, M68K_REG_D3);
    CIII.d4 = m68k_get_reg(NULL, M68K_REG_D4);
    CIII.d5 = m68k_get_reg(NULL, M68K_REG_D5);
    CIII.d6 = m68k_get_reg(NULL, M68K_REG_D6);
    CIII.d7 = m68k_get_reg(NULL, M68K_REG_D7);
    CIII.a0 = m68k_get_reg(NULL, M68K_REG_A0);
    CIII.a1 = m68k_get_reg(NULL, M68K_REG_A1);
    CIII.a2 = m68k_get_reg(NULL, M68K_REG_A2);
    CIII.a3 = m68k_get_reg(NULL, M68K_REG_A3);
    CIII.a4 = m68k_get_reg(NULL, M68K_REG_A4);
    CIII.a5 = m68k_get_reg(NULL, M68K_REG_A5);
    CIII.a6 = m68k_get_reg(NULL, M68K_REG_A6);
    CIII.a7 = m68k_get_reg(NULL, M68K_REG_A7);
    CIII.chipset = (Chipset_t*)&RAM24bit[0xDFF000];
    
    if(diss==0){
        return;
    }else{
        diss += 0;
    }
    CSWindow_t* window = (CSWindow_t*)&RAM24bit[0xDFF000];
    
    // The following code will print out instructions as they are executed
    static char buff[100];
    static char buff2[100];
    static unsigned int pc;
    static unsigned int instr_size;
    
    pc = m68k_get_reg(NULL, M68K_REG_PC);
    instr_size = m68k_disassemble(buff, pc, M68K_CPU_TYPE_68000);
    
    if(pc == 0xf80e64){
        diss = 1;
    }

    make_hex(buff2, pc, instr_size);
        
    //fprintf(globalFD,"E %03x: %-20s: %s\n", pc, buff2, buff);
    printf("E %03x: %-20s: %s\n", pc, buff2, buff);
    fflush(stdout);
    //SDL_Delay(75);

    
    
    
}



void printCPUContext(void){
    return;
    printf("D0: 0x%x\n",m68k_get_reg(NULL, M68K_REG_D0));
    printf("D1: 0x%x\n",m68k_get_reg(NULL, M68K_REG_D1));
    printf("D2: 0x%x\n",m68k_get_reg(NULL, M68K_REG_D2));
    printf("D3: 0x%x\n",m68k_get_reg(NULL, M68K_REG_D3));
    printf("D4: 0x%x\n",m68k_get_reg(NULL, M68K_REG_D4));
    printf("D5: 0x%x\n",m68k_get_reg(NULL, M68K_REG_D5));
    printf("D6: 0x%x\n",m68k_get_reg(NULL, M68K_REG_D6));
    printf("D7: 0x%x\n",m68k_get_reg(NULL, M68K_REG_D7));
    printf("A0: 0x%x\n",m68k_get_reg(NULL, M68K_REG_A0));
    printf("A1: 0x%x\n",m68k_get_reg(NULL, M68K_REG_A1));
    printf("A2: 0x%x\n",m68k_get_reg(NULL, M68K_REG_A2));
    printf("A3: 0x%x\n",m68k_get_reg(NULL, M68K_REG_A3));
    printf("A4: 0x%x\n",m68k_get_reg(NULL, M68K_REG_A4));
    printf("A5: 0x%x\n",m68k_get_reg(NULL, M68K_REG_A5));
    printf("A6: 0x%x\n",m68k_get_reg(NULL, M68K_REG_A6));
    printf("A7: 0x%x\n",m68k_get_reg(NULL, M68K_REG_A7));
}
