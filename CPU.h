//
//  CPU.h
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

#ifndef CPU__HEADER
#define CPU__HEADER

#include <stdio.h>
#include <stdint.h>
#include "Chipset.h"

#define ADDRESS_SPACE_SIZE 16777215 // address starts at 0, so is 1 less than 16777216 (2^24)

unsigned int cpu_read_byte(unsigned int address);
unsigned int cpu_read_word(unsigned int address);
unsigned int cpu_read_long(unsigned int address);
void cpu_write_byte(unsigned int address, unsigned int value);
void cpu_write_word(unsigned int address, unsigned int value);
void cpu_write_long(unsigned int, unsigned int);
void cpu_pulse_reset(void);
void cpu_set_fc(unsigned int fc);
int  cpu_irq_ack(int level);
void cpu_instr_callback();

void cpu_init();
void cpu_execute();

void checkInterrupt(Chipset_t*);
void system_loadState(uint32_t*);


extern int disass;

#endif /* SIM__HEADER */




