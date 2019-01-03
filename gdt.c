/*-------------------------------------------------------------------------------------------------
 * Stupid OS Project GDT Copyright 2001-2002 3DMinus3
 * Started : Dec 07 2001 Updated : Dec 07 2001
 *-------------------------------------------------------------------------------------------------*/

#include <gdt.h>
#include <print.h>

void gdt_init()
{
	gdt_st_t gdt_temp;
	int lv0;

	gdt_temp.seg_limit_low 		= 0x0FFF;	//Code Descriptor	
	gdt_temp.base_low 		= 0x0000;
	gdt_temp.base_mid 		= 0x00;
	gdt_temp.seg_type		= 0xA;
	gdt_temp.seg_desc_type 		= CODE_DATA;
	gdt_temp.dpl 			= RING_0;
	gdt_temp.present 		= PRESENT;
	gdt_temp.seg_limit_high 	= 0x0;
	gdt_temp.gran_def_op 		= GDO;
	gdt_temp.base_high 		= 0x00;

	make_gdt_entry(&gdt_temp, 1);			//Make Second entry (Code descriptor)
	
	gdt_temp.seg_limit_low 		= 0x0FFF;	//Data Descriptor
	gdt_temp.base_low 		= 0x0000;
	gdt_temp.base_mid 		= 0x00;
	gdt_temp.seg_type		= 0x2;
	gdt_temp.seg_desc_type 		= CODE_DATA;
	gdt_temp.dpl 			= RING_0;
	gdt_temp.present 		= PRESENT;
	gdt_temp.seg_limit_high 	= 0x0;
	gdt_temp.gran_def_op 		= GDO;
	gdt_temp.base_high 		= 0x00;

	make_gdt_entry(&gdt_temp, 2);			//Make Third entry (Data descriptor)

	gdt_temp.seg_limit_low 		= 0;		//Null descriptor
	gdt_temp.base_low 		= 0;
	gdt_temp.base_mid 		= 0;
	gdt_temp.seg_type		= 0;
	gdt_temp.seg_desc_type 		= 0;
	gdt_temp.dpl 			= 0;
	gdt_temp.present 		= 0;
	gdt_temp.seg_limit_high 	= 0;
	gdt_temp.gran_def_op 		= 0;
	gdt_temp.base_high 		= 0;

	make_gdt_entry(&gdt_temp, 0);			//Make first entry (Null descriptor)
	
	for(lv0 = 3; lv0 < 256; lv0++)			//Make other 253 entries empty
		make_gdt_entry(&gdt_temp, lv0);

	load_gdtr();
	asm("mov $0x0010, %ax");
        asm("mov %ax, %ds");
        asm("mov %ax, %ss");
        asm("mov %ax, %es");
        asm("mov %ax, %gs");
        asm("mov %ax, %fs");
	
	print("GDT Initialization\t\t\t\t[ DONE ]\n", 0xf);
	

}

void make_gdt_entry(gdt_st_t *gdt_new, int gdt_num)
{
	gdt_st_t *gdt_entry = (gdt_st_t*) GDT_BASE + gdt_num;
	
	gdt_entry -> seg_limit_low 	= gdt_new -> seg_limit_low;
	gdt_entry -> base_low 		= gdt_new -> base_low;
	gdt_entry -> base_mid 		= gdt_new -> base_mid;
	gdt_entry -> seg_type		= gdt_new -> seg_type;
	gdt_entry -> seg_desc_type 	= gdt_new -> seg_desc_type;
	gdt_entry -> dpl 		= gdt_new -> dpl;
	gdt_entry -> present 		= gdt_new -> present;
	gdt_entry -> seg_limit_high 	= gdt_new -> seg_limit_high;
	gdt_entry -> gran_def_op 	= gdt_new -> gran_def_op;
	gdt_entry -> base_high 		= gdt_new -> base_high;
}

void load_gdtr()
{
	gdtr_st_t gdtr;
	
	gdtr.gdt_length = (unsigned short int) 256 * 8;
	gdtr.gdt_base_low = (unsigned short int) GDT_BASE_LOW;
	gdtr.gdt_base_high = (unsigned short int) GDT_BASE_HIGH;

	asm("lgdt (%0)": :"p" (&gdtr));
}

	
