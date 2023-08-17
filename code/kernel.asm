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
	int 33
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
	.end:
	popa
	ret
	.noteq:
	;check the 
	mov ebx, [keybufferloc]
	sub ebx, keybuffer
	cmp ebx, 0xff
	jge .end
	mov ebx, [keybufferloc]
	mov [ebx], al
	inc dword [keybufferloc]
	jmp .end





%include "./code/isr.asm"

;initialized read-write data-
SECTION .data
eax_boot:
db "aaa",0
ebx_boot:
db "aaa",0


;read only data
SECTION .RODATA