/*--------------------------------------------------------------------------------------------------
Stupid OS Project Copyright 2001-2002 3DMinus3
Started : Dec 10 2001	Updated : Dec 12 2001
File Name : pic.c       Purpose : Functions for initializing PIC(8259)
--------------------------------------------------------------------------------------------------*/

#include <assem.h>
#include <pic.h>
#include <print.h>

static byte irq_status_master = 0;
static byte irq_status_slave = 0;
void pic_init()
{
	outb(MASTER_PORT_A, 0x11);		// ICW 1
	outb(SLAVE_PORT_A, 0x11);		// 7 | 6 | 5 | 4 | 3  | 2 | 1  |
						// 0 | 0 | 0 | 1 |Trig|M/S|ICW4|
						// Master Slave Configuration
						// IC4 needed                            

	outb(MASTER_PORT_B, 0x20);		// ICW 2
	outb(SLAVE_PORT_B, 0x28);		// INT 0x20-0x27 mapped to IRQ 0 to IRQ 7
						// INT 0x28-0x2F mapped to IRQ 8 to IRQ 15
	
	outb(MASTER_PORT_B, 0x04);   		 // ICW 3 
	outb(SLAVE_PORT_B, 0x02);      		// IR 2 of master is connected to slave pic
						// IRQ on master this slave is connected to
	
	outb(MASTER_PORT_B, 0x01);		// ICW 4
	outb(SLAVE_PORT_B, 0x01);		// Master, Fully Nested Mode
						// Slave, Fully Nested Mode
	
	outb(MASTER_PORT_B, 0xFF);		// All interrupts except IRQ 0 are disabled
	outb(SLAVE_PORT_B, 0xFF);
	asm("sti");
	print("PIC Initialization\t\t\t\t[ DONE ]\n", 0xf);
}

void enable_irq(byte irq_no)
{
	byte temp = 0x01;

	if (irq_no >15)
		return;

	if ( irq_no < 8)
	{
		temp = temp << (irq_no );
		irq_status_master |= temp;
		
		outb(MASTER_PORT_B,(byte)~irq_status_master);
	}
	else
	{
		irq_no -= 8;
		temp = temp <<(irq_no);
		irq_status_slave |= temp;
		outb(SLAVE_PORT_B,(byte)~irq_status_slave);
		irq_status_master |= 0x04;
		outb(MASTER_PORT_B,(byte)~irq_status_master);
	}
}
void disable_irq(byte irq_no)
{
        byte temp = 0x01;

        if (irq_no >15)
                return;

        if ( irq_no < 8)
        {
                temp = temp << (irq_no );
                irq_status_master &= ~temp;

                outb(MASTER_PORT_B,(byte)~irq_status_master);
        }
        else
        {
                irq_no -= 8;
                temp = temp <<(irq_no);
                irq_status_slave &= ~temp;
                outb(SLAVE_PORT_B,(byte)~irq_status_slave);
                irq_status_master &= ~0x04;
                outb(MASTER_PORT_B,(byte)~irq_status_master);
        }
}

