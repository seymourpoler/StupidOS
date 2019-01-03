/*--------------------------------------------------------------------------------------------------Stupid OS Project Copyright 2001-2002 3DMinus3
Started : Dec 08 2001   Updated : Dec 08 2001
File Name : assem.h     Purpose : C declarations for Assembly functions
--------------------------------------------------------------------------------------------------*/

#ifndef __pic_h__
#define __pic_h__

#define  MASTER_PORT_A 0x20          /* Addresses of the ports      */
#define  MASTER_PORT_B 0x21
#define  SLAVE_PORT_A 0xA0
#define  SLAVE_PORT_B 0xA1

#define TIMER 0x0
#define KEYBOARD 0x1
#define SLAVE 0x2
#define TTY1 0x3
#define TTY2 0x4
#define XT_WINCHESTER 0x5
#define FLOPPY 0x6
#define PRINTER 0x7

#define RTC 0x8
#define FPU_EXCEPTION 0x13
#define AT_WINCHESTER 0x14

void pic_init();
void enable_irq(byte);
void disable_irq(byte);

#endif
