//
//  debug.c
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

#include "debug.h"

int debugChipAddress;
int debugChipValue;

/*
 uint32_t buffer[65536];
 
 uint8_t* dat = df0.mfmData;
 for(int h=0;h<size;h +=12798){
 
 
 for(int i=h;i<h+12798;++i){
 int test = dat[i] <<24 | dat[i+1] <<16 | dat[i+2] << 8 | dat[i+3];
 
 
 if(test==0x44894489){
 
 int odd  = dat[i+4] <<24 | dat[i+5] <<16 | dat[i+6] << 8 | dat[i+7];
 int even  = dat[i+8] <<24 | dat[i+9] <<16 | dat[i+10] << 8 | dat[i+11];
 int info = ((odd & 0x55555555) << 1) | (even & 0x55555555);
 
 int track = (info >> 17) & 127;
 int head  = (info >> 16) & 1;
 
 printf("%d: Track %d, Side: %d, Sector: %d (%d)\n",i,track,head,(info >>8) & 255,info & 255);
 
 for(int j=60;j<512+64;++j){
 
 if(j>492 && j<523){
 char ch =((dat[i+j] & 0x55) << 1) | (dat[512+i+j] & 0x55);
 //printf("%c",ch);
 }
 
 }
 //printf("\n");
 
 }
 
 }
 
 printf("\n");
 }
 */

// Experimental ADF to MFM loader


/* //Code to convert BMP to binary data
 SDL_Surface* logo = SDL_LoadBMP("/Users/Shared/uae/clogo.bmp");
 
 uint32_t* pix =logo->pixels;
 
 for(int w=0;w<50;++w){
 for(int h=0;h<50;++h){
 
 uint8_t r= (*pix >> 24) & 255;
 uint8_t g= (*pix >> 16) & 255;
 uint8_t b= (*pix >> 8 )& 255;
 uint8_t a = *pix & 255;
 
 printf("%d,%d,%d,%d,",r,g,b,a);
 pix++;
 
 }
 }
 */


/*
 //*************************************** ADF to MFM... doesn't work
 fd = open("/Users/Shared/uae/WORKBENCH/wbraw.adf",O_RDWR);
 const char name[] ={"UAE-1ADF   "};
 uint8_t cylind = 160;
 uint8_t nul = 0;
 lseek(fd, 0, SEEK_SET);
 write(fd, name, 8);
 write(fd, &nul, 1);
 write(fd, &nul, 1);
 write(fd, &nul, 1);
 write(fd, &cylind, 1);
 int byte=0;
 for(int trac=0;trac<160;++trac){
 uint32_t type = 1;
 type = ((type << 8) & 0xFF00FF00 ) | ((type >> 8) & 0xFF00FF );
 type = type << 16 | type >> 16;
 write(fd, &type, 4);
 
 type = (11*544)*2;
 type = ((type << 8) & 0xFF00FF00 ) | ((type >> 8) & 0xFF00FF );
 type = type << 16 | type >> 16;
 write(fd, &type, 4);
 
 type = ((11*544)*2)*8;
 type = ((type << 8) & 0xFF00FF00 ) | ((type >> 8) & 0xFF00FF );
 type = type << 16 | type >> 16;
 write(fd, &type, 4);
 
 }
 
 for(int trac=0;trac<160;++trac){
 for(int side =0; side<2;++side){
 adf2rawtrack((uint8_t*)df0.data, df0.mfmTrack, trac, side);
 
 write(fd, df0.mfmTrack, 1088*11);
 
 }
 }
 
 close(fd);
 */

/*
int MFMEncode(int previous, int byte){
    
    int value = byte;
    
    if( (previous & 1)==0 && (byte & 64)==0){
        value |= 128;
    }
    
    if((byte & 64)==0 && (byte & 16)==0){
        value |= 32;
    }
    
    if((byte & 16)==0 && (byte & 4)==0){
        value |= 8;
    }
    
    if((byte & 4)==0 && (byte & 1)==0){
        value |= 2;
    }
    
    return value;
}
*/



