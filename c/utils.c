#include "headers.h"

void text_serial(){
text_out_type = 1;
}

void text_screen(){
text_out_type = 0;
}

void init(){

//init the gddt
string_serial("initialzing gdt\n");
gdt_init();


//copy the multiboot infostruct into a different one
string_serial("copying structs\n");
struct MultiBootInfoStruct* bootinfo = (struct MultiBootInfoStruct*)ebx_boot;
stateinfo = *bootinfo;

//copy the vbe mode infostruct into a different one
struct vbe_mode_info* minfo = (struct vbe_mode_info*)stateinfo.vbe_mode_info;
vbe_info = *minfo;

//copy the vbe control infostruct into a different one
struct vbe_control_info* cinfo = (struct vbe_control_info*)stateinfo.vbe_control_info;
vbe_control_info = *cinfo;


//initialize the graphics
string_serial("initing graphics\n");
graphics_init();

//install and initialize the idt
string_serial("initing idt\n");
install_idt();

//install and initialize the irq handlers
string_serial("initing irqs\n");
irq_init();

//check some things about the machine
string_serial("checking machine\n");
checks();

//display info about the machine and state
info();
}

void cmdinterpret(){
if(((stateinfo.flags>>2)&1) == 0){return;}
char* strptr = (char*)stateinfo.cmdline;
while(*strptr != 0){
	if(*strptr == '-'){
		switch(*(strptr+1)){
			case('i'):
			flags |= 1;
			break;
			case('s'):
			flags |= 0b10;
			break;
		}
	}
	strptr++;
}
}

//checks a variety of flags for fatal and non-fatal errors
void checks(){
if(flags&1){return;}
//check for magic number
if(eax_boot != 0x2BADB002){err_Nfat(00);}
//check for flags
if((stateinfo.flags&req_multiboot_flags) != req_multiboot_flags){err_Nfat(01);}
// err_Nfat(00);
return;
}

//error reporting for fatal errors
void err_fat(uint8_t err_code){
print_string("FATAL ERROR. ERROR CODE: ");
hexbyte(err_code);
print_string(" SEE MANUAL!");
string_serial("FATAL ERROR: ");
text_serial();
hexbyte(err_code);
hang();
}

//error reporting for non-fatal errors
void err_Nfat(uint8_t err_code){
print_string("NON-FATAL ERROR. ERROR CODE: ");
hexbyte(err_code);
print_string(" SEE MANUAL!");
string_serial("NON-FATAL ERROR: ");
text_serial();
hexbyte(err_code);
text_screen();

newline();
print_string("PRESS ANY KEY TO CONTINUE");
//wait for keypress
while(!is_key_waiting());
}

//prints out some info about the computer and boot process
void info(){
print_string("Welcome to JETHROS.\n");
print_string(version);
print_string("\nBOOTLOADER: ");
print_string((char*)stateinfo.boot_loader_name);
//video info
print_string("\nScreen resolution: ");
decint(x_res);
print_string(" X ");
decint(y_res);
print_string("\nCharacter resolution: ");
decint(x_char_res);
print_string(" X ");
decint(y_char_res);
print_string("\nFlags value: ");
bindword(stateinfo.flags);
if(stateinfo.mem_lower == 640){
	print_string("\nFull lower mem, upper mem: 0x");
	hexword(stateinfo.mem_upper);
	print_string(" KB");
}else{
	print_string("\nAvailable lower memory: 0x");
	hexword(stateinfo.mem_lower);
	print_string(" KB");
	if(stateinfo.mem_upper != 0){
		print_string("\nAvailable upper mem: 0x");
		hexword(stateinfo.mem_upper);
		print_string(" KB");
	}
text_serial();
decint(x_res);
putchar('\n');
decint(y_res);
putchar('\n');
bindword(stateinfo.flags);
putchar('\n');
hexword(stateinfo.mem_lower);
putchar('\n');
hexword(stateinfo.mem_upper);
text_screen();
}
//more info

newline();
}

//print a number(up to 99,999)
void decint(uint16_t number){
for(int8_t i = 4; i >= 0; i--){
	uint16_t tmp = number/pow(10,i);
	putchar((tmp%10)+0x30);
}
}

//prints out one byte in binary
void binbyte(uint8_t val){
for(int8_t i = 7; i >= 0; i--){
	putchar(0x30+((val>>i)&0x1));
}
}

//16 bits in binary
void binword(uint16_t val){
binbyte(val>>8);
binbyte(val&0xff);
}

//32 bits in binary
void bindword(uint32_t val){
binword(val>>16);
binword(val&0xffff);
}

//4 bits in hex
void hexdig(uint8_t val){
val += 0x30;
if(val > 0x39){val += 0x07;}
putchar(val);
}

//8 bits in hex
void hexbyte(uint8_t val){
hexdig(val>>4);
hexdig(val&0x0f);
}

//16 bits in hex
void hexword(uint16_t val){
hexbyte(val>>8);
hexbyte(val&0xff);
}

//32 bits in hex
void hexdword(uint32_t val){
hexword(val>>16);
hexword(val&0xffff);
}

//64 bits in hex
void hexqword(uint64_t val){
hexdword(val>>32);
hexdword(val&0xffffffff);
}

void string_serial(char* string){
text_serial();
print_string(string);
text_screen();
}

//prints a null terminated string (including newlines)
void print_string(char* string){
while(*string != 0){
putchar(*string);
string++;
}
}

//does a newline
void newline(){
#ifndef ramwatch
cursorx = 0;
cursory++;
#endif
#ifdef ramwatch
putchar('\\');
putchar('n');
#endif
}

//copy from start to dest
void memcpy(void* start, uint32_t length, void* dest){
	for(uint32_t i = 0; i < length; i++){
		*(uint8_t*)(i+dest) = *(uint8_t*)(i+start);
	}
}

//set length bytes after base to val
void memset(uint32_t base, uint8_t val, uint32_t length){
for(uint32_t i = 3; i < length; i++){
uint8_t* j = (uint8_t*)i+base+1;
*j = val;
}
}

//execute the function at address funcaddr
void arbitraryfunc(uint32_t funcaddr){
((void(*)(void))funcaddr)();
}