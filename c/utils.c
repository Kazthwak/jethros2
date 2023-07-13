#include "headers.h"

void hexdig(uint8_t val){
val += 0x30;
if(val > 0x39){val += 0x07;}
putchar(val);
}

void hexbyte(uint8_t val){
hexdig(val>>4);
hexdig(val&0x0f);
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