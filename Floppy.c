//
//  Floppy.c
//  The Omega Project
//  https://github.com/h5n1xp/Omega
//
//  Created by Matt Parsons on 02/02/2019.
//  Copyright © 2019 Matt Parsons. All rights reserved.
//  <h5n1xp@gmail.com>
//
//  Clocking and Checksum Code contributed by Dirk Hoffmann
//
//  This Source Code Form is subject to the terms of the
//  Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "Floppy.h"
#include "Chipset.h"
#include "Memory.h"

#include <unistd.h>
#include <fcntl.h>


void encodeBlock(uint8_t* source, uint8_t* destination,int size){
    
    for(int i=0;i<size;i++){
        destination[i] = (source[i] >> 1) & 0x55;
        destination[i+size] = (source[i]) & 0x55;
    }
    
}

uint8_t addClockBits(uint8_t previous, uint8_t value) {
    // Clear all previously set clock bits
    value &= 0x55;
    
    // Compute clock bits (clock bit values are inverted)
    uint8_t lShifted = (value << 1);
    uint8_t rShifted = (value >> 1) | (previous << 7);
    uint8_t cBitsInv = lShifted | rShifted;
    
    // Reverse the computed clock bits
    uint64_t cBits = cBitsInv ^ 0xAA;
    
    // Return original value with the clock bits added
    return value | cBits;
}

