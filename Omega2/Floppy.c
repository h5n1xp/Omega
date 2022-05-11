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
    int cylinder;
    int side;
    int delayCountdown;
    int stepReady;
    int empty;
    uint8_t mask;
    uint8_t* ADF;
    uint8_t* RAW;
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
    drive[0].empty = 1;

    drive[1].state = idcode;
    drive[1].motor = 0;
    drive[1].direction = 0;
    drive[1].mask = 0x10;
    drive[1].ADF = 0;
    drive[1].stepReady = 1;
    drive[1].empty = 1;
    
    drive[2].state = idcode;
    drive[2].motor = 0;
    drive[2].direction = 0;
    drive[2].mask = 0x20;
    drive[2].ADF = 0;
    drive[2].stepReady = 1;
    drive[2].empty = 1;
    
    drive[3].state = idcode;
    drive[3].motor = 0;
    drive[3].direction = 0;
    drive[3].mask = 0x40;
    drive[3].ADF = 0;
    drive[3].stepReady = 1;
    drive[3].empty = 1;
    
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
    drive[number].RAW = ADFdata;
    printf("Floppy inserted in df%d\n",number);
}

void FloppyEject(int number){
    drive[number].ADF = 0;
    drive[number].empty = 1;
    SetDriveEmpty();
}

//uint8_t oldPRB;