/*
void adf2rawtrack(uint8_t* adf,uint16_t* track,int cylinder,int surface){ //returns little endian data
    
    //surface = 1 - surface; //invert value :-/
    
    int index = 0;
    for(int sectorNumber=0;sectorNumber<11;++sectorNumber){
        index = sectorNumber*544;
        
        //generate Sector header
        
        
        int id = 0xFF;
        int tn =(cylinder << 1) | surface;
        int sn = sectorNumber;
        int cd = 10 - sn; if(cd ==-1){cd = 11;}
        
        int ido = (id >> 1) & 0x55; id &= 0x55;
        int tno = (tn >> 1) & 0x55; tn &= 0x55;
        int sno = (sn >> 1) & 0x55; sn &= 0x55;
        int cdo = (cd >> 1) & 0x55; cd &= 0x55;
        
        track[index]   = 0xAAAA;
        track[index+1] = 0xAAAA;
        track[index+2] = 0x8944;
        track[index+3] = 0x8944;
        
        track[index+4] = MFMEncode(0x89, ido) | (MFMEncode(ido, tno) << 8);
        track[index+5] = MFMEncode(tno, sno)  | (MFMEncode(sno, cdo) << 8);
        track[index+6] = MFMEncode(cdo, id)   | (MFMEncode(id, tn)   << 8);
        track[index+7] = MFMEncode(tn,sn)     | (MFMEncode(sn, cd)   << 8);
        track[index+8] = MFMEncode(cd, 0)     | (MFMEncode(0, 0)     << 8);
        
        track[index+9]  = 0xAAAA;
        track[index+10] = 0xAAAA;
        track[index+11] = 0xAAAA;
        track[index+12] = 0xAAAA;
        track[index+13] = 0xAAAA;
        track[index+14] = 0xAAAA;
        track[index+15] = 0xAAAA;
        track[index+16] = 0xAAAA;
        track[index+17] = 0xAAAA;
        track[index+18] = 0xAAAA;
        track[index+19] = 0xAAAA;
        track[index+20] = 0xAAAA;
        track[index+21] = 0xAAAA;
        track[index+22] = 0xAAAA;
        track[index+23] = 0xAAAA;
        
        //header check sum
        uint32_t sum = 0;
        
        for(int i = 4; i < 22;++i){
            sum ^=track[index+i];
            sum = ((sum >> 1) & 0x55555555) ^ (sum & 0x55555555);
        }
        
        
        uint32_t sumo = (sum >> 1) & 0x55555555; sum &= 0x55555555;
        
        track[index+24] = (MFMEncode(0, (sumo>>24) & 255) ) | (MFMEncode((sumo>>24) & 255,(sumo>>16) & 255) <<8);
        track[index+25] =  (MFMEncode((sumo>>16) & 255, (sumo>>8) & 255)) | (MFMEncode((sumo>>8) & 255, sumo & 255) << 8);
        
        track[index+26] = (MFMEncode(0, (sum>>24) & 255) ) | (MFMEncode((sum>>24) & 255,(sum>>16) & 255) <<8);
        track[index+27] =  (MFMEncode((sum>>16) & 255, (sum>>8) & 255)) | (MFMEncode((sum>>8) & 255, sum & 255) << 8);
        
        
        int LBA = ((cylinder*22)+(surface*11)+sectorNumber)*512; //Logical block address in the adf data.
        
        int prevOdd = 0;
        int prevEven = 0;
        uint8_t* tracDat =(uint8_t*)&track[index+32];
        
        //data Section
        for(int i=0;i<512; i +=1){
            
            char debug = adf[LBA+i];
            int even = adf[LBA+i];
            int odd = (even >> 1) & 0x55;even &= 0x55;
            
            tracDat[i]     = MFMEncode(prevOdd, odd);
            tracDat[i+512] = MFMEncode(prevEven, even);
            prevEven = even;
            prevOdd = odd;
        }
        
        //data checksum
        sum = 0;
        
        for(int i = 32; i < 544;++i){
            sum ^=track[index+i];
            sum = ((sum >> 1) & 0x55555555) ^ (sum & 0x55555555);
        }
        
        
        sumo = (sum >> 1) & 0x55555555; sum &= 0x55555555;
        
        track[index+28] = (MFMEncode(0, (sumo>>24) & 255) ) | (MFMEncode((sumo>>24) & 255,(sumo>>16) & 255) <<8);
        track[index+29] =  (MFMEncode((sumo>>16) & 255, (sumo>>8) & 255)) | (MFMEncode((sumo>>8) & 255, sumo & 255) << 8);
        
        track[index+30] = (MFMEncode(0, (sum>>24) & 255) ) | (MFMEncode((sum>>24) & 255,(sum>>16) & 255) <<8);
        track[index+31] =  (MFMEncode((sum>>16) & 255, (sum>>8) & 255)) | (MFMEncode((sum>>8) & 255, sum & 255) << 8);
        
    }
 */

    
    
    //return;
    
    /*
     
     //sector gap 5984 -> 6400;
     if(index>5983){
     return 0xAAAA;
     }
     
     surface = 1 - surface;//reverse value :(
     
     uint16_t value=0;
     
     int sn = index/544; //sector number
     
     int sectorIndex = index % 544;
     int LBA = ((cylinder*22)+(surface*11)+sn)*512+(sectorIndex-32); //Logical block address in the adf data.
     
     
     //even data section
     if(sectorIndex>288){
     
     LBA = LBA - 256;
     
     int previous = 0;
     
     if(LBA >0){
     previous = (adf[LBA-1] >> 1) & 0x55;
     }
     
     int byte1 = adf[LBA] & 0x55;
     int byte2 = adf[LBA+1] &0x55;
     
     value = MFMEncode(previous, byte1) << 8 | MFMEncode(byte1, byte2);
     
     return value;
     }
     
     //odd data section
     if(sectorIndex>31){
     
     LBA = LBA >> 1;
     
     int previous = 0;
     
     if(LBA >0){
     previous = (adf[LBA-1] >> 1) & 0x55;
     }
     
     int byte1 = (adf[LBA]   >> 1)  & 0x55;
     int byte2 = (adf[LBA+1] >> 1) & 0x55;
     
     value = MFMEncode(previous, byte1) << 8 | MFMEncode(byte1, byte2);
     
     
     return value;
     }
     
     //generate data checksum
     if(sectorIndex>27){
     value = (MFMEncode(0, 0) <<8) | MFMEncode(0, 0);
     
     return value;
     }
     
     //generate header checksum
     if(sectorIndex>23){
     
     uint32_t sum = 0;
     
     for(int i = 0; i < 24;++i){
     sum ^=adf2rawtrack(adf, cylinder, surface, index);
     }
     
     if(sectorIndex==24){
     value = (MFMEncode(0, (sum>>24) & 255) <<8) | MFMEncode((sum>>24) & 255,(sum>>16) & 255);
     }
     
     if(sectorIndex==26){
     value = (MFMEncode((sum>>16) & 255, (sum>>8) & 255) <<8) | MFMEncode((sum>>8) & 255, sum & 255);
     }
     
     
     
     return value;
     }
     
     //generate Sector header
     int id = 0xFF;
     int tn =(cylinder << 1) | surface;
     int cd = 11 - sn;
     
     int ido = (id >> 1) & 0x55; id &= 0x55;
     int tno = (tn >> 1) & 0x55; tn &= 0x55;
     int sno = (sn >> 1) & 0x55; sn &= 0x55;
     int cdo = (cd >> 1) & 0x55; cd &= 0x55;
     
     
     
     switch(sectorIndex){
     case 0: value = 0xAAAA; break;
     case 1: value = 0xAAAA; break;
     case 2: value = 0x4489; break;
     case 3: value = 0x4489; break;
     
     case 4: value = MFMEncode(0x89, ido) << 8 | MFMEncode(ido, tno); break; // odd bits
     case 5: value = MFMEncode(tno, sno)  << 8 | MFMEncode(sno, cdo); break; // odd bits
     case 6: value = MFMEncode(cdo, id)   << 8 | MFMEncode(id, tn);   break; // even bits
     case 7: value = MFMEncode(tn,sn)     << 8 | MFMEncode(sn, cd);   break; // even bits
     
     case 8: value = MFMEncode(cd, 0)     << 8 | MFMEncode(0, 0);     break;
     
     default:value = 0xAAAA; break;
     }
     
     return value;
     */