void ADF2MFM(int fd, uint8_t* mfm){
    
    int size =(int) lseek(fd, 0, SEEK_END);
    
    //printf("FloppySize: %d\n",size);
    
    //512 bytes per sector
    int sectors = size/512;
    
    //22 sectors per track (each side 11 sectors)
    int tracks = (sectors / 22);
    
    //each track is 12798 bytes in size, multiplied by 2 because there are 2 sides
    //int mfmSize = tracks * (12798 * 2);
    
    uint8_t adf[size];
    // uint8_t mfm[mfmSize];
    
    uint8_t lowlevelSector[544]; //bytes per low level sector
    
    lseek(fd, 0, SEEK_SET);
    read(fd, adf, size);
    
    
    int count = 0;
    int s = 0;
    
    for(int track = 0;track<tracks;track++){
        for(int side=0;side<2;side++){
            for(int sector=0;sector<11;sector++){
                
                //int secCountDown = 11 - sector;
                
                
                //printf("%d: Track %d, Side: %d, Sector %d (%d)\n",s,track,side,sector,secCountDown);
                
                //Build the sector
                lowlevelSector[0] = 0x0;
                lowlevelSector[1] = 0x0;
                
                lowlevelSector[2] = 0xA1; // will be a sync mark
                lowlevelSector[3] = 0xA1; // will be a sync mark
                
                //sector info
                
                lowlevelSector[4] = 0xFF;
                lowlevelSector[5] = track << 1 | side;
                lowlevelSector[6] = sector;
                lowlevelSector[7] = 11 - sector;
                
                //Sector label
                lowlevelSector[8]  = 0x0;
                lowlevelSector[9]  = 0x0;
                lowlevelSector[10] = 0x0;
                lowlevelSector[11] = 0x0;
                
                lowlevelSector[12] = 0x0;
                lowlevelSector[13] = 0x0;
                lowlevelSector[14] = 0x0;
                lowlevelSector[15] = 0x0;
                
                lowlevelSector[16] = 0x0;
                lowlevelSector[17] = 0x0;
                lowlevelSector[18] = 0x0;
                lowlevelSector[19] = 0x0;
                
                lowlevelSector[20] = 0x0;
                lowlevelSector[21] = 0x0;
                lowlevelSector[22] = 0x0;
                lowlevelSector[23] = 0x0;
                
                
                //data
                for(int i=0;i<512;++i){
                    lowlevelSector[32+i] = adf[i+(count*512)];
                    
                }
                
                
                
                //***************
                
                //Encode
                
                mfm[s+0] = 0xAA;
                mfm[s+1] = 0xAA;
                mfm[s+2] = 0xAA;
                mfm[s+3] = 0xAA;
                
                mfm[s+4] = 0x44;
                mfm[s+5] = 0x89;
                mfm[s+6] = 0x44;
                mfm[s+7] = 0x89;
                
                //info
                encodeBlock(&lowlevelSector[4], &mfm[s+8], 4); // adds 8 bytes
                
                //Disklabel
                encodeBlock(&lowlevelSector[8], &mfm[s+16], 16);//adds 32 bytes
                
                //Data section
                encodeBlock(&lowlevelSector[32], &mfm[s+64], 512);
                
                
                //Header checksum
                uint8_t hcheck[4] = { 0, 0, 0, 0 };
                for(unsigned i = 8; i < 48; i += 4) {
                    hcheck[0] ^= mfm[s+i];
                    hcheck[1] ^= mfm[s+i+1];
                    hcheck[2] ^= mfm[s+i+2];
                    hcheck[3] ^= mfm[s+i+3];
                }
                
                lowlevelSector[24] = hcheck[0];
                lowlevelSector[25] = hcheck[1];
                lowlevelSector[26] = hcheck[2];
                lowlevelSector[27] = hcheck[3];
                
                //header checksum
                encodeBlock(&lowlevelSector[24], &mfm[s+48], 4); //adds 8 bytes
                
                
                // Data checksum
                uint8_t dcheck[4] = { 0, 0, 0, 0 };
                for(unsigned i = 64; i < 1088; i += 4) {
                    dcheck[0] ^= mfm[s+i];
                    dcheck[1] ^= mfm[s+i+1];
                    dcheck[2] ^= mfm[s+i+2];
                    dcheck[3] ^= mfm[s+i+3];
                }
                
                lowlevelSector[28] = dcheck[0];
                lowlevelSector[29] = dcheck[1];
                lowlevelSector[30] = dcheck[2];
                lowlevelSector[31] = dcheck[3];
                
                //Encode Data checksum
                encodeBlock(&lowlevelSector[28], &mfm[s+56], 4); //adds 8 bytes
                
                
                
                //Add clocking bits
                for(int i=8;i<1088;i++){
                    uint8_t previous = mfm[s+i-1];
                    
                    mfm[s+i] = addClockBits(previous,mfm[s+i]);
                    //mfm[s+i] = clocking(previous, mfm[s+i]);
                    
                }
                
                s += 1088;    //Why not 1088, which is the size of the data we've produced
                count +=1;
                
                
            }
            
            //Add clocking bits to the track gap
            mfm[s]   = addClockBits(mfm[s-1],0);
            mfm[s+1] = 0xA8;
            mfm[s+2] = 0x55;
            mfm[s+3] = 0x55;
            mfm[s+4] = 0xAA;
            
            for(int i=5;i<700;i++){
                uint8_t previous = mfm[s+i-1];
                
                mfm[s+i] = addClockBits(previous,0);
                //mfm[s+i] = clocking(previous, mfm[s+i]);
                
            }
            
            s += 830;   //pad track to make 12798 bytes to meet the ADF-EXT spec.
            //printf("\n");
        }
        
    }
    //printf("loaded");
    
}
//*******************************




int driveSelected=0;
Fd_t df[4];

uint8_t floppyDataRead(){ //this function should be called by the DMA
    
    //Fd_t* ourDisk = &df[driveSelected];
    
    //int surface = 1 - disk->diskSide;
    //int position = (disk->diskTrack * (12798*2)) + (surface * 12798) + (disk->index);
    int position   = (df[driveSelected].track * (12798 * 2)) + (df[driveSelected].side  * 12798) + df[driveSelected].index;
    
    
    //disk->index +=1;
    df[driveSelected].index +=1;
    /*
    if(disk->index>12667){ //ADF track has 12798 bytes, but on a normal AmigaOS disk 12668 are used.
        disk->index = 0;
        CIAIndex(&CIAB);    // generate CIAB index interupt
    }
    */
    if(df[driveSelected].index>12667){ //ADF track has 12798 bytes, but on a normal AmigaOS disk 12668 are used.
        df[driveSelected].index= 0;
        CIAIndex(&CIAB);    // generate CIAB index interupt
    }
    
    //uint8_t retVal = disk->mfmData[position];
    uint8_t retVal = df[driveSelected].mfmData[position];
    
    return retVal;
}


