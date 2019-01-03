/*--------------------------------------------------------------------------------------------------Stupid OS Project Copyright 2001-2002 3DMinus3
Started : Dec 08 2001   Updated : Dec 08 2001
File Name : assem.h     Purpose : C declarations for Assembly functions
--------------------------------------------------------------------------------------------------*/

#ifndef __idt_h__
#define __idt_h__

#include "types.h"

typedef struct idt_st_tag
{
	usint loffset;		// 2 bytes
	usint selector;		// 2 bytes	
	byte unused;		// 1 byte
	byte options;		
				/*	constant:5;	 5 bits
					 dpl:2;		 2 bits
					 present:1;	 1 bit		*/

	usint uoffset; 		// 2 bytes	
}idt_st_t;

typedef struct exceptiont_st_tag
{
	sint number;
	int (* handler)();
}exception_st_t;

typedef struct idtr_st_tag
{
	usint limit;
	usint lowbase;
	usint highbase;
}idtr_st_t;

#define CS_SELECTOR 0x08
#define IDTBASE 0x6800
#define IDT_LOWBASE 0x6800
#define IDT_HIGHBASE 0

#define INT_GATE 0x06     
#define TRAP_GATE 0x07

#define BITS_16 0x00
#define BITS_32 0x08

#define ABSENT 0x0
#define PRESENT 0x80

#define RING_0 0x00
#define RING_1 0x20
#define RING_2 0x40
#define RING_3 0x60


void load_idt_entry(int num,uint offset,usint selector,byte options);
void idt_init();
void load_idtr(idtr_st_t*);

#endif

