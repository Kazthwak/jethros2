%macro glob 1
global isr%1
%endmacro

glob 0
glob 1
glob 2
glob 3
glob 4
glob 5
glob 6
glob 7
glob 8
glob 9
glob 10
glob 11
glob 12
glob 13
glob 14
glob 15
glob 16
glob 17
glob 18
glob 19
glob 20
glob 21
glob 22
glob 23
glob 24
glob 25
glob 26
glob 27
glob 28
glob 29
glob 30
glob 31

%macro isrnoerrc 1
isr%1:
	cli
	push byte 0
	push byte %1
	jmp isr_common_stub
%endmacro

%macro isriserrc 1
isr%1:
	cli
	push byte %1
	jmp isr_common_stub
%endmacro

isrnoerrc 0
isrnoerrc 1
isrnoerrc 2
isrnoerrc 3
isrnoerrc 4
isrnoerrc 5
isrnoerrc 6
isrnoerrc 7
;double fault
isriserrc 8
isrnoerrc 9
isriserrc 10
isriserrc 11
isriserrc 12
isriserrc 13
isriserrc 14
isrnoerrc 15
isrnoerrc 16
isriserrc 17
isrnoerrc 18
isrnoerrc 19
isrnoerrc 20
isriserrc 21
isrnoerrc 22
isrnoerrc 23
isrnoerrc 24
isrnoerrc 25
isrnoerrc 26
isrnoerrc 27
isrnoerrc 28
isriserrc 29
isriserrc 30
isrnoerrc 31



;thanks bran
extern fault_handler
isr_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10   ; Load the Kernel Data Segment descriptor!
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp   ; Push us the stack
    push eax
    mov eax, fault_handler
    call eax       ; A special call, preserves the 'eip' register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!



%macro irqn 1
global irq%1
irq%1:
	cli
	push byte 0
	push byte (%1+32)
	jmp irq_common_stub 
%endmacro

irqn 0
irqn 1
irqn 2
irqn 3
irqn 4
irqn 5
irqn 6
irqn 7
irqn 8
irqn 9
irqn 10
irqn 11
irqn 12
irqn 13
irqn 14
irqn 15


extern irq_handler

; This is a stub that we have created for IRQ based ISRs. This calls
; '_irq_handler' in our C code. We need to create this in an 'irq.c'
irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, irq_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret