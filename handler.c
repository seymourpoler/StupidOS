#include <handler.h>
#include <idt.h>
#include <print.h>
#include <pic.h>
#include <assem.h>

int divide_error()
{
	print("divide error exception\n", 0xF);
	while(1);
	return 0;
}

int debug()
{ 
	print("debug exception\n", 0xF);

	while(1);
	return 0;
}

int nmi_interrupt()
{
	print("nmi interrupt exception\n", 0xF);

	while(1);
	return 0;
}

int breakpoint()
{
	print("breakpoint exception\n", 0xF);

	while(1);
	return 0;
}
int overflow()
{
	print("overflow exception\n", 0xF);

	while(1);
	return 0;
}

int bound_range_exceeded()
{
	print("bound range exceeded exception\n", 0xF);

	while(1);
	return 0;
}

int invalid_opcode()
{
	print("invalid opcode exception\n", 0xF);

	while(1);
	return 0;
}

int device_not_available()
{
	print("device not available exception\n", 0xF);

	while(1);
	return 0;
}

int double_fault()
{
	print("double fault exception\n", 0xF);

	while(1);
	return 0;
}
int coprocessor_segment_overrun()
{
	print("coprocessor segment overrun exception\n", 0xF);

	while(1);
	return 0;
}

int invalid_tss()
{
	print("invalid tss exception\n", 0xF);

	while(1);
	return 0;
}

int segment_not_present()
{
	print("segment not present exception\n", 0xF);

	while(1);
	return 0;
}

int stack_fault()
{
	print("stack fault exception\n", 0xF);

	while(1);
	return 0;
}

int general_protection()
{	
	print("general protection exception\n", 0xF);
	outb(MASTER_PORT_B, 0xFF);

	while(1);
	return 0;
}

int page_fault()
{
	print("page fault exception\n", 0xF);

	while(1);
	return 0;
}

int floating_point_error()
{
	print("floating point error exception\n", 0xF);

	while(1);
	return 0;
}

int alignment_check()
{
	print("alignment check exception\n", 0xF);

	while(1);
	return 0;
}

int machine_check()
{
	print("machine check exception\n", 0xF);
	return 0;
}

int simd_floating_point()
{
	print("simd floating point exception\n", 0xF);
	return 0;
}

int reserved()
{
	print("reserved exception\n", 0xF);
	return 0;
}

exception_st_t exceptions[32]=
{
        {0, &divide_error},
        {1, &debug},
        {2, &nmi_interrupt},
        {3, &breakpoint},
        {4, &overflow},
        {5, &bound_range_exceeded},
        {6, &invalid_opcode},
        {7, &device_not_available},
        {8, &double_fault},
        {9, &coprocessor_segment_overrun},
        {10, &invalid_tss},
        {11, &segment_not_present},
        {12, &stack_fault},
        {13, &general_protection},
        {14, &page_fault},
        {15, &floating_point_error},
        {16, &alignment_check},
        {17, &machine_check},
        {18, &simd_floating_point},
        {19, &reserved},
        {20, &reserved},
        {21, &reserved},
        {22, &reserved},
        {23, &reserved},
        {24, &reserved},
        {25, &reserved},
        {26, &reserved},
        {27, &reserved},
        {28, &reserved},
        {29, &reserved},
        {30, &reserved},
        {31, &reserved}
};


