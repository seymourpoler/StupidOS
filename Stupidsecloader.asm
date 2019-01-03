;-----------------------------------------------------------------------------------------------------------
; Stupid OS Project Secondary Loader Copyright 2001-2002 3DMinus3 
; Started : Aug 03 2001 Updated : Dec 06 2001
;-----------------------------------------------------------------------------------------------------------
stupid_sec_loader_entry:
	mov ax,0x0200			; Set ds to reflect 
	mov ds,ax			; Current address

	mov ax, 0xb800
	mov fs, ax			; Set fs to point to base of video memory

stupid_a20_enable:
        cli                             ; Disable interrupts

        mov bl, 0xd0                    ; Make available current status
        call stupid_kbd_write_cmd

        call stupid_kbd_read_data	; Read the current status
        or al, 2                        ; Set the a20 enable bit
        push ax				; Save it in the stack

        mov bl, 0xd1                    ; Make the port wait for new status
        call stupid_kbd_write_cmd	

        pop bx				; Fill bx with the saved ax value	
        call stupid_kbd_write_data      ; Write the new status

        mov bl, 0xd0                    ; Make available current status
        call stupid_kbd_write_cmd

        call stupid_kbd_read_data       ; Read the current status
        and al, 2			; Check whether a20 line is enabled

        sti                             ; Enable interrupts

        jnz stupid_a20_ok		; 'al' is not zero if a20 is set

	mov si, stupid_a20_error_str
	call stupid_display_str
	jmp stupid_a20_enable

stupid_a20_ok:

	mov ax, 0x8000
        mov es, ax
        mov di, 0x0                     ; "es:(e)di" -> 0x0000:0x8000 Destination location

        mov si, stupid_gdt_null         ; Source

        mov cx, 48                      ; Length of GDT(in decimals)
        cld                             ; Move in forward direction
        rep movsb                       ; Move GDT to its new location

        lgdt [stupid_gdtr]              ; Load gdt register

; Now read from the floppy, sectors starting from 4 as per need and load it into memory.
; Code in the sector 4 is 32 bit. Before passing control to sector 4, we switch to protected
; mode.
	
	mov ax, 0x0700
	mov es, ax
	mov bx, 0x0
		
	mov ax, 0x0020
	mov word [no_of_sec], ax

	mov ax, 0x4 
	mov word [dos_sec], ax

	mov si, stupid_load_str
	call stupid_display_str

	call read_floppy

	mov cx, 0xFFFF

floppy_off:
	mov dx,0x3f2
	mov al,0x0
	out dx,al
        loop floppy_off
	
	mov si, stupid_a20_ok_str
	call stupid_display_str

	mov si, stupid_temp_gdt_load_str
	call stupid_display_str

	mov si, stupid_disable_ints_str
	call stupid_display_str

	mov si, stupid_ent_pm_str
	call stupid_display_str
	
; disable ALL interrupts

        cli                             ; disable interrupts

        mov al, 11111111b               ; select to mask of all irq's
        out 0x21, al                    ; write it to the pic controller

        in al, 0x70                     ; read a value
        or al, 10000000b                ; set the nmi disable bit
        out 0x70, al                    ; write it back again

; enter protected mode

        mov eax, cr0
        or al, 1                        ; set protected mode bit
        mov cr0, eax


; transfer control to kernel

	jmp 0x8:0x7000                  ; transfer control to test kernel	

;-------------------------------------------------------------------------------
;Callback functions
;-------------------------------------------------------------------------------

read_floppy:
	mov ax, [no_of_sec]
	cmp ax, 0x0
	jz read_over
	call calc_all_phy
	call stupid_read_sectors
	mov ax, [no_of_sec]
	dec ax
	mov word [no_of_sec], ax
	mov ax, [dos_sec]
	inc ax
	mov word [dos_sec], ax
	jmp read_floppy
	
read_over:
	ret

calc_all_phy:
	
				;formula used to calculate phy_sec, phy_head, phy_track from dos_sector				
				;physical_sector = 1 + (dos_sector  MOD  sectors_per_track)
				;physical_head = (dos_sector / sectors_per_track)  MOD  num_heads
				;physical_track = dos_sector / (sectors_per_track * num_heads)
	push ax
	push cx
	push dx
	
	mov ax, [dos_sec]	;dos sector ranges from 0 to n-1
	mov dx, 0x0		;divide dos_sector by 0x12(18) 
	mov cx, 0x12		;Remainder + 1 gives the phy_sector
	div cx	
	
	inc dx	
		
	mov byte [phy_sec], dl
	
	mov dx, 0x0		;Quotient from previous divison is divided by 0x2(2)
	mov cx, 0x2		;Remainder gives the phy_head
	div cx
	
	mov byte [phy_head], dl
	
	mov ax, [dos_sec]	;dos_sector is divided by 0x24(18 * 2)
	mov dx, 0x0
	mov cx, 0x24		;Quotient gives the phy_track
	div cx

	mov byte [phy_track], al
	
	pop dx
	pop cx
	pop ax
	ret

stupid_reset_drive:
	mov ah, 0                       ; reset drive function
        int 0x13                        ; call bios disk i/o
	jc stupid_reset_drive
	ret

stupid_read_sectors:
; input : es:bx = address of destination
;         al = sector count
;         cl = sector start number

        call stupid_reset_drive
	mov ah, 0x02
	mov al, 0x01
	mov cl, [phy_sec]
        mov ch, [phy_track]
        mov dl, 0
        mov dh, [phy_head]
        int 0x13 
	jc stupid_read_retry
	add bx, 0x200
	mov si, stupid_dot_str
	call stupid_display_str
	ret

stupid_read_retry:
	call stupid_reset_drive
	jmp stupid_read_sectors
	
stupid_display_str:
; input : ds:si points to zero terminated string

        cld
        lodsb
        cmp al, 0
        jz stupid_return
        mov ah, 0x0E
        int 0x10
        jmp stupid_display_str

stupid_return:
	ret

stupid_write_char:
	push bx	
	mov bx, [stupid_cur_pos]
	mov byte [fs:bx], al
	add bx, 0x2
	mov [stupid_cur_pos], bx
	pop bx
	ret

stupid_wait_keypress:
        mov ah, 0                       ; Read keypress function
        int 0x16                        ; Call bios keyboard services
        ret

stupid_kbd_wait_cmd:
        in al, 0x64                     ; Read the controller status port
        and al, 2                       ; Check if the command port is ready -> Second bit set
        jnz stupid_kbd_wait_cmd         ; If not ready loop back
        ret                             ; If ready return back

stupid_kbd_wait_data:
        in al, 0x64                     ; Read the controller status port
        and al, 1                       ; Check if the data port is ready -> First bit set
        jz stupid_kbd_wait_data		; If not ready loop back
        ret				; If ready return back

stupid_kbd_write_cmd:
; input : bl = command

        call stupid_kbd_wait_cmd        ; Call to wait for ready status
        mov al, bl
        out 0x64, al                    ; Send the command to the command port
        ret

stupid_kbd_read_data:
; output : al = data

        call stupid_kbd_wait_data       ; Call to wait for ready status
        in al, 0x60                     ; Read data from data port
        ret

stupid_kbd_write_data:
; input bl = data

        call stupid_kbd_wait_cmd        ; Call to wait for ready status
        mov al, bl
        out 0x60, al                    ; Write data to data port
        ret

;-----------------------------------------------------------------
;Global Descriptor Table
;-----------------------------------------------------------------

stupid_gdtr:
	dw stupid_gdt_end - stupid_gdt_null - 1	; GDT size or limit
	dd 0x80000				; Physical address of GDT. We give the value
						; because we relocate the GDT to 0x800 manually
						; above

stupid_gdt_null:
;NULL descriptor -> Necessary (0x0)
	dw 0
	dw 0
	db 0
	db 0
	db 0
	db 0

stupid_gdt_system_code: 
;System code segment descriptor (0x8)
	dw 0x0FFF
	dw 0x0000
	db 0
	db 0x9A
	db 0xC0
	db 0

stupid_system_data:
;System data and stack segment descriptor (0x10)
	dw 0x0FFF
	dw 0
	db 0
	db 0x92
	db 0xC0
	db 0

stupid_gdt_end:

;----------------------------------------------------------------------------
; Strings
;----------------------------------------------------------------------------

	stupid_cur_pos dw 800
	stupid_a20_error_str db 'A20 Init Error...', 13, 10, 0
	stupid_a20_ok_str db 'OK', 13, 10, 'A20 Initialised...', 13, 10, 0
	stupid_disable_ints_str db 'Disabling Interrupts...', 13, 10, 0
	stupid_temp_gdt_load_str db 'Loading Temporary GDT...', 13, 10, 0
	stupid_ent_pm_str db 'Entering PM...', 13, 10, 0
	stupid_load_str db 'Stupid OS Loading', 0
	stupid_dot_str db '.', 0

	dos_sec dw 0x0
	no_of_sec dw 0x0
	phy_sec db 0x0 
	phy_head db 0x0
	phy_track db 0x0

;----------------------------------------------------------------------------
; Padding
;----------------------------------------------------------------------------

        times 1536-($-$$) db 0           ; Padding with zeroes