void FloppyCycle(){
        
    //check CIA B - PRB for any signals
    uint8_t PRB = ~RAM24bit[0xBFD100];  // invert the signals to make them easier to understand
    //uint8_t PRA = RAM24bit[0xBFE001];   // Just for DEbugging not needed for code to work
    int number = driveLUT[(PRB >> 3) & 0xF];
    
    
    
    //No drive Selected
    if(number == -1){
        
        if(activeDrive != -1){
            ClearDriveReady();
            activeDrive = number;
           // printf("\n");
        }
        return;
    }
    

    
    //A new drive has been selected!
    if(number != activeDrive){
        
        //Drive has been selected and the motor is off so Set the drive ready flag
        if((PRB & 0x80) == 0){
            drive[number].state = 1; //Set the Drive Ready flag
            //printf("Drive %d Selected\n",number);
        }
        
        activeDrive = number;
    }
    
    //If the motor needs to be on
    if(PRB & 0x80){
        
        if(drive[number].motor == 0){
            //printf("Drive %d Motor On  (PRA: 0x%x) ********************************************************\n",number, PRA); printCPUContext();

            drive[number].state = 1;   //Disk ready immediatly
            drive[number].motor = 1;
            
            drive[number].delayCountdown = 800;
        }
        
        //Motor is on but we need to wait for disk to spin up
       //drive[number].delayCountdown -= 1;
        if( drive[number].delayCountdown < 0){
            drive[number].state = 1;   //Disk ready
        }
        
        
    }else{
        
        if(drive[number].motor == 1){
            drive[number].state = -33;//Disk not ready, reset the select counter
            //printf("Drive %d Motor Off  (PRA: 0x%x) ********************************************************\n",number, PRA); printCPUContext();
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
        //printf("Drive %d Changed Side  (PRA: 0x%x) ********************************************************\n",number, PRA); printCPUContext();
    }
    
    //Step the head
    if( (PRB & 0x1) && (drive[number].stepReady == 1) ){
        drive[number].stepReady = 0;
        
        //printf("Drive %d Step Head ",number);
        
        if(drive[number].ADF){
            drive[number].empty = 0;
        }else{
            drive[number].empty = 1;
        }
        
        //Set track step Direction
        int direction = (PRB >> 1) & 1;
        
        if(direction){
            drive[number].cylinder += 1;
        }else{
            drive[number].cylinder -= 1;
            if(drive[number].cylinder < 0){drive[number].cylinder = 0;}
        }
        
        //printf("Cylinder (side: %d) - %d (PRA: 0x%x) ********************************************************\n",drive[number].side >> 2,drive[number].cylinder, PRA); printCPUContext();
        printf(""); //Printf just as a debug break point
    }
    
    //Reset the step latch
    if( (PRB & 0x1) == 0){
        drive[number].stepReady = 1;
    }
    

    //No disk, then clear the flag
    if(drive[number].empty){
        SetDriveEmpty();
    }else{
        SetDriveFull();
    }
    
    //Set Zero Flag
    if(drive[number].cylinder == 0){
        SetTrack0();
    }else{
        ClearTrack0();
    }

    
    
}


void ADFTrack2MFMTrack(uint8_t* adfTrack,int cylinder, int side, uint32_t address, unsigned  int len);

void FloppyReadTrack(uint32_t DSKPTR, int len){
    
    if(activeDrive == -1 || DSKPTR == 0){
       // printf("Error, No active Drive!\n");
        return;
    }
    
    int cylinder = drive[activeDrive].cylinder;
    int side = drive[activeDrive].side >> 2;
    
   // int index = ((cylinder* 22) + (side* 11)) * 512; // Old Calculation
    int index = ((cylinder << 1) | side) * 5632;       //Optimised calculation
    
    //printf("Disk Read: Cylinder %d, Side %d\n",cylinder, side);
    
    //Copy the ADF track into the Amiga's memory encoding it into MFM on the way
    ADFTrack2MFMTrack(&drive[activeDrive].ADF[index], cylinder, side, DSKPTR, len);
    

    
    //printf("Buffer: 0x%x\n",DSKPTR);
}


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




void ADFTrack2MFMTrack(uint8_t* adfTrack,int cylinder, int side, uint32_t address, unsigned int len){

    len = len * 2;
    
    uint8_t* buffer = &RAM24bit[address];
    

    
    int s = 0; //the buffer index.. called s, I don't remember why.
    uint8_t lowlevelSector[544]; //bytes per low level sector
 
    for(int sector=0; sector < 11; ++sector){
        
        
        //int secCountDown = 11 - sector;
        
        
        //printf("%d: Cylinder %d, Side: %d, Sector %d (%d)\n",s,cylinder, side,sector,11 - sector);
        
        //Build the sector
        lowlevelSector[0] = 0x0;
        lowlevelSector[1] = 0x0;
        
        lowlevelSector[2] = 0xA1; // will be a sync mark
        lowlevelSector[3] = 0xA1; // will be a sync mark
        
        //sector info
        
        lowlevelSector[4] = 0xFF;
        lowlevelSector[5] = cylinder << 1 | side;
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
            lowlevelSector[32+i] = adfTrack[(sector*512)+i];
            
        }
        
        
        
        //***************
        
        //Encode
        
 
        buffer[s+0] = 0xAA;
        buffer[s+1] = 0xAA;
        buffer[s+2] = 0xAA;
        buffer[s+3] = 0xAA;
        
        buffer[s+4] = 0x44;
        buffer[s+5] = 0x89;
        buffer[s+6] = 0x44;
        buffer[s+7] = 0x89;
        
        
        //info
        encodeBlock(&lowlevelSector[4], &buffer[s+8], 4); // adds 8 bytes
        
        //Disklabel
        encodeBlock(&lowlevelSector[8], &buffer[s+16], 16);//adds 32 bytes
        
        //Data section
        encodeBlock(&lowlevelSector[32], &buffer[s+64], 512);
        
        
        //Header checksum
        uint8_t hcheck[4] = { 0, 0, 0, 0 };
        for(unsigned i = 8; i < 48; i += 4) {
            hcheck[0] ^= buffer[s+i];
            hcheck[1] ^= buffer[s+i+1];
            hcheck[2] ^= buffer[s+i+2];
            hcheck[3] ^= buffer[s+i+3];
        }
        
        lowlevelSector[24] = hcheck[0];
        lowlevelSector[25] = hcheck[1];
        lowlevelSector[26] = hcheck[2];
        lowlevelSector[27] = hcheck[3];
        
        //header checksum
        encodeBlock(&lowlevelSector[24], &buffer[s+48], 4); //adds 8 bytes
        
        
        // Data checksum
        uint8_t dcheck[4] = { 0, 0, 0, 0 };
        for(unsigned i = 64; i < 1088; i += 4) {
            dcheck[0] ^= buffer[s+i];
            dcheck[1] ^= buffer[s+i+1];
            dcheck[2] ^= buffer[s+i+2];
            dcheck[3] ^= buffer[s+i+3];
        }
        
        lowlevelSector[28] = dcheck[0];
        lowlevelSector[29] = dcheck[1];
        lowlevelSector[30] = dcheck[2];
        lowlevelSector[31] = dcheck[3];
        
        //Encode Data checksum
        encodeBlock(&lowlevelSector[28], &buffer[s+56], 4); //adds 8 bytes
        
        
        
        //Add clocking bits
        for(int i=8;i<1088;i++){
            uint8_t previous = buffer[s+i-1];
            
            buffer[s+i] = addClockBits(previous,buffer[s+i]);
            
        }
        
        s += 1088;    //Why 1088? this is the size of the MFM data block we've just produced.
        
    }
    
    //Add clocking bits to the track gap
    /*

    buffer[s]   = addClockBits(buffer[s-1],0);
    buffer[s+1] = 0xA8;
    buffer[s+2] = 0x55;
    buffer[s+3] = 0x55;
    buffer[s+4] = 0xAA;
    
    for(int i=5;i<830;i++){
        uint8_t previous = buffer[s+i-1];
        
        buffer[s+i] = addClockBits(previous,0);
        
    }
    */
    
    
    for(int i=0;i<700;i++){
        uint8_t previous = buffer[s+i-1];
        buffer[s+i] = addClockBits(previous,0);
    }
    
    s += 700;   //pad track to make 12668 bytes which is a normal AmigaDOS disk
    //printf("\n");
    
    //If the buffer is bigger than the track, then just loop back around until buffer is full
    int rep = 0;
    for(int i = s; i < len; ++i){
        buffer[i] = buffer[rep];
        rep++;
    }
    
    
    DiskIndexPulse();
    
    
}

unsigned int FloppyCountdown = 0;
int FloppyTrackReady = 0;

void FloppyExecute(Chipset_t* ChipsetState){
    uint16_t* DSKLEN = (uint16_t*)&ChipsetState->chipram[0xDFF024];

    
    if(*DSKLEN & 0x8000){
        uint32_t* address = (uint32_t*)&ChipsetState->chipram[0xDFF020];

        
        if(FloppyTrackReady==0){
            FloppyReadTrack(*address, *DSKLEN & 0x3FFF);
            FloppyTrackReady = 1;
            FloppyCountdown = (*DSKLEN);
        }
        
        FloppyCountdown -= 1; // Emulate the read delay
        if(FloppyCountdown == 0){
            WriteChipsetWord(0xDFF09C, 0x8002); // Generate a disk block finished interrupt
            *DSKLEN &= 0x7FFF; //Clear the DISK DMA Enable Flag..
            *address += (*DSKLEN & 0x3FFF) << 1; // Pointer incremented by the byte length.
            FloppyTrackReady = 0;
        }
        
    }
}
