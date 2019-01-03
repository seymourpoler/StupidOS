/*--------------------------------------------------------------------------------------------------
Stupid OS Project Copyright 2001-2002 3DMinus3
Started : Dec 08 2001	Updated : Dec 10 2001
File Name : cmos.c      Purpose : Functions to access CMOS
--------------------------------------------------------------------------------------------------*/

#include <types.h>
#include <assem.h>
#include <cmos.h>
#include <print.h>

int cmos_read(byte address)
{
	while(cmos_busy());
	outb(CMOS_OUT_PORT, 0x80|address);		// Use 0x80 coz we need to disable NMI
	return bcd2bin(inb(CMOS_IN_PORT));
}

boolean cmos_busy()
{
	outb(CMOS_OUT_PORT, 0x80|STATUS_A);
	if(inb(CMOS_IN_PORT) & 0x80)			// Bit 7 is set in Status Register A if Clock is busy
		return TRUE;
	else
		return FALSE;
}

int bcd2bin(byte bcd)
{
	return((bcd & 0xF) + ((bcd >> 4) * 10));
}

//BCD to BIN Math
//Its easy to explain with an example
//Input : bin = 25 BCD
//		  bin = 0010 0101
//		  bin & 0xF = 0000 0101          -> (a)
//		  bin >> 4 = 0000 0010
//		  (bin >> 4) * 10 = 0001 0100    -> (b)
//		  (a) + (b) = 0001 1001 -> This is 25 in BINARY

