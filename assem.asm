;----------------------------------------------------------------------------------------------------
;Stupid OS Project <Program Name> Copyright 2001-2002 3DMinus3 
;Started : Dec 08 2001		Updated : Dec 10 2001
;----------------------------------------------------------------------------------------------------
[BITS 32]

GLOBAL outb
GLOBAL outw
GLOBAL outd

GLOBAL inb
GLOBAL inw
GLOBAL ind

%define port [ebp + 8]
%define data [ebp + 12]

ALIGN 4
outb:
	push ebp
	mov ebp, esp
	push eax
	push edx
	mov edx, port
	mov eax, data
	out dx, al
	pop edx
	pop eax
	leave
	ret

ALIGN 4
outw:
	push ebp
	mov ebp, esp
	push eax
	push edx
	mov edx, port
	mov eax, data
	out dx, ax
	pop edx
	pop eax
	leave
	ret

ALIGN 4
outd:
	push ebp
	mov ebp, esp
	push eax
	push edx
	mov edx, port
	mov eax, data
	out dx, eax
	pop edx
	pop eax
	leave
	ret

ALIGN 4
inb:
	push ebp
	mov ebp, esp
	push edx
	mov edx, port
	xor eax, eax
	in al, dx
	pop edx
	leave
	ret	

ALIGN 4
inw:
	push ebp
	mov ebp, esp
	push edx
	mov edx, port
	xor eax, eax
	in ax, dx
	pop edx
	leave
	ret	

ALIGN 4
ind:
	push ebp
	mov ebp, esp
	push edx
	mov edx, port
	xor eax, eax
	in eax, dx
	pop edx
	leave
	ret	
