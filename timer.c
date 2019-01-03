/*---------------------------------------------------------------------------------------------------
Stupid OS Project Copyright 2001-2002 3DMinus3
Started : Dec 09 2001	Updated : Dec 09 2001
File Name : timer.c     Purpose : PIT 8253(8254) Initialization
---------------------------------------------------------------------------------------------------*/

#include <assem.h>
#include <timer.h>
#include <print.h>
#include <idt.h>
#include <types.h>
#include <pic.h>

void timer_handler();

void timer_init()
{
//	static ulong count = PIT_FREQUENCY / INTERVAL;
	static usint count = 1193182 / 60; 

	outb(PIT_MODE_PORT, 0x36);	// Counter 0, LSB and MSB, Mode 3, Binary
					// Mode 3 is Square Wave(SW) generator
					// Count is decremented by 2 for every clock period
					// Upper half of SW constitutes N/2 clock pulses and
					// Lower half constitutes another N/2 for a count N

	outb(PIT_COUNTER_0_PORT, (byte) count & 0xFF);		// Pass the LSB first
	outb(PIT_COUNTER_0_PORT, (byte) count >> 8);		// Followed by the MSB           


         load_idt_entry(32,(uint)&timer_handler,CS_SELECTOR,TRAP_GATE|PRESENT|RING_0|BITS_32);
	 enable_irq(TIMER);
	 print("Timer Initialization\t\t\t\t[ DONE ]\n", 0xf);

}

void timer_handler()
{
	static int i = 0;
	i++;
	if(i == 5000)
	{
	        print(".",0x0F);
		i = 0;
	}
        outb(0x20,0x20);
        asm("mov %ebp,%esp");
        asm("pop %ebp");
        asm("iret");
}


