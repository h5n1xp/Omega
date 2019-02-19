//
//  Floppy.c
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

#include "Floppy.h"
#include "Chipset.h"
#include "Memory.h"

Floppy_t df0;
Floppy_t df1;
Floppy_t df2;
Floppy_t df3;


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




 
 void floppyInsert(Floppy_t* disk){
 
 disk->diskTrack= 0;
 disk->diskProtect = 0;
 disk->diskMotor = 0;
 
 
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
              */
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


uint8_t floppyDataRead(Floppy_t* disk){ //this function should be called by the DMA
    
    int surface = 1 - disk->diskSide;
    int position = (disk->diskTrack * (12798*2)) + (surface * 12798) + (disk->index);
    
    disk->index +=1;
    if(disk->index>12798){
        disk->index = 0;
        CIAIndex(&CIAB);    // generate CIAB index interupt
    }
    
    //Don't allow 0 in the data stream
    uint8_t retVal = disk->mfmData[position];
    if(disk->index>4 & retVal == 0){
        disk->index = 0;
        CIAIndex(&CIAB);    // generate CIAB index interupt
        retVal = disk->mfmData[(disk->diskTrack * (12798*2)) + (surface * 12798) + (disk->index)]; //firstvalue
        disk->index +=1;
    }

    return retVal;
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
                
                /*
                if(floppy->diskStep == 0){
                    b->prb &= 0xFE;     // clear disk side bit
                }else{
                    b->prb |= 0x1;     // set diskside bit
                }
                 */

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



