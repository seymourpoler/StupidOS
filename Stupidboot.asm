;----------------------------------------------------------------------------------------------------
;Stupid OS Project Boot Code Copyright 2001-2002 3DMinus3 
;Started : Jul 24 2001 Updated : Aug 03 2001
;----------------------------------------------------------------------------------------------------

; To assemble: nasm stupid.asm -f bin -o stupid.bin
; To create boot disk: debug stupid.bin
; -w 100 0 0 1
; -q

[BITS 16]
[ORG 0]


        jmp short stupid_start
        nop                             ; required nop

;----------------------------------------------------------------------------
;Bios parameter block
;----------------------------------------------------------------------------

        OEM						db 'StupidOS'	 ; 8 bytes
        BytesPerSector          dw 0x200	 ; 512 bytes per sector
        SectorsPerCluster       db 1		 ; Single sector per cluster
        ReservedSectors         dw 1		 ; Reserved sector(s) -> Boot Sector
        TotalFATs	        	db 2		 ; 2 FATs present -> Dunno where the second one is
        MaxRootDirectoryEntries dw 0x00E0	 ; 224 root directory entries
        TotalSectors            dw 0x0B40	 ; 2880 sectors in total
        MediaDescriptor         db 0xF0		 ; 1.44M floppy
        SectorsPerFAT           dw 0x0009	 ; 9 sectors per FAT
        SectorsPerTrack         dw 0x0012	 ; 18 sectors per track
        NumberOfHeads           dw 0x0002	 ; 2 heads
        HiddenSectors           dd 0		 ; Dunno?
        TotalSectorsLarge       dd 0		 ; Dunno?
        DriveNumber             db 0		 ; 0 -> Floppy drive I
        Flags                   db 0		 ; Think this is reserved	
        Signature               db 0x29		 ; Dunno?
        VolumeID                dd 0
        VolumeName              db 'STUPID     ' ; Volume name
        FileSystemType          db 'FAT12   '	 ; File system is FAT12	

;----------------------------------------------------------------------------
;Actual booting code
;----------------------------------------------------------------------------

stupid_start:

        cli                             ; Disable interrupts

        mov ax, 0x07C0
        mov ds, ax                      ; Setup data segment

		mov ax, 0xb800			
		mov fs, ax                      ; To access video memory

        mov ax, 0x9000
        mov ss, ax                      ; Setup a stack
        mov sp, 0x1FFF                  ; 0x9000:0x0000 to 0x9000:0x1FFF
        				; Spanning 8K
	sti				; Enable interrupts again
        
        mov [stupid_bootdrive], dl      ; Save boot drive	
	
; Code below is written to relocate the loaded code and data coz we'll load the kernel at the 
; bottom of the memory. So we want that area of the memory to be clear else we'll overwrite 
; some code and data that we need.
        
stupid_relocate: 

		mov ax,0x8000
		mov es,ax
		mov di,0			; "es:(e)di" -> 0x8000:0x0000 Destination location
		
		mov si,0			; "ds:(e)si" -> 0x07C0:0x0000 Start here

        mov cx, 0x200                   ; Length of boot code is 512 bytes
        cld                             ; Move in forward direction
        rep movsb                       ; Move the 512 bytes of boot code
	
	jmp 0x8000:stupid_relocated
	
stupid_relocated:

; When the system boots the bios area starts at 0x0040:0x0000. Since we'll load the 
; kernel at the bottom of the memory we have to relocate the bios data area to a 
; safer position. We'll move it to 0x7000:0x0000

        mov ax, 0x7000			
        mov es, ax
        mov di, 0                       ; "es:(e)di" -> 0x7000:0x0000 Destination location

        mov ax, 0x0040					; Bios data area starts here
        mov ds, ax
        mov si, 0                       ; "ds:(e)si" -> 0x0040:0x0000 Start here

        mov cx, 256                     ; Length of bios data area is 256 bytes
        cld                             ; Move in forward direction
        rep movsb                       ; Move 256 bytes of bios data area

        mov ax, 0x8000
        mov ds, ax                      ; Data segment is made to point the new location

		call stupid_clear_screen
		call stupid_reset_drive			; Reset floppy drive
		
