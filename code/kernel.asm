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
;cli
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

string:
db "Testing the routine", 0x0a ,0x00

global intt0
intt0:
	pusha
	mov eax, 0xfffffffe
	int 0x30
	popa
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

global load_page_directory
load_page_directory:
	push ebp
	mov ebp, esp
	mov eax, [esp+8]
	mov cr3, eax
	mov esp, ebp
	pop ebp
	ret

global enable_paging
enable_paging:
	push ebp
	mov ebp, esp
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	mov esp, ebp
	pop ebp
	ret

extern timerflags
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
	
	;open uber debugger in an emergency
	cmp al, 0x01
	jne .not_debugger
	mov [timerflags], dword 1
	.not_debugger:
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
	pusha

	;clear screen
	mov eax, 2
	int 48

	;get character
	.lop:
		mov ebx, tmpstr
		mov eax, 0x11
		int 48
		cmp byte [tmpstr], 0x00
		je test_program.lop
	
	;output character
	mov eax, 0
	mov ebx, tmpstr
	int 48

	;newline
	mov eax, 0x01
	mov ebx, 1
	mov ecx, 1
	int 48

	popa
	ret

tmpstr:
db "a", 0

test_program_end:

global test
test:
pusha
int 0
popa
ret

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
