#include "headers.h"

#define user_start 0

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

struct tss_t{
    uint16_t   link;
    uint16_t   link_h;

    uint32_t   esp0;
    uint16_t   ss0;
    uint16_t   ss0_h;

    uint32_t   esp1;
    uint16_t   ss1;
    uint16_t   ss1_h;

    uint32_t   esp2;
    uint16_t   ss2;
    uint16_t   ss2_h;

    uint32_t   cr3;
    uint32_t   eip;
    uint32_t   eflags;

    uint32_t   eax;
    uint32_t   ecx;
    uint32_t   edx;
    uint32_t    ebx;

    uint32_t   esp;
    uint32_t   ebp;

    uint32_t   esi;
    uint32_t   edi;

    uint16_t   es;
    uint16_t   es_h;

    uint16_t   cs;
    uint16_t   cs_h;

    uint16_t   ss;
    uint16_t   ss_h;

    uint16_t   ds;
    uint16_t   ds_h;

    uint16_t   fs;
    uint16_t   fs_h;

    uint16_t   gs;
    uint16_t   gs_h;

    uint16_t   ldt;
    uint16_t   ldt_h;

    uint16_t   trap;
    uint16_t   iomap;
}__attribute__((packed));

//define the objects
//a code and a data segment for the kernel, and the same for userspace
//kernel segments will access the whole memory
// user will access all except the first 4 mb and the video mem
#define gdt_len 6
struct gdt_entry gdt[gdt_len];

struct gdt_ptr gdtr;

struct tss_t tss;

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
	memset((uint32_t)&gdt, 0, sizeof(gdt));
	//null descriptor
	gdt_set_entry(0, 0, 0, 0, 0);
	//kernel code	0-GB, kernel below 4M
	gdt_set_entry(1, 0x0, 0xffffffff, kernel_code_access, kernel_code_gran);
	//kernel data	^^^^^^^^^^^^^^^^^^^^^
	gdt_set_entry(2, 0x0, 0xffffffff, kernel_data_access, kernel_data_gran);
	//user code		4M-4G
	gdt_set_entry(3, user_start, 0xffffffff, user_code_access, user_code_gran);
	//user data		^^^^^
	gdt_set_entry(4, user_start, 0xffffffff, user_data_access, user_data_gran);
	tss_init();
	gdtr.base = (uint32_t)&gdt;
	gdtr.limit = (8*gdt_len)-1;
	gdtr_load();
	gdt_load();
	//MUST BE DONE AFTER INSTALLING GDT *facepalm emoticon*
	flush_tss();
}

void tss_init(){
	memset((uint32_t)&tss, 0, sizeof(tss));
	//set up tss gdt segment
	gdt_set_entry(5, (uint32_t)&tss, sizeof(tss), 0x89, 0x0);
	tss.ss0 = 0x10;
	tss.iomap = sizeof(tss);
	tss.esp0 = 32768;
}