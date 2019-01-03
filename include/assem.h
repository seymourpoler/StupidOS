/*--------------------------------------------------------------------------------------------------
Stupid OS Project Copyright 2001-2002 3DMinus3
Started : Dec 08 2001	Updated : Dec 08 2001
File Name : assem.h     Purpose : C declarations for Assembly functions
--------------------------------------------------------------------------------------------------*/

#ifndef __assem_h__
#define __assem_h__

#include "types.h"

void outb(const word port, const byte b);
void outw(const word port, const word w);
void outd(const word port, const dword d);

byte inb(const word port);
word inw(const word port);
dword ind(const word port);

#endif

