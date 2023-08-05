#include "headers.h"

void checks(){
return;
}

void err_fat(uint8_t err_code){
print_string("FATAL ERROR. ERROR CODE: ");
hexbyte(err_code);
print_string(" SEE MANUAL!");
hang();
}

void err_Nfat(uint8_t err_code){
switch(err_code){
case 0:
//print err message
break;
}
//still in low tech mode (i.e no proper keyboard or screen drivers)
//print wait message
//wait for keypress
}

void binbyte(uint8_t val){
for(uint8_t i = 0; i < 8; i++){
	putchar(0x30+((val>>i)&0x1));
}
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
cursorx = 0;
cursory++;
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