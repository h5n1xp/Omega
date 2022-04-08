//
//  Floppy.c
//  Omega2
//
//  Created by Matt Parsons on 05/04/2022.
//

#include "Memory.h"
#include "CIA.h"

#include "Floppy.h"

typedef struct{
    int state;
    int motor;
    int direction;
    int track;
    int side;
    int delayCountdown;
    int stepReady;
    uint8_t mask;
    uint8_t* ADF;
} Floppy_t;

Floppy_t drive[4]; // 4 Floppy drives

int driveLUT[]={-1,0,1,-1,2,-1,-1,-1,3,-1,-1,-1,-1,-1,-1,-1};
int activeDrive;

void FloppyInit(void){
    
    int idcode = -31;
    
    drive[0].state = idcode;
    drive[0].motor = 0;
    drive[0].direction = 0;
    drive[0].mask = 0x8;
    drive[0].ADF = 0;
    drive[0].stepReady = 1;

    drive[1].state = idcode;
    drive[1].motor = 0;
    drive[1].direction = 0;
    drive[1].mask = 0x10;
    drive[1].ADF = 0;
    drive[1].stepReady = 1;
    
    drive[2].state = idcode;
    drive[2].motor = 0;
    drive[2].direction = 0;
    drive[2].mask = 0x20;
    drive[2].ADF = 0;
    drive[2].stepReady = 1;
    
    drive[3].state = idcode;
    drive[3].motor = 0;
    drive[3].direction = 0;
    drive[3].mask = 0x40;
    drive[3].ADF = 0;
    drive[3].stepReady = 1;
    
    activeDrive = -1;
}

void FloppyReset(){
    drive[0].motor = 0;
    drive[1].motor = 0;
    drive[2].motor = 0;
    drive[3].motor = 0;
}

void FloppyInsert(int number,uint8_t* ADFdata){
    drive[number].ADF = ADFdata;
}

void FloppyEject(int number){
    drive[number].ADF = 0;
    SetDriveEmpty();
}

uint8_t oldPRB;

void FloppyExecute(){
        
    //check CIA B - PRB for any signals
    uint8_t PRB = ~RAM24bit[0xBFD100];  // invert the signals to make them easier to understand
    uint8_t PRA = RAM24bit[0xBFE001];   // Just for DEbugging not needed for code to work
    int number = driveLUT[(PRB >> 3) & 0xF];
    
    
    if(PRB != oldPRB){

        //printf("Drive %d Something Changed!\n",number);
    }
    oldPRB = PRB;
    
    //No drive Selected
    if(number == -1){
        ClearDriveReady();
        activeDrive = number;
        return;
    }
    

    
    //A new drive has been selected!
    if(number != activeDrive){
        
        //Drive has been selected and the motor is off so increment the selected count
        if((PRB & 0x80) == 0){
            //ClearDriveReady();  //Drive definitely not ready;
            //SetDriveReady();
            //printf("Drive %d ID Pulse: %d  (PRA: 0x%x) ********************************************************\n",number,drive[number].state,PRA); printCPUContext();
            //drive[number].state +=1; //increment the select count,
            drive[number].state = 1; //Set the Drive Ready flag
        }
        activeDrive = number;
    }
    
    //If the motor needs to be on
    if(PRB & 0x80){
        
        if(drive[number].motor == 0){
            printf("Drive %d Motor On  (PRA: 0x%x) ********************************************************\n",number, PRA); printCPUContext();

            drive[number].state = 1;   //Disk ready immediatly
            drive[number].motor = 1;
            
            //drive[number].delayCountdown = 10;
        }
        
        //Motor is on but we need to wait for disk to spin up
       //drive[number].delayCountdown -= 1;
       // if( drive[number].delayCountdown < 0){
        //    drive[number].state = 1;   //Disk ready
        //}
        
    }else{
        
        if(drive[number].motor == 1){
            drive[number].state = -33;//Disk not ready, reset the select counter
            printf("Drive %d Motor Off  (PRA: 0x%x) ********************************************************\n",number, PRA); printCPUContext();
            drive[number].motor = 0;
        }
        
    }
    
    if(drive[number].state > 0){
        SetDriveReady();
    }else{
        ClearDriveReady();
    }
    
    //Check we are on the correct side
    if( drive[number].side != (PRB & 0x4) ){
        drive[number].side = (PRB & 0x4);
        printf("Drive %d Changed Side  (PRA: 0x%x) ********************************************************",number, PRA); printCPUContext();
    }
    
    //Step the head
    if( (PRB & 0x1) && (drive[number].stepReady == 1) ){
        drive[number].stepReady = 0;
        
        printf("Drive %d Step Head ",number);
        
        if(drive[number].ADF){
            SetDriveFull();
        }else{
            SetDriveEmpty();
        }
        
        //Set track by Direction
        int direction = (PRB >> 1) & 1;
        
        if(direction){
            drive[number].track += 1;
        }else{
            drive[number].track -= 1;
            if(drive[number].track < 0){drive[number].track = 0;}
        }
        
        printf(" Track - %d (PRA: 0x%x) ********************************************************\n",drive[number].track, PRA); printCPUContext();
        printf(""); //Essentially a NOP
    }
    
    //Reset the step latch
    if( (PRB & 0x1) == 0){
        drive[number].stepReady = 1;
    }
    

    //No disk, then clear the flag
    if(!drive[number].ADF){
        SetDriveEmpty();
    }
    
    
    //Set Zero Flag
    if(drive[number].track == 0){
        SetTrack0();
    }else{
        ClearTrack0();
    }
    
    
}
