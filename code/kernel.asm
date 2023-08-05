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

hang:
cli
hlt
jmp hang


;initialized read-write data
SECTION .data
eax_boot:
db "aaa",0
ebx_boot:
db "aaa",0


;read only data
SECTION .RODATA