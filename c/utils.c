#include "headers.h"

void checks(){
//check for magic number
if(eax_boot != 0x2BADB002){err_Nfat(00);}
//check for flags
if((stateinfo.flags&req_multiboot_flags) != req_multiboot_flags){err_Nfat(01);}
return;
}

void err_fat(uint8_t err_code){
print_string("FATAL ERROR. ERROR CODE: ");
hexbyte(err_code);
print_string(" SEE MANUAL!");
hang();
}

void err_Nfat(uint8_t err_code){
print_string("NON-FATAL ERROR. ERROR CODE: ");
hexbyte(err_code);
print_string(" SEE MANUAL!");
//still in low tech mode (i.e no proper keyboard or screen drivers)
newline();
print_string("PRESS ANY KEY TO CONTINUE");
uint8_t key = bytein(0x60);
while(key == bytein(0x60)){}
key = bytein(0x60);
while(key == bytein(0x60)){}
//wait for keypress
}

void info(){
print_string("Welcome to JETHROS.\n");
print_string(version);
print_string("\nBOOTLOADER: ");
print_string((char*)stateinfo.boot_loader_name);
//video info
print_string("\nScreen resolution: 0x");
hexword(x_res);
print_string(" X 0x");
hexword(y_res);
print_string("\nCharacter resolution: 0x");
hexword(x_char_res);
print_string(" X 0x");
hexword(y_char_res);
//more info

newline();
}

void decint(uint16_t number){
for(int8_t i = 4; i >= 0; i--){
	uint8_t tmp = number/pow(10,i);
	putchar((tmp%10)+0x30);
}
}

void binbyte(uint8_t val){
for(int8_t i = 7; i >= 0; i--){
	putchar(0x30+((val>>i)&0x1));
}
}

void binword(uint16_t val){
binbyte(val>>8);
binbyte(val&0xff);
}

void bindword(uint32_t val){
binword(val>>16);
binword(val&0xffff);
}

void hexdig(uint8_t val){
val += 0x30;
if(val > 0x39){val += 0x07;}
putchar(val);
}

void hexbyte(uint8_t val){
hexdig(val>>4);
hexdig(val&0x0f);
}

void hexword(uint16_t val){
hexbyte(val>>8);
hexbyte(val&0xff);
}

void hexdword(uint32_t val){
hexword(val>>16);
hexword(val&0xffff);
}

void print_string(char* string){
while(*string != 0){
if(*string == 10){
newline();
}
else{
putchar(*string);
}
string++;
}
}


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