//}
/*
void ADF2MFMOld(int adfSize,uint8_t* adf, uint8_t MFMdisk[80][2][11][1088]){
    /*
     
     
     uint8_t LLFdisk[80][2][11][544];
     
     int index =0;
     for(int track=0;track<80;++track){
     for(int surface=0;surface<2;++surface){
     for(int sector=0;sector<11;++sector){
     for(int byte=32;byte<544;++byte){
     
     if(byte==32){
     //Buid Low level header
     int block = index / 512;
     LLFdisk[track][surface][sector][0] = 0;
     LLFdisk[track][surface][sector][1] = 0;
     
     LLFdisk[track][surface][sector][2] = 0xA1;
     LLFdisk[track][surface][sector][3] = 0xA1;
     
     LLFdisk[track][surface][sector][4] = 0xFF;
     LLFdisk[track][surface][sector][5] = ((block / 22) << 1 ) | (((block % 22) < 11) ? 0:1);
     LLFdisk[track][surface][sector][6] = block % 11;
     LLFdisk[track][surface][sector][7] = 11 - sector;
     
     // Sector label area - always 0
     LLFdisk[track][surface][sector][8]  = 0;
     LLFdisk[track][surface][sector][9]  = 0;
     LLFdisk[track][surface][sector][10] = 0;
     LLFdisk[track][surface][sector][11] = 0;
     
     LLFdisk[track][surface][sector][12] = 0;
     LLFdisk[track][surface][sector][13] = 0;
     LLFdisk[track][surface][sector][14] = 0;
     LLFdisk[track][surface][sector][15] = 0;
     
     LLFdisk[track][surface][sector][16] = 0;
     LLFdisk[track][surface][sector][17] = 0;
     LLFdisk[track][surface][sector][18] = 0;
     LLFdisk[track][surface][sector][19] = 0;
     
     LLFdisk[track][surface][sector][20] = 0;
     LLFdisk[track][surface][sector][21] = 0;
     LLFdisk[track][surface][sector][22] = 0;
     LLFdisk[track][surface][sector][23] = 0;
     
     //Calculate header checksum
     LLFdisk[track][surface][sector][24] = 0;
     LLFdisk[track][surface][sector][25] = 0;
     LLFdisk[track][surface][sector][26] = 0;
     LLFdisk[track][surface][sector][27] = 0;
     
     //Calculate data checksum
     LLFdisk[track][surface][sector][28] = 0;
     LLFdisk[track][surface][sector][29] = 0;
     LLFdisk[track][surface][sector][30] = 0;
     LLFdisk[track][surface][sector][31] = 0;
     
     }
     
     LLFdisk[track][surface][sector][byte] = adf[index];
     
     index +=1;
     }
     
     }
     }
     
     }
     
     
     for(int track=0;track<80;++track){
     for(int surface=0;surface<2;++surface){
     for(int sector=0;sector<11;++sector){
     
     //Buid MFM Sync marker
     int block = index / 512;
     MFMdisk[track][surface][sector][0] = 0xAA;
     MFMdisk[track][surface][sector][1] = 0xAA;
     MFMdisk[track][surface][sector][2] = 0xAA;
     MFMdisk[track][surface][sector][3] = 0xAA;
     
     MFMdisk[track][surface][sector][4] = 0x44;
     MFMdisk[track][surface][sector][5] = 0x89;
     MFMdisk[track][surface][sector][6] = 0x44;
     MFMdisk[track][surface][sector][7] = 0x89;
     
     //Build MFM Info section
     uint16_t info0 = getMFM(LLFdisk[track][surface][sector][3], LLFdisk[track][surface][sector][4]);
     uint16_t info1 = getMFM(LLFdisk[track][surface][sector][4], LLFdisk[track][surface][sector][5]);
     uint16_t info2 = getMFM(LLFdisk[track][surface][sector][5], LLFdisk[track][surface][sector][6]);
     uint16_t info3 = getMFM(LLFdisk[track][surface][sector][6], LLFdisk[track][surface][sector][7]);
     
     uint16_t i0o = (info0 >> 1) & 0x5555;
     uint16_t i0e =  info0 & 0x5555;
     
     uint16_t i1o = (info1 >> 1) & 0x5555;
     uint16_t i1e =  info1 & 0x5555;
     
     uint16_t i2o = (info2 >> 1) & 0x5555;
     uint16_t i2e =  info2 & 0x5555;
     
     uint16_t i3o = (info3 >> 1) & 0x5555;
     uint16_t i3e =  info3 & 0x5555;
     
     //odd bits
     MFMdisk[track][surface][sector][8]  =i0o >> 8;
     MFMdisk[track][surface][sector][9]  =i0o & 255;
     MFMdisk[track][surface][sector][10] =i1o >> 8;
     MFMdisk[track][surface][sector][11] =i1o & 255;
     MFMdisk[track][surface][sector][12] =i2o >> 8;
     MFMdisk[track][surface][sector][13] =i2o & 255;
     MFMdisk[track][surface][sector][14] =i3o >> 8;
     MFMdisk[track][surface][sector][15] =i3o & 255;
     //even bits
     MFMdisk[track][surface][sector][16] =i0e >> 8;
     MFMdisk[track][surface][sector][17] =i0e & 255;
     MFMdisk[track][surface][sector][18] =i1e >> 8;
     MFMdisk[track][surface][sector][19] =i1e & 255;
     MFMdisk[track][surface][sector][20] =i2e >> 8;
     MFMdisk[track][surface][sector][21] =i2e & 255;
     MFMdisk[track][surface][sector][22] =i3e >> 8;
     MFMdisk[track][surface][sector][23] =i3e & 255;
     
     
     // Sector label area - always 0
     LLFdisk[track][surface][sector][8]  = 0;
     LLFdisk[track][surface][sector][9]  = 0;
     LLFdisk[track][surface][sector][10] = 0;
     LLFdisk[track][surface][sector][11] = 0;
     
     LLFdisk[track][surface][sector][12] = 0;
     LLFdisk[track][surface][sector][13] = 0;
     LLFdisk[track][surface][sector][14] = 0;
     LLFdisk[track][surface][sector][15] = 0;
     
     LLFdisk[track][surface][sector][16] = 0;
     LLFdisk[track][surface][sector][17] = 0;
     LLFdisk[track][surface][sector][18] = 0;
     LLFdisk[track][surface][sector][19] = 0;
     
     LLFdisk[track][surface][sector][20] = 0;
     LLFdisk[track][surface][sector][21] = 0;
     LLFdisk[track][surface][sector][22] = 0;
     LLFdisk[track][surface][sector][23] = 0;
     
     //Calculate header checksum
     LLFdisk[track][surface][sector][24] = 0;
     LLFdisk[track][surface][sector][25] = 0;
     LLFdisk[track][surface][sector][26] = 0;
     LLFdisk[track][surface][sector][27] = 0;
     
     //Calculate data checksum
     LLFdisk[track][surface][sector][28] = 0;
     LLFdisk[track][surface][sector][29] = 0;
     LLFdisk[track][surface][sector][30] = 0;
     LLFdisk[track][surface][sector][31] = 0;
     
     
     
     
     
     }
     }
     
     }
     */
