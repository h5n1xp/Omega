//
//  CIA.c
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

#include "CIA.h"
#include "m68k.h"
#include "Floppy.h"

CIA_t CIAA;
CIA_t CIAB;

void CIAInit(CIA_t* cia,uint16_t paulaMask){
    cia->ta  = 0;//65535;
    cia->tb  = 0;//65535;
    cia->taLatch = 0;
    cia->tbLatch = 0;
    cia->tod = 0;
    cia->todAlarm = 0;
    cia->chipInt = paulaMask;
    
    cia->icr = 0;
    
    //cia->cra = 1;   //may as well have them running at boot....
    //cia->crb = 1;
    
    cia->Name[0] ='B';
    cia->Name[1] = 0;
    
    //set CIAB ports
    cia->prb = 255;//All lines active low

    
    //set up CIAA ports
    if(paulaMask == 32776){
        cia->Name[0] ='A';
        cia->pra = 55;   //Pretend the ROM memory overlay is active (Powerlight is dim and floppy drive is not ready, and disk inseted is true).
    }
}

void CIAWrite(CIA_t* cia,int reg,uint8_t value){
    
    m68k_end_timeslice();
    
    switch(reg){
            
        case 0x0:cia->pra = value & 63;break;//pra
        case 0x1://prb - Due to the floppy drive's dependance upon this register, it only flags a change if the write actually changed the value.
            /*
            if(value != cia->prb){
                cia->prbw = value;
                cia->prbChanged=1;
            }
             */
            cia->prb = value;
            floppySetState();
            break;
        case 0x2:cia->ddra = value;break;//ddra
        case 0x3:cia->ddrb = value;break;//ddrb
        case 0x4:cia->taLatch = (cia->taLatch & 65280) | value;break;//talo
        case 0x5://tahi
            cia->taLatch = (cia->taLatch & 255)   | (value<<8);
            
            if( (cia->cra & 8) == 8){ //in one shot mode a write here starts the timer
                 cia->ta  = cia->taLatch;
                 cia->cra = cia->cra | 1;
            }
            
            break;
        case 0x6:cia->tbLatch = (cia->tbLatch & 65280) | value;break;//tblo
        case 0x7://tbhi
            cia->tbLatch = (cia->tbLatch & 255)   | (value<<8);
            
            if( (cia->crb & 8) == 8){ //in one shot mode a write here starts the timer
                cia->tb  = cia->tbLatch;
                cia->crb = cia->crb | 1;
            }
            
            break;
        case 0x8: //todlo
            if( (cia->crb & 128) ==0){
                cia->tod = (cia->tod & 16776960) | value;
            }else{
                cia->todAlarm = (cia->todAlarm & 16776960) | value;
            }
            break;
        case 0x9: //todmid
            if( (cia->crb & 128) ==0){
                cia->tod = (cia->tod & 16711935) | (value << 8);
            }else{
                cia->todAlarm = (cia->todAlarm & 16711935) | value << 8;
            }
            break;
        case 0xA: //todhi
            if( (cia->crb & 128) ==0){
                cia->tod = (cia->tod & 65535) | (value << 16);
            }else{
                cia->todAlarm = (cia->todAlarm & 65535) | value << 16;
            }
            break;
        case 0xB: //ICR WRITE - Internal use only
            if( (value & 128) ==0){
                cia->icr = cia->icr ^ (cia->icr & value);
            }else{
                cia->icr = cia->icr | (value  & 127);
            }
            break;
        case 0xC:cia->sdr = value;break;//sdr
        case 0xD://icr
            if( (value & 128) ==0){
                cia->icrMask = cia->icrMask ^ (cia->icrMask & value);
            }else{
                cia->icrMask = cia->icrMask | (value  & 127);
            }
            break;
        case 0xE:cia->cra = value; break;//cra
        case 0xF:cia->crb = value; break;//crb
            
    }
}

