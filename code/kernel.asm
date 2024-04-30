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
;call the c kernel
cli
call kernel_main
;return back to the prievious assembly file
ret

global inton
inton:
	sti
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
	; al is the char code
	;ah is the top byte of the char
	mov ebx, keypressed ;ebx has the address
	and al, 0x7f ;ignore the top bit
	mov edx, eax ;because one cannot create the sum of multiple values that differ in size
	and edx, 0xff;get rid of everything except al, probably unecassary
	add ebx, edx ;add them together
	xor ah, 1 ;reverse ah, so a release turns to a 0 and a press becomes a 1
	;update the array index
	mov [ebx], ah 

	.real_end:
	popa
	ret

global flush_tss
flush_tss:
	mov ax, 0x28 | 0 ;gdt entry bit shifted then or'ed with the privelidge level
	ltr ax
	ret




global test_program_end
global test_program
test_program:
int 48
int 0
jmp $
mov ax, 0
mov dx, 0
div dl ;int 0
test_program_end:

%include "./code/isr.asm"

;initialized read-write data-
SECTION .data
eax_boot:
db "aaa",0
ebx_boot:
db "aaa",0
user_stack:
times(1024) db 0

;read only data
SECTION .RODATA
