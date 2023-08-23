#include "headers.h"

//idt

struct idt_entry{
uint16_t addr_low;
uint16_t gdt_selector;
uint8_t reserved;
uint8_t flags;
uint16_t addr_high;
}__attribute__((packed));

struct idt_ptr{
uint16_t len;
uint32_t base;
}__attribute__((packed));


		

struct idt_ptr idtr;

#define idt_len 256
struct idt_entry idt_table[idt_len];

//functions
void set_idt_entry(uint8_t entry, uint32_t addr, uint16_t gdt_selector, uint8_t flags){
	idt_table[entry].addr_low = addr&0xffff;
	idt_table[entry].addr_high = addr>>16;

	idt_table[entry].gdt_selector = gdt_selector;
	idt_table[entry].reserved = 0;
	idt_table[entry].flags = flags;
}


void install_idt(){
	memset((uint32_t)&idt_table, 0, sizeof(idt_table));
	idtr.len = (8*idt_len)-1;
	idtr.base = (uint32_t)&idt_table;

	set_idt_entry(0, (uint32_t)isr0, 0x08, 0x8e);
	set_idt_entry(1, (uint32_t)isr1, 0x08, 0x8e);
	set_idt_entry(2, (uint32_t)isr2, 0x08, 0x8e);
	set_idt_entry(3, (uint32_t)isr3, 0x08, 0x8e);
	set_idt_entry(4, (uint32_t)isr4, 0x08, 0x8e);
	set_idt_entry(5, (uint32_t)isr5, 0x08, 0x8e);
	set_idt_entry(6, (uint32_t)isr6, 0x08, 0x8e);
	set_idt_entry(7, (uint32_t)isr7, 0x08, 0x8e);
	set_idt_entry(8, (uint32_t)isr8, 0x08, 0x8e);
	set_idt_entry(9, (uint32_t)isr9, 0x08, 0x8e);
	set_idt_entry(10, (uint32_t)isr10, 0x08, 0x8e);
	set_idt_entry(11, (uint32_t)isr11, 0x08, 0x8e);
	set_idt_entry(12, (uint32_t)isr12, 0x08, 0x8e);
	set_idt_entry(13, (uint32_t)isr13, 0x08, 0x8e);
	set_idt_entry(14, (uint32_t)isr14, 0x08, 0x8e);
	set_idt_entry(15, (uint32_t)isr15, 0x08, 0x8e);
	set_idt_entry(16, (uint32_t)isr16, 0x08, 0x8e);
	set_idt_entry(17, (uint32_t)isr17, 0x08, 0x8e);
	set_idt_entry(18, (uint32_t)isr18, 0x08, 0x8e);
	set_idt_entry(19, (uint32_t)isr19, 0x08, 0x8e);
	set_idt_entry(20, (uint32_t)isr20, 0x08, 0x8e);
	set_idt_entry(21, (uint32_t)isr21, 0x08, 0x8e);
	set_idt_entry(22, (uint32_t)isr22, 0x08, 0x8e);
	set_idt_entry(23, (uint32_t)isr23, 0x08, 0x8e);
	set_idt_entry(24, (uint32_t)isr24, 0x08, 0x8e);
	set_idt_entry(25, (uint32_t)isr25, 0x08, 0x8e);
	set_idt_entry(26, (uint32_t)isr26, 0x08, 0x8e);
	set_idt_entry(27, (uint32_t)isr27, 0x08, 0x8e);
	set_idt_entry(28, (uint32_t)isr28, 0x08, 0x8e);
	set_idt_entry(29, (uint32_t)isr29, 0x08, 0x8e);
	set_idt_entry(30, (uint32_t)isr30, 0x08, 0x8e);
	set_idt_entry(31, (uint32_t)isr31, 0x08, 0x8e);


	set_idt_entry(32, (uint32_t)irq0, 0x08, 0x8e);
	set_idt_entry(33, (uint32_t)irq1, 0x08, 0x8e);
	set_idt_entry(34, (uint32_t)irq2, 0x08, 0x8e);
	set_idt_entry(35, (uint32_t)irq3, 0x08, 0x8e);
	set_idt_entry(36, (uint32_t)irq4, 0x08, 0x8e);
	set_idt_entry(37, (uint32_t)irq5, 0x08, 0x8e);
	set_idt_entry(38, (uint32_t)irq6, 0x08, 0x8e);
	set_idt_entry(39, (uint32_t)irq7, 0x08, 0x8e);
	set_idt_entry(40, (uint32_t)irq8, 0x08, 0x8e);
	set_idt_entry(41, (uint32_t)irq9, 0x08, 0x8e);
	set_idt_entry(42, (uint32_t)irq10, 0x08, 0x8e);
	set_idt_entry(43, (uint32_t)irq11, 0x08, 0x8e);
	set_idt_entry(44, (uint32_t)irq12, 0x08, 0x8e);
	set_idt_entry(45, (uint32_t)irq13, 0x08, 0x8e);
	set_idt_entry(46, (uint32_t)irq14, 0x08, 0x8e);
	set_idt_entry(47, (uint32_t)irq15, 0x08, 0x8e);
	set_idt_entry(48, (uint32_t)irq16, 0x08, 0x8e);



	idtr_load();

	// hexdword(((uint32_t)isr0)&0xffff);
	// newline();
	// hexdword(idt_table[0].addr_low);
	// newline();
	// hexdword((uint32_t)isr0);
	// newline();
	// hexdword((uint32_t)&idt_table[0]);
	// newline();
	// hexdword(idtr.base);
	// // hang();

}


//pic
