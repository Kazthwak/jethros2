;I prefer nasm syntax, so this is a seperate assembly file with that syntax


;code section
SECTION .text
;section to initialize anything that I have to
global kernel_init
kernel_init:



;declare that the c kernel exists
extern kernel_main
;call the c kernel
call kernel_main
;return back to the prievious assembly file
ret

global hang
hang:
cli
hlt
jmp hang


;initialized read-write data
SECTION .data

;read only data
SECTION .RODATA