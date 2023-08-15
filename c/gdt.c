#include "headers.h"

//define the structs
struct gdt_entry{
//low half of limit
uint16_t limit;
//low half of base
uint16_t base_low;
//second highest byte of base
uint8_t base_middle;
//access flags
uint8_t access;
//granularity flag
uint8_t granularity;
//highest byte of base
uint8_t base_high;
}__attribute__((packed));


struct gdt_ptr{
uint16_t limit;
uint32_t base;
}__attribute__((packed));

//define the objects
//a code and a data segment for the kernel, and the same for userspace
//kernel segments will access the whole memory
// user will access all except the first 4 mb and the video mem
#define gdt_len 5
struct gdt_entry gdt[gdt_len];

struct gdt_ptr gdtr;

//allow 4MB for kernel

//funcs

//thank you bran
void gdt_set_entry(uint8_t entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity){
	gdt[entry].base_low = base&0xffff;
	gdt[entry].base_middle = (base>>16)&0xff;
	gdt[entry].base_high = (base>>24)&0xff;

	gdt[entry].limit = limit&0xffff;

	gdt[entry].access = access;

	gdt[entry].granularity = ((limit >> 16)&0x0f);
	gdt[entry].granularity |= (granularity & 0xf0);
}



#define kernel_code_access 0x9A
#define kernel_code_gran 0xCF

#define kernel_data_access 0x92
#define kernel_data_gran 0xCF


#define user_code_access 0xFA
#define user_code_gran 0xCF

#define user_data_access 0xFA
#define user_data_gran 0xCF




void gdt_init(){
	//null descriptor
	gdt_set_entry(0, 0, 0, 0, 0);
	//kernel code	0-GB, kernel below 4M
	gdt_set_entry(1, 0x0, 0xffffffff, kernel_code_access, kernel_code_gran);
	//kernel data	^^^^^^^^^^^^^^^^^^^^^
	gdt_set_entry(2, 0x0, 0xffffffff, kernel_data_access, kernel_data_gran);
	//user code		4M-4G
	gdt_set_entry(3, 0x400, 0xffffffff, user_code_access, user_code_gran);
	//user data		^^^^^
	gdt_set_entry(4, 0x400, 0xffffffff, user_data_access, user_data_gran);
	gdtr.base = (uint32_t)&gdt;
	gdtr.limit = (8*gdt_len)-1;
	gdtr_load();
	gdt_load();
}