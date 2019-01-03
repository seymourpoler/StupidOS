/*--------------------------------------------------------------------------------------------------
Stupid OS Project Copyright 2001-2002 3DMinus3
Started : Dec 09 2001	Updated : Dec 09 2001
File Name : timer.h     Purpose : Declarations for PIT 8253(8254) initialization
--------------------------------------------------------------------------------------------------*/

#ifndef __timer_h__
#define __timer_h__
	
#include <types.h>

#define PIT_COUNTER_0_PORT 0x40
#define PIT_MODE_PORT 0x43
#define PIT_FREQUENCY (uint)1193182	// Given as 1.193 MHz in Ralph Brown Ports List
					// Used 1193182 as suggested by Christopher Giese
#define INTERVAL (usint)100		// Frequency is used. Equivalent to 10ms

void timer_init();

#endif

