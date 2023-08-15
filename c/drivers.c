//shutsdown the computer, but only on qemu
__attribute__((noreturn))
void Qshutdown(){
wordout(0x604, 0x2000);
__builtin_unreachable();
}

//takes a byte from port
int bytein(uint32_t port){
unsigned char result;
__asm__("push %ax\n\t"
"push %dx\n\t");
__asm__("in %% dx , %% al" : "=a" (result):"d"((unsigned short)port));
__asm__("pop %dx\n\t"
"pop %ax\n\t");
return((unsigned short)result);
}

//gives a byte to port
void byteout(uint32_t port, uint8_t data){
// " a " ( data ) means : load EAX with data
// " d " ( port ) means : load EDX with port
__asm__("out %% al, %% dx" : : "a" (data), "d" ((unsigned short)port));
}

// takes a word from port
int wordin(uint32_t port){
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
#ifndef ramwatch
putcharxyc(cursorx, cursory, letter);
#endif
#ifdef ramwatch
char* video_mem_base = (char*)0xa0000;
uint32_t video_offset = (cursorx+(80*cursory));
*(video_mem_base+video_offset) = letter;
#endif
cursorx++;
if(cursorx >= 80){
cursorx = 0;
cursory++;
}
}