/*--------------------------------------------------------------------------------------------------Stupid OS Project Copyright 2001-2002 3DMinus3
Started : Dec 08 2001   Updated : Dec 08 2001
File Name : assem.h     Purpose : C declarations for Assembly functions
--------------------------------------------------------------------------------------------------*/

#ifndef __gdt_h__
#define __gdt_h__


#define GDT_BASE 0x6000
#define GDT_BASE_LOW 0x6000
#define GDT_BASE_HIGH 0x0000

#define GDO 0xC					//default value for gran_def_op


typedef enum present_tag
{
	ABSENT,
	PRESENT
}present_t;

typedef enum dpl_tag
{
	RING_0,
	RING_1,
	RING_2,
	RING_3
}dpl_t;

typedef enum seg_desc_tag
{
	SYSTEM,
	CODE_DATA
}seg_desc_t;

typedef enum seg_tag
{
	RES_1,
   	TSS_16A,
   	LDT,
       	TSS_16B,
   	CALL_GATE_16,
   	TASK_GATE,
   	INT_GATE_16,
   	TRAP_GATE_16,
   	RES_2,
   	TSS_32A,
   	RES_3,
   	TSS_32B,
   	CALL_GATE_32,
   	RES_4,
   	INT_GATE_32,
   	TRAP_GATE_32
}seg_t;


typedef struct gdt_str_tag
{
	unsigned short int seg_limit_low;       //segment limit 00:15
	unsigned short int base_low;		//Base Address 00:15
	unsigned char base_mid;			//Base Address 16:23
	seg_t seg_type:4;			//Segment Type (4 bits)
	seg_desc_t seg_desc_type:1;		//Descriptor type (1 bit, 0 = system, 1 = code/data)
	dpl_t dpl:2;				//Descriptor Privilege level (2 bits)
	present_t present:1;			//Segment present (1 bit)
	unsigned char seg_limit_high:4;		//segment limit 16:19 (4 bits)
	unsigned char gran_def_op:4;		//avl (1 bit)
						//reserved = 0 (1 bit)
						//D/B(default operation size / big) (1 bit)
						//Granularity (1 bit)
						//This is always 0xC in PM
	unsigned char base_high;		//Base Address 24:31
}gdt_st_t;

typedef struct gdtr_str_tag
{
	unsigned short int gdt_length;
	unsigned short int gdt_base_low;
	unsigned short int gdt_base_high;


}gdtr_st_t;

void gdt_init();
void make_gdt_entry(gdt_st_t*, int);
void load_gdtr();

#endif
 
