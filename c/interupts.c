#include "headers.h"

//irq

void* irqs[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void install_irq_handle(uint8_t irq_num, void(*handler)(struct regs* r)){
	irqs[irq_num] = handler;
}

void uninstall_irq(uint8_t irq_num){
	irqs[irq_num] = 0;
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

    // Find out if we have a custom handler to run for this
    //  IRQ, and then finally, run it
    handler = irqs[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

	// If the IDT entry that was invoked was greater than 40
    // (meaning IRQ8 - 15), then we need to send an EOI to
    // the slave controller
    if (r->int_no >= 40)
    {
        byteout(0xA0, 0x20);
    }

    // In either case, we need to send an EOI to the master
    // interrupt controller too
    byteout(0x20, 0x20);
}
		
void irq_init(){
pic_remap();
}


//isr

void double_fault(){
print_string("FATAL ERROR, DOUBLE FAULT");
__asm__("cli; hlt;");
}

void fault_handler(struct regs* r){
if(r->int_no == 8){double_fault();}
hang();
}