//}

//Floppy_t df0;
//Floppy_t df1;
//Floppy_t df2;
//Floppy_t df3;

/*
 void floppy_init(Floppy_t* floppy,int driveNumber){
 
 int shift = driveNumber+3;
 
 floppy->ID = 1 << shift;
 floppy->ID = (~floppy->ID) &0x78;
 
 floppy->diskSelected = 1;           // Start in a non selected state
 floppy->diskMotor    = 1;           // Motor is off
 floppy->diskSide     = 0;           // Upper side selected
 floppy->userInserted = 0;           // drive starts with no disk
 floppy->diskchange   = 0;           // no disk
 floppy->diskReady    = 1;           // Disk not ready
 floppy->index = 0;                  // disk starts at 0... this is the disk rotation
 
 floppy->indexTop = 6399;
 }
 
 
 
 
 
 void floppy_execute(Floppy_t* disk){
 
 static int state = 0;
 
 CIA_t* ciaBView = &CIAB;
 
 // check if Drive has been slected
 if( (CIAB.prbw & 0x78) == disk->ID){
 
 if(state==0){
 state = 1;
 }
 }else{
 state = 0;
 }
 
 switch(state){
 case 0:
 //Do nothing
 break;
 case 1:    //restore CIA A State
 if(disk->diskReady == 0){
 CIAA.pra &= 0xDF;     // clear the disk ready bit
 }else{
 CIAA.pra |= 0x20;     // set the disk ready bit
 }
 
 if(disk->diskTrack == 0){
 CIAA.pra &= 0xEF;     // clear the track 0 bit
 }else{
 CIAA.pra |= 0x10;     // set the track 0 bit
 }
 
 if(disk->diskProtect == 0){
 CIAA.pra &= 0xF7;     // clear the disk protect bit
 }else{
 CIAA.pra |= 0x8;      // set the disk protect bit
 }
 
 if(disk->diskchange == 0){
 CIAA.pra &= 0xFB;     // clear the disk change bit
 }else{
 CIAA.pra |= 0x4;      // set the disk change bit
 }
 
 
 //restore CIA B State to reflect this drive
 CIAB.prb = CIAB.prbw;
 
 if(disk->diskMotor ==0){
 CIAB.prb &= 0x7F;     // clear motor on bit
 }else{
 CIAB.prb |= 0x80;     // set motor on bit
 }
 
 if(disk->diskSide == 0){
 CIAB.prb &= 0xFB;     // clear disk side bit
 }else{
 CIAB.prb |= 0x4;     // set disk side bit
 }
 
 if(disk->diskDirection == 0){
 CIAB.prb &= 0xFD;     // clear disk side bit
 }else{
 CIAB.prb |= 0x2;     // set diskside bit
 }
 
 /*
 if(floppy->diskStep == 0){
 b->prb &= 0xFE;     // clear disk side bit
 }else{
 b->prb |= 0x1;     // set diskside bit
 }
 
 state = 2;
 break;
 
 case 2:
 //update Drive to the new state
 if( (CIAB.prbw & 0x80) == 0){
 CIAB.prb &= 0x7F;
 disk->diskMotor =0;
 //printf("Disk Motor On\n");
 }else if(disk->diskMotor==0){
 CIAB.prb |= 0x80;         // set motor on bit
 disk->diskMotor = 1;  // Motor Off
 CIAA.pra |= 0x20;     // set the disk ready bit as the disk is no longer ready
 disk->diskReady = 1;
 //printf("Disk Motor Off\n");
 
 }
 
 if( (CIAB.prbw & 0x4) == 0){
 CIAB.prb &= 0xFB;         // clear side bit
 disk->diskSide = 0;  // set upper side
 //printf("Top Surface Selected\n");
 }else if(disk->diskSide==0){
 CIAB.prb |= 0x4;         // set lower side
 disk->diskSide = 1;  // lower side
 //printf("Lower Surface Selected\n");
 }
 
 if( (CIAB.prbw & 0x2) == 0){
 CIAB.prb &= 0xFD;         // clear direction bit
 disk->diskDirection = 0;  // direction inwards
 //printf("Direction Inwards\n");
 }else if(disk->diskDirection==0){
 CIAB.prb |= 0x2;         // direction outwards
 disk->diskDirection = 1;  // direction outwards
 //printf("Direction outwards\n");
 }
 
 
 //step head
 if( (CIAB.prbw & 0x1)==0){
 
 if(disk->diskDirection ==0){
 disk->diskTrack +=1;
 }else{
 disk->diskTrack -=1;
 
 if(disk->diskTrack==0){
 CIAA.pra &= 0xEF;     // clear the track 0 bit signalling we are at track 0
 }else{
 CIAA.pra |= 0x10;     // set the track 0 bit
 }
 
 }
 
 
 // has a disk been inserted?
 if(disk->userInserted==1){
 disk->diskchange=1;
 CIAA.pra |= 0x4;      // set the disk change bit
 }else{
 disk->diskchange=0;
 CIAA.pra &= 0xFB;     // clear the disk change bit
 }
 
 //printf("Step head to Cylinder %d\n",floppy->diskTrack);
 
 }
 
 break;
 case 3:
 
 break;
 
 
 }
 
 
 
 }
 void resetIndex(Floppy_t* disk){
 
 disk->index = 0;
 
 }
 void floppyDisk_execute(Floppy_t* floppy,Chipset_t* chipset, CIA_t* a, CIA_t* b){
 
 
 //our drive has been slelcted then we are interested
 if( (b->prbw & 0x78) == floppy->ID){
 
 //only execute if the register state has changed
 if(b->prbChanged){
 b->prbChanged=0;    //reset flag
 
 
 //Check if we need to restore state
 if(floppy->diskSelected==1){
 floppy->diskSelected=0;
 
 switch(floppy->ID){
 //case 0x70: printf("Selected disk 0\n");break;
 //case 0x68: printf("Selected disk 1\n");break;
 //case 0x58: printf("Selected disk 2\n");break;
 //case 0x38: printf("Selected disk 3\n");break;
 }
 
 //restore CIA A State to reflect this drive
 
 if(floppy->diskReady == 0){
 a->pra &= 0xDF;     // clear the disk ready bit
 }else{
 a->pra |= 0x20;     // set the disk ready bit
 }
 
 if(floppy->diskTrack == 0){
 a->pra &= 0xEF;     // clear the track 0 bit
 }else{
 a->pra |= 0x10;     // set the track 0 bit
 }
 
 if(floppy->diskProtect == 0){
 a->pra &= 0xF7;     // clear the disk protect bit
 }else{
 a->pra |= 0x8;      // set the disk protect bit
 }
 
 if(floppy->diskchange == 0){
 a->pra &= 0xFB;     // clear the disk change bit
 }else{
 a->pra |= 0x4;      // set the disk change bit
 }
 
 
 //restore CIA B State to reflect this drive
 b->prb = b->prbw;
 
 if(floppy->diskMotor ==0){
 b->prb &= 0x7F;     // clear motor on bit
 }else{
 b->prb |= 0x80;     // set motor on bit
 }
 
 if(floppy->diskSide == 0){
 b->prb &= 0xFB;     // clear disk side bit
 }else{
 b->prb |= 0x4;     // set disk side bit
 }
 
 if(floppy->diskDirection == 0){
 b->prb &= 0xFD;     // clear disk side bit
 }else{
 b->prb |= 0x2;     // set diskside bit
 }
 
 
 if(floppy->diskStep == 0){
 b->prb &= 0xFE;     // clear disk side bit
 }else{
 b->prb |= 0x1;     // set diskside bit
 }
 
 
 }
 
 //update Drive to the new state
 if( (b->prbw & 0x80) == 0){
 b->prb &= 0x7F;
 floppy->diskMotor =0;
 //printf("Disk Motor On\n");
 }else if(floppy->diskMotor==0){
 b->prb |= 0x80;         // set motor on bit
 floppy->diskMotor = 1;  // Motor Off
 a->pra |= 0x20;     // set the disk ready bit as the disk is no longer ready
 floppy->diskReady = 1;
 //printf("Disk Motor Off\n");
 
 }
 
 if( (b->prbw & 0x4) == 0){
 b->prb &= 0xFB;         // clear side bit
 floppy->diskSide = 0;  // set upper side
 //printf("Top Surface Selected\n");
 }else if(floppy->diskSide==0){
 b->prb |= 0x4;         // set lower side
 floppy->diskSide = 1;  // lower side
 //printf("Lower Surface Selected\n");
 }
 
 if( (b->prbw & 0x2) == 0){
 b->prb &= 0xFD;         // clear direction bit
 floppy->diskDirection = 0;  // direction inwards
 //printf("Direction Inwards\n");
 }else if(floppy->diskDirection==0){
 b->prb |= 0x2;         // direction outwards
 floppy->diskDirection = 1;  // direction outwards
 //printf("Direction outwards\n");
 }
 
 
 //step head
 if( (b->prbw & 0x1)==0){
 
 if(floppy->diskDirection ==0){
 floppy->diskTrack +=1;
 }else{
 floppy->diskTrack -=1;
 
 //Can't step back.
 if(floppy->diskTrack==-1){
 floppy->diskTrack = 0;
 }
 
 if(floppy->diskTrack==0){
 a->pra &= 0xEF;     // clear the track 0 bit signalling we are at track 0
 }else{
 a->pra |= 0x10;     // set the track 0 bit
 }
 
 }
 
 
 // has a disk been inserted?
 if(floppy->userInserted==1){
 floppy->diskchange=1;
 a->pra |= 0x4;      // set the disk change bit
 }else{
 floppy->diskchange=0;
 a->pra &= 0xFB;     // clear the disk change bit
 }
 
 //printf("Step head to Cylinder %d\n",floppy->diskTrack);
 
 }
 
 }
 
 
 }else{
 
 if(floppy->diskSelected==0){
 floppy->diskSelected=1;// our drive is not selected
 return;
 }
 
 }
 
 //run drive
 
 if(floppy->diskMotor==0){
 a->pra &= 0xDF;     // clear the disk ready bit
 }else{
 a->pra |= 0x20;     // set the disk ready bit
 }
 
 }
 
 */
