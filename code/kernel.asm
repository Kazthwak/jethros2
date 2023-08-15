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

extern Qshutdown
hang:
cli
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
	int 0
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


%include "./code/isr.asm"

;initialized read-write data-
SECTION .data
eax_boot:
db "aaa",0
ebx_boot:
db "aaa",0


;read only data
SECTION .RODATA