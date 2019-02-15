//
//  endianMacros.h
//  Emulatron
//
//  Created by Matt Parsons on 03/02/2017.
//  Copyright © 2017 Matt Parsons. All rights reserved.
//

#ifndef endianMacros_h
#define endianMacros_h

/* Read/write macros handles endian issues... but they look stupidly slow */
#define READ_BYTE(BASE, ADDR) (BASE)[ADDR]
#define READ_WORD(BASE, ADDR) (((BASE)[ADDR]<<8)  |	 (BASE)[(ADDR)+1])
#define READ_LONG(BASE, ADDR) (((BASE)[ADDR]<<24) |	((BASE)[(ADDR)+1]<<16) | ((BASE)[(ADDR)+2]<<8) | (BASE)[(ADDR)+3])


#define WRITE_BYTE(BASE, ADDR, VAL) (BASE)[ADDR] = (VAL)&0xff

#define WRITE_WORD(BASE, ADDR, VAL) (BASE)[ADDR] = ((VAL)>>8) & 0xff;		\
(BASE)[(ADDR)+1] = (VAL)&0xff

#define WRITE_LONG(BASE, ADDR, VAL) (BASE)[ADDR] = ((VAL)>>24) & 0xff;		\
(BASE)[(ADDR)+1] = ((VAL)>>16)&0xff;	\
(BASE)[(ADDR)+2] = ((VAL)>>8)&0xff;		\
(BASE)[(ADDR)+3] =  (VAL)&0xff

#endif /* endianMacros_h */
