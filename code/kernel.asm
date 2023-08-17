;I prefer nasm syntax, so this is a seperate assembly file with that syntax


;code section
SECTION .text
;section to initialize anything that I have to
global kernel_init
global hang
global eax_boot
global ebx_boot
kernel_init:



;declare that the c kernel exists
extern kernel_main
; call hang
;call the c kernel
call kernel_main
;return back to the prievious assembly file
ret

global inton
inton:
	sti
	ret

extern Qshutdown
hang:
hlt
jmp hang

global gdtr_load
extern gdtr
gdtr_load:
	lgdt[gdtr]
	ret

global idtr_load
extern idtr
idtr_load:
	lidt[idtr]
	ret


global intt0
intt0:
	int 32
	ret

global gdt_load
gdt_load:
	jmp 0x08:.reloaded
	.reloaded:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret

extern keybuffer
extern keybufferloc
extern keypressed
global keyboard_int
keyboard_int:
	pusha
	;take the keyboard input
	mov dx, 0x60
	in al, dx
	;copy it to ah
	mov ah, al
	;shift it to the right 7 times
	shr ah, 7
	;check if it is 0
	cmp ah, 0
	;but then not actuallu check
	jmp .noteq
	jmp .end
	.noteq:
	;check the 
	mov ebx, [keybufferloc]
	sub ebx, keybuffer
	cmp ebx, 0xff
	jge .end
	mov ebx, [keybufferloc]
	mov [ebx], al
	inc dword [keybufferloc]
	.end:
	;DOES NOT WORK SO SKIP
	jmp .real_end

	;al needs to be preserved
	;check which sase to go to
	cmp ah, 0
	;now ah is 1 for press and 0 for unpress
	xor ah, 1
	;shift it to the correct bit placement
	mov bl, al
	;take cl remainder 8
	and bl, 0b111
	; shl ah, bl
	;ebx is the pointer to the byte
	mov ebx, keypressed
	;divide the char code by 8
	shr al, 3
	;add on the aarray index offset
	; add ebx, al
	;ebx is the address
	;ah is 0s with a 1 in the correct bit  
	je .unpress
	.press:
	or byte [ebx], ah
	jmp .real_end
	.unpress:
	xor ah, 0xff
	and [ebx], ah
	jmp .real_end
	.real_end:
	popa
	ret





%include "./code/isr.asm"

;initialized read-write data-
SECTION .data
eax_boot:
db "aaa",0
ebx_boot:
db "aaa",0


;read only data
SECTION .RODATA