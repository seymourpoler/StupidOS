ASM = nasm 
CC = gcc
INCLUDE = include
DD = dd 

SRC = 	main.c\
	gdt.c\
	idt.c\
	disp.c\
	time.c\
	timer.c\
	handler.c\
	cmos.c\
	pic.c\
	keyboard.c

OBJ =	main.o\
	gdt.o\
	assem.o\
	idt.o\
	disp.o\
	time.o\
	timer.o\
	handler.o\
	cmos.o\
	pic.o\
	keyboard.o
	
CFLAGS = -c -Wall -I ${INCLUDE}

all: 	boot second kernel assem bin
	 
clean:
	rm -f *.o
	rm -f *.bin 
	
boot:		
	${ASM} -fbin -o Stupidboot.bin Stupidboot.asm

second:
	${ASM} -fbin -o Stupidsecloader.bin Stupidsecloader.asm

kernel:		
	${ASM} -fbin -o Stupidkernel.bin Stupidkernel.asm

assem:		
	${ASM} -felf -o assem.o assem.asm

bin:		
	${CC} ${CFLAGS} ${SRC}
	ld -oformat binary -emain -Ttext 0x7400 -omain.bin ${OBJ} 

asm:	boot second kernel assem

boot_image:
	make
	cat Stupidboot.bin Stupidsecloader.bin Stupidkernel.bin main.bin > STUPID
	${DD} if=STUPID bs=512 skip=0 seek=0 of=/dev/fd0

