#include "headers.h"

//shutsdown the computer, but only on qemu
__attribute__((noreturn))
void Qshutdown(){
wordout(0x604, 0x2000);
__builtin_unreachable();
}

//takes a byte from port
/*
uint8_t bytein(uint32_t port){
uint8_t result;
__asm__("in %% dx , %% al" : "=a" (result):"d"((unsigned short)port));
return(result);
}*/

uint8_t bytein(uint16_t port){
	uint8_t ret;
	asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}


//gives a byte to port
void byteout(uint32_t port, uint8_t data){
// " a " ( data ) means : load EAX with data
// " d " ( port ) means : load EDX with port
__asm__("out %% al, %% dx" : : "a" (data), "d" ((unsigned short)port));
}

// takes a word from port
uint16_t wordin(uint32_t port){
unsigned short result;
__asm__("in %% dx , %% ax" : "=a" (result) : "d" ((unsigned short)port));
return result;
}

//gives a word to port
void wordout(uint32_t port, uint16_t data){
__asm__("out %% ax , %% dx" : : "a" (data), "d" ((unsigned short)port));
}

//for looking at ram instead of text output (mainly for graphics debugging)
// #define ramwatch

//prints one char to the cursor location, then increments the cursor
void putchar(char letter){
if(text_out_type == 0){
	if(letter == 0xa){newline(); return;}
putcharxyc(cursorx, cursory, letter);
}else if(text_out_type == 1){
	byteout(0x3f8, letter);
	return;
}
cursorx++;
if(cursorx >= x_char_res){
cursorx = 0;
cursory++;
}
}