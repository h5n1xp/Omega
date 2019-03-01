//
//  BCM2837.h
//  The Omega Project
//  https://github.com/h5n1xp/Omega
//
//  Created by Matt Parsons on 20/02/2019.
//  Copyright © 2019 Matt Parsons. All rights reserved.
//  <h5n1xp@gmail.com>
//
//
//  This Source Code Form is subject to the terms of the
//  Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
//  with this file, You can obtain one at http://mozilla.org/MPL/2.0/.


/*
 https://github.com/dwelch67/raspberrypi
 
 A Basic memory map of How Omega will look on a RaspberryPi 3
 
 EmuMem (16 megs dedicated to firmware, buffers, floppy disk images, etc).      0x0
 Emulator executable Code located here	                                        0x80000
 low16Meg(appears as first 16meg to 68K) - 16 Meg	                            0x1000000
 FastRam (Adress space 0x1000000 to 0x2000000 is not available to 68k)  - 1GB 	0x2000000
 Preiperal base                                                                 0x3F000000
 System Timer	                                                                0x3F003000
 Interrupt controller	                                                        0x3F00B000
 VideoCore mailbox                                                           	0x3F00B880
 Power management	                                                            0x3F100000
 Random Number Generator	                                                    0x3F104000
 General Purpose IO controller	                                                0x3F200000
 UART0 (serial port, PL011)                                                     0x3F201000
 UART1 (serial port, AUX mini UART)                                             0x3F215000
 External Mass Media Controller (SD card reader)                             	0x3F300000
 Universal Serial Bus controller	                                            0x3F980000
 
 */

#ifndef BCM2837_h
#define BCM2837_h

#include <stdio.h>




#endif /* BCM2837_h */