void floppyInsert(int drive){

    if(df[drive].idMode !=0){
        //Only Vaild drives have an ID Mode == 0 
        return;
    }
    df[drive].pra |= 0x4;
    
    
}

uint8_t* floppyInit(int drive){
    df[drive].idMode = -1;
    df[drive].index = 0;
    df[drive].track = 0;
    df[drive].side = 0;
    df[driveSelected].pra  &= 0xFB;      // no disk;
    return df[drive].mfmData;
}

void floppyState(){
    CIAA.pra |= df[driveSelected].pra & 0x3C;
}

void floppySetState(){            //To be called when Writes to CIAB prb happen.
    
    static uint8_t PRB;
    static int count = 0;
    
    // No point doing anything if nothing has changed.
    if(PRB == CIAB.prb){
        PRB = CIAB.prb;
        return;
    }

    PRB = CIAB.prb;
    
    switch (PRB & 0x78) {
        case 0x78:
            CIAA.pra &=0xC3;    //Need to take the Drives off the floppy bus.
            return;
            break;
            
        case 0x70:
            driveSelected = 0;
            break;

        case 0x68:
            driveSelected = 1;
            //return;
            break;
            
        case 0x58:
            driveSelected = 2;
            //return;
            break;
            
        case 0x38:
            driveSelected = 3;
            //return;
            break;
            
        default:
            break;
    }
    //count++;

    //printf("%d: ",count);
    
    //ID mode... to identify exteernal drives...
     if(df[driveSelected].idMode>0){   // Id mode
         //printf("DF%d ID Mode: %d\n",driveSelected,df[driveSelected].idMode); // The Disk Ready sitnal is pusled 32 times to signle a drive is present on the bus
         df[driveSelected].pra  &= 0xDF;     //Drive ready flag signals the drive is there
         df[driveSelected].idMode -=1;
         CIAA.pra |= df[driveSelected].pra & 0x3C;
         return;
     }
    
    // If no change in state just
    if(PRB == df[driveSelected].prb){
        //printf("---\n");
        df[driveSelected].prb = PRB;
        return;
    }
    

    df[driveSelected].prb = PRB;
    

    //printf("DF%d - ",driveSelected);
    
    if(PRB & 0x80){
        //printf(" Motor Off ");
        //df[driveSelected].prb  |= 0x80;
        
        if(df[driveSelected].idMode==-1){   //is this the first time the motor has been turned off?
            df[driveSelected].idMode = 32;  //if so activate ID Mode
        }
        
        df[driveSelected].pra  |= 0x20;     //Drive not ready
        
    }else{
        //printf(" Motor On ");
        //df[driveSelected].prb  &= 0x7F;
        
        if(df[driveSelected].pra & 0x4){     // disk inserted
            df[driveSelected].pra  &= 0xDF;  // Drive ready
        }
        
    }
    
    
    if(PRB & 0x4){
        df[driveSelected].side = 0;
        //df[driveSelected].prb |= 0x4;
        //printf(" -upper surface.");
        
    }else{
        df[driveSelected].side = 1;
        //df[driveSelected].prb &= 0xFB;
        //printf(" -lower surface.");
    }

    
    //Step head
    if(PRB & 0x1){
        
        
        if(PRB & 0x2){
            df[driveSelected].track -=1;
            //printf(" Head Stepped back ");
        }else{
            df[driveSelected].track +=1;
           // printf(" Head Stepped forward ");
        }
        
        if(df[driveSelected].track < 0){
            df[driveSelected].track = 0;
        }
        
        if(df[driveSelected].track==0){
            df[driveSelected].pra  &= 0xEF; //Track 0 reached
        }else{
            df[driveSelected].pra  |= 0X10; // not track 0;
        }
        
        //printf(" to track %d|",df[driveSelected].track);
        
    }
    
    //printf("\n");
    
    CIAA.pra |= df[driveSelected].pra & 0x3C;
    //CIAB.prb |= df[driveSelected].prb & 0x87;
    

}

