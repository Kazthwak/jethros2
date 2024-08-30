#include "headers.h"

//irq

void* irqs[224];

//from brans tutorial, but extended to handle all interupts



void install_irq_handle(uint8_t irq_num, void(*handler)(struct regs* r)){
	irqs[irq_num] = handler;
}

void uninstall_irq(uint8_t irq_num){
	irqs[irq_num] = 0;
}

void test_func(struct regs* r){
fired++;
for(uint8_t i = 0; i < 16; i++){
for(uint8_t j = 0; j < 16; j++){
putpixel(i,j,0xff0000);
}}
r++;
}

void pic_remap(){
	byteout(0x20, 0x11);
	byteout(0xA0, 0x11);
	byteout(0x21, 0x20);
	byteout(0xA1, 0x28);
	byteout(0x21, 0x04);
	byteout(0xA1, 0x02);
	byteout(0x21, 0x01);
	byteout(0xA1, 0x01);
	byteout(0x21, 0x0);
	byteout(0xA1, 0x0);
}

void irq_handler(struct regs *r){
	// This is a blank function pointer
	void (*handler)(struct regs *r);
	text_serial();
	// print_string("IRQ 0x");
	// hexdword(r->int_no);
	// Find out if we have a custom handler to run for this
	//  IRQ, and then finally, run it
	handler = irqs[r->int_no - 32];
	if (handler){
		handler(r);
	}

	// If the IDT entry that was invoked was greater than 40
	// (meaning IRQ8 - 15), then we need to send an EOI to
	// the slave controller
	// but only if it is a pic interupt
	if(r->int_no < 48){
	
	if (r->int_no >= 40){
		byteout(0xA0, 0x20);
	}

	// In either case, we need to send an EOI to the master
	// interrupt controller too
	byteout(0x20, 0x20);
	}
text_screen();	
}
		
void irq_init(){
memset((uint32_t)&keypressed, 0, sizeof(keypressed));
memset((uint32_t)&irqs, 0, sizeof(irqs));
pic_remap();
for(uint8_t i = 0; i <16; i++){IRQ_clear_mask(i);}
timer_phase(timer_hz);
// IRQ_set_mask(1);
// IRQ_set_mask(0);
install_irq_handle(1, keyboard_int);
install_irq_handle(0, timer_handle);
install_irq_handle(16, test_func);
inton();
}


//isr

void double_fault(){
print_string("FATAL ERROR, DOUBLE FAULT");
__asm__("cli; hlt;");
}

void fault_handler(struct regs* r){
text_serial();
print_string("EXCEPTION NO 0x");
hexdword(r->int_no);
byteout(0x3f8, '\n');
bindword(r->err_code);
uint32_t addr;
asm volatile("mov %%cr2 , %%eax" : "=eax" (addr) :);
byteout(0x3f8, '\n');
hexdword(addr);
if(r->int_no == 8){double_fault();}
hang();
}

void IRQ_set_mask(unsigned char IRQline){
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        IRQline -= 8;
    }
    value = bytein(port) | (1 << IRQline);
    byteout(port, value);        
}
 
 //osdev wiki
void IRQ_clear_mask(unsigned char IRQline){
    uint16_t port;
    uint8_t value;
 
    if(IRQline < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        IRQline -= 8;
    }
    value = bytein(port) & ~(1 << IRQline);
    byteout(port, value);        
}