stupid_drive_ok:	
	mov ax,0x0200
	mov es,ax
	mov bx,0x0					; Read from floppy to 0x0200:0x0000
	
	mov al,0x3					; No of sectors to read	
	mov cl,0x2					; Starting sector
	
	call stupid_read_n_sectors	; Read the no of sectors specified above

	mov byte al, 'S'
	call stupid_write_char	


	jmp 0x0200:0x0000			; Jump to secondary loader (from sector 1)



;----------------------------------------------------------------------------
; Functions
;----------------------------------------------------------------------------

stupid_clear_screen:
        mov al, 3                       ; Select video mode 3 - color text
        mov ah, 0                       ; Set video mode function
        int 0x10                        ; Clear display
        ret

; After int 13h (Reset Drive) CF will be clear if interrupt is successful else CF
; is set. Any error in int 13h (Reset Drive) will cause CF to be set

stupid_reset_drive:
	push ax				; Store previous value of ax

stupid_reset_retry:
	mov byte al, 'R'
	call stupid_write_char

        mov ah, 0                       ; Set ah to zero to reset drive
        int 0x13                        ; Disk - Reset drive interrupt
	
	jc stupid_reset_retry		; If not reset try again
	mov byte al, 'O'
	call stupid_write_char
	pop ax				; If done load ax from stack
        ret

; After int 13h (Read Sector(s)) CF will be clear if interrupt is successful else CF
; is set. Any error in int 13h (Reset Drive) will cause CF to be set

stupid_read_sectors:
; input : es:bx = address of destination
;         al = sector count
;         cl = sector start number

        push ax
		mov byte al, 'A'
		call stupid_write_char
		pop ax

		mov ah, 0x02                    ; Set ah to 2 to read sector(s) into memory
		mov al, 0x01
        mov ch, 0                       ; 'ch' contains the cylinder/track number
        mov dl, [stupid_bootdrive]      ; 'dl' contains the drive number
        mov dh, 0                       ; 'dh' contains the head number
        int 0x13                        ; Disk - Read sector interrupt
		jc stupid_drive_read_retry		; Drive may fail in the first attempt. So try again
										; till it gets loaded, from scratch
		push ax
		mov byte al, 'S'
		call stupid_write_char
		pop ax
		ret								; Success

stupid_drive_read_retry:
	call stupid_reset_drive		; Reset the drive again before reading again
	jmp stupid_read_sectors		; Read the sector required
	
stupid_read_n_sectors:
;	input : al = Number of Sectors to read
;		cl = Starting sector Number
;		es:bx address of Destination

stupid_next_sector:
	cmp al, 0					; Check if all sectors read
	je stupid_read_over			; If yes return
	call stupid_reset_drive		; Else reset drive	
	push ax						; Push remaining sectors to be read to stack
	mov al, 0x1					; Let sectors to be read be one
	call stupid_read_sectors	; Read one sector starting from from cl
	pop ax						; Pop sectors to be read
	inc cl						; Move to next sector
	dec al						; Decrement sectors to read by one
	add bx, 0x200				; Increment the memory pointer
	jmp stupid_next_sector		; Read next sector

stupid_read_over:
	push ax						; Print N to indicate read over
	mov byte al, 'N'
	call stupid_write_char
	pop ax
	ret							; return 
        
stupid_write_char:
	push bx						; Store current value of bx
	mov bx, [stupid_cur_pos]	; Set bx to cursor position
	mov byte [fs:bx], al		; Copy the character to be printer to
								; 0xB800:bx
	add bx, 0x2					; Increment cursor position
	mov [stupid_cur_pos], bx	; Store the value in memory
	pop bx						; Load old value of bx
	ret

;----------------------------------------------------------------------------
; Data
;----------------------------------------------------------------------------

        stupid_bootdrive db 0
	stupid_cur_pos dw 0
        	
;----------------------------------------------------------------------------
; Boot Signature
;----------------------------------------------------------------------------

        times 510-($-$$) db 0           ; Padding
        dw 0xAA55                       ; Boot signature
