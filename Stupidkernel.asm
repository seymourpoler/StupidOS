;-------------------------------------------------------------------------------------------------
; Stupid OS Project Kernel Copyright 2001-2002 3DMinus3
; Started : Aug 03 2001 Updated : Dec 06 2001
;-------------------------------------------------------------------------------------------------
[ORG 0x7000]
[BITS 32]

PG1 equ 0x1007

	mov ax, 0x10			; Set ds, es, ss to data selector(base 0x0)
        mov ds, ax
        mov ax, 0x10
        mov es, ax
        mov ax, 0x10
        mov ss, ax
        mov esp, 0xFFFF			; Stack 0x0000:0xFFFF to 0x0000:0x0000

        mov esi, stupid_kernel_msg
        call stupid_pmode_print_string

        mov esi, stupid_pg_start_msg
        call stupid_pmode_print_string

page_setup_start:
	mov ecx, 1024*5
	mov eax, 0x0
	mov edi, 0x0
	cld
	rep stosd
	
	mov eax, 0x0
	mov dword [eax], PG1
	
	add eax, 4
	mov dword [eax], PG1 + 0x1000
	
	add eax, 4
	mov dword [eax], PG1 + 0x2000
	
	add eax, 4
	mov dword [eax], PG1 + 0x3000
	
	mov edi, 0x1000
	mov eax, 0x0007

	cld

fill_pte:
	stosd
	add eax, 0x1000
	cmp eax, 0x1000007
	jnz fill_pte

	mov eax, 0x0
	mov cr3, eax
	
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax

        mov esi, stupid_pg_over_msg
        call stupid_pmode_print_string
       
        mov esi, stupid_c_jump_msg
        call stupid_pmode_print_string
 
        push dword main_ret 		; If main returns goto to main_ret
 
        jmp 0x8:0x7400	

main_ret:
       	mov byte al, '*'
       	mov byte [es:0xb8A02], al

stupid_dummy:
       jmp stupid_dummy


;----------------------------------------------------------------------------
; function
;----------------------------------------------------------------------------

stupid_pmode_print_character:
; input al : character
;       ah : attribute

        pushad                          ; Save registers

        cmp al, 10                      ; Linefeed character is 10
        jnz stupid_not_line_feed

        add byte [stupid_yposition], 1
        jmp stupid_pmode_print_character_done

stupid_not_line_feed:
        cmp al, 13                      ; Carriage return character is 13
        jnz stupid_not_carriage_return

        mov byte [stupid_xposition], 0
        jmp stupid_pmode_print_character_done

stupid_not_carriage_return:
        mov ecx, eax                    ; save character and attribute

        mov ebx, 0
        mov bl, [stupid_xposition]
        shl bl, 1                       ; calculate x offset

        mov eax, 0
        mov al, [stupid_yposition]
        mov edx, 160
        mul edx                         ; calculate y offset

        mov edi, 0xb8000                ; start of video memory; modified by us to 0x0(0xb8000)
        add edi, eax                    ; add y offset
        add edi, ebx                    ; add x offset

        mov ax, cx                      ; restore character and attribute
        cld                             ; forward direction
        stosw                           ; write character and attribute

        add byte [stupid_xposition], 1

stupid_pmode_print_character_done:

        call stupid_hardware_move_cursor

        popad                           ; restore registers
        ret

stupid_pmode_print_string:
; input ds:esi = points to zero terminated string

        lodsb
        cmp al, 0
        jz stupid_pmode_print_string_done
        mov ah, 0x0F                    ; white text, black background
        call stupid_pmode_print_character
        jmp stupid_pmode_print_string

stupid_pmode_print_string_done:

        ret

stupid_hardware_move_cursor:

        pushad                          ; save registers

        mov ebx, 0
        mov bl, [stupid_xposition]             ; get x offset

        mov eax, 0
        mov al, [stupid_yposition]
        mov edx, 80
        mul edx                         ; calculate y offset

        add ebx, eax                    ; calculate index

        ; select to write low byte of index
        mov al, 0xf
        mov dx, 0x03d4
        out dx, al

        ; write it
        mov al, bl
        mov dx, 0x03d5
        out dx, al

        ; select to write high byte of index
        mov al, 0xe
        mov dx, 0x03d4
        out dx, al

        ; write it
        mov al, bh
        mov dx, 0x03d5
        out dx, al

        popad                           ; restore registers

        ret

;----------------------------------------------------------------------------
; data
;----------------------------------------------------------------------------

	stupid_xposition db 0
	stupid_yposition db 5
        stupid_kernel_msg db 'Protected mode test kernel loaded successfully', 13, 10, 0
	stupid_pg_start_msg db 'Setting up paging', 13, 10, 0
	stupid_pg_over_msg db 'Page set up over', 13, 10, 0
	stupid_c_jump_msg db 'Jumping to C code', 13, 10, 0

        times 1024-($-$$) db 0           ; padding
