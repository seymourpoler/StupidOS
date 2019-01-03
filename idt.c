#include <idt.h>
#include <types.h>
#include <print.h>

void idt_init()
{
	int lv0 = 0;
	idtr_st_t idtr;
	
	extern exception_st_t exceptions[32];

	for(lv0 = 0; lv0 <= 31; lv0++)
	{
		load_idt_entry(exceptions[lv0].number,(uint)exceptions[lv0].handler,CS_SELECTOR,TRAP_GATE|PRESENT|RING_0|BITS_32);
			
	}

	idtr.limit = (usint) (34 * 8);
	idtr.lowbase = (usint) IDT_LOWBASE;
	idtr.highbase = (usint) IDT_HIGHBASE;

	load_idtr(&idtr);
	print("IDT Initialization\t\t\t\t[ DONE ]\n", 0xf);
}

void load_idt_entry(int num,uint offset,usint selector,byte options)
{
	
	idt_st_t* idt_ptr  = (idt_st_t *) IDTBASE  + num;

	usint loff ,uoff;    
        /*loff and hoff are used to store the lower and upper offsets respectively */
	loff = offset & 0x0000FFFF;
	uoff = (offset & 0xFFFF0000) >>16;
	
	idt_ptr ->loffset = loff;
	idt_ptr ->selector = selector;
	idt_ptr ->unused = 0x00;
	idt_ptr ->options = options;
	idt_ptr ->uoffset = uoff;
}

void load_idtr(idtr_st_t *idtr)
{
	asm("lidt (%0) ": :"p" (idtr));
}
	
