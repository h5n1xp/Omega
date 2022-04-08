//
//  CIA.h
//  Omega2
//
//  Created by Matt Parsons on 28/03/2022.
//

#ifndef CIA_h
#define CIA_h

#include <stdio.h>
#include <stdint.h>

#define      CIA_APRA      0xBFE001
#define      CIA_APRB      0xBFE101
#define      CIA_ADDRA     0xBFE201
#define      CIA_ADDRB     0xBFE301
#define      CIA_ATALO     0xBFE401
#define      CIA_ATAHI     0xBFE501
#define      CIA_ATBLO     0xBFE601
#define      CIA_ATBHI     0xBFE701
#define      CIA_ATODL     0xBFE801
#define      CIA_ATODM     0xBFE901
#define      CIA_ATODH     0xBFEA01
#define      CIA_ASDR      0xBFEC01
#define      CIA_AICR      0xBFED01
#define      CIA_ACRA      0xBFEE01
#define      CIA_ACRB      0xBFEF01
#define      CIA_BPRA      0xBFD000
#define      CIA_BPRB      0xBFD100
#define      CIA_BDDRA     0xBFD200
#define      CIA_BDDRB     0xBFD300
#define      CIA_BTALO     0xBFD400
#define      CIA_BTAHI     0xBFD500
#define      CIA_BTBLO     0xBFD600
#define      CIA_BTBHI     0xBFD700
#define      CIA_BTODL     0xBFD800
#define      CIA_BTODM     0xBFD900
#define      CIA_BTODH     0xBFDA00
#define      CIA_BSDR      0xBFDC00
#define      CIA_BICR      0xBFDD00
#define      CIA_BCRA      0xBFDE00
#define      CIA_BCRB      0xBFDF00

typedef struct{
    void (*Write[32])(uint8_t value);
    
    uint16_t ATA;
    uint16_t ATAPreScaler;
    uint16_t ATB;
    uint16_t ATBPreScaler;
    uint16_t BTA;
    uint16_t BTAPreScaler;
    uint16_t BTB;
    uint16_t BTBPreScaler;
    
    uint32_t ATOD;
    uint32_t ATODLatch;
    uint32_t ATODAlarm;
    uint32_t BTOD;
    uint32_t BTODLatch;
    uint32_t BTODAlarm;
    
    uint8_t AICR;
    uint8_t BICR;
}CIA_t;

extern CIA_t* CIAState;

void InitCIA(void* chipstate, void* memory);
void RunCIACycle(void);
void WriteCIA(unsigned int address, uint8_t value);

void CIAATOD(void);
void CIABTOD(void);

void SetTrack0(void);
void ClearTrack0(void);
void SetDriveReady(void);
void ClearDriveReady(void);
void SetDriveFull(void);
void SetDriveEmpty(void);

void releaseKey(uint16_t keyCode);
void pressKey(uint16_t keyCode);

#endif /* CIA_h */
