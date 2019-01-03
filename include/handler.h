/*--------------------------------------------------------------------------------------------------Stupid OS Project Copyright 2001-2002 3DMinus3
Started : Dec 08 2001   Updated : Dec 08 2001
File Name : assem.h     Purpose : C declarations for Assembly functions
--------------------------------------------------------------------------------------------------*/

#ifndef __handler_h__
#define __handler_h__

int divide_error();
int debug();
int nmi_interrupt();
int breakpoint();
int overflow();
int bound_range_exceeded();
int invalid_opcode();
int device_not_available();
int double_fault();
int coprocessor_segment_overrun();
int invalid_tss();
int segment_not_present();
int stack_fault();
int general_protection();
int page_fault();
int floating_point_error();
int alignment_check();
int machine_check();
int simd_floating_point();
int reserved();

#endif 