uint8_t CIARead(CIA_t* cia,int reg){
    
    m68k_end_timeslice();
    
    uint8_t value=0;
    
    switch(reg){   
            
        case 0x0:
            floppyState();
            value = cia->pra;break;//pra
        case 0x1:value = cia->prb;break;//prb
        case 0x2:value = cia->ddra;break;//ddra
        case 0x3:value = cia->ddrb;break;//ddrb
        case 0x4:value = cia->ta & 255;break;//talo
        case 0x5:value =(cia->ta >> 8) & 255;break;//tahi
        case 0x6:value = cia->tb & 255;break;//tblo
        case 0x7:value =(cia->tb >> 8) & 255;break;//tbhi
        case 0x8:value = cia->tod & 255;break;//todlo
        case 0x9:value =(cia->tod >> 8 ) & 255;break;//todmid
        case 0xA:value =(cia->tod >> 16) & 255;break;//todhi
        case 0xB:value =(cia->tod >> 24) & 255;break;//todunused
        case 0xC:value = cia->sdr; cia->sdr = 0;break;//sdr - clear after read.
        case 0xD://icr
            value = cia->icr;
            cia->icr = 0;
            cia->irqLatch = 0;
            break;
        case 0xE:value = cia->cra;break;//cra
        case 0xF:value = cia->crb;break;//crb
            
    }
    
    return value;
}


void CIAExecute(CIA_t* cia){
    
    int taUnderflow=0;
    
    //Force load timer and clear load bit
    if( (cia->cra & 16) == 16){
        cia->ta =cia->taLatch;
        cia->cra = cia->cra & 239;
    }
    
    //Force load timer and clear load bit
    if( (cia->crb & 16) == 16){
        cia->tb =cia->tbLatch;
        cia->crb = cia->crb & 239;
    }
    
    //TimerA - Normal
    if((cia->cra & 33) == 1){
        cia->ta -=1;
        
        //underflow
        if(cia->ta == -1){
            cia->ta = cia->taLatch;
            
            if( (cia->cra & 8) == 8){ //in oneshot mode clear the start bit.
                 cia->cra = cia->cra & 254;
            }
            
            cia->icr =  cia->icr | (1 & cia->icrMask);
            taUnderflow=1;
        }
    }
    
    
    //TimerB - Normal
    if((cia->crb & 97) == 1){
        cia->tb -=1;
        
        //underflow
        if(cia->tb == -1){
            cia->tb = cia->tbLatch;
            
            if( (cia->crb & 8) == 8){ //in oneshot mode clear the start bit.
                 cia->crb = cia->crb & 254;
            }
            
            cia->icr =  cia->icr | (2 & cia->icrMask);
        }
    }
    
    //Timer B - TA Underflow
    if( (cia->crb & 97) == 65 && taUnderflow==1 ){
        cia->tb -=1;
        
        //underflow
        if(cia->tb == 65535){
            cia->tb = cia->tbLatch;
            
            if( (cia->crb & 8) == 8){ //in oneshot mode clear the start bit.
                 cia->crb = cia->crb & 254;
            }
            
            cia->icr =  cia->icr | (2 & cia->icrMask);
        }
    }
    
    //interupt occured
    if( cia->icr > 0 && cia->irqLatch == 0 ){
        putChipReg16[INTREQ](cia->chipInt);
        cia->irqLatch = 1;
        //printf("CIA %s int %d\n",cia->Name,cia->icr);
    }
    
}

void CIATODEvent(CIA_t* cia){
    cia->tod +=1;
    
    if(cia->tod ==16777216){
        cia->tod = 0;
    }
    
    if(cia->todAlarm !=0){
        if(cia->todAlarm == cia->tod){
            cia->icr =  cia->icr | (4 & cia->icrMask);
        }
    }
}

void CIAIndex(CIA_t* cia){
    
    cia->icr =  cia->icr | (0x90 & cia->icrMask);
}

void keyboardInt(){
        CIAA.icr =  CIAA.icr | (0x8 & CIAA.icrMask); //raise Serial port interrupt flag on CIA A
}
