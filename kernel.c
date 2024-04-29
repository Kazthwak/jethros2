#include "./c/headers.h"
#include "./c/files.h"

//THIS IS UNEEDED FOR ME (MAINLY BC MY IDE REFUSES TO ACKNOWLEDGE THAT __linux__ IS NOT DEFINED or that __i386__ is defined)
//I HATE VS CODE
// /* Check if the compiler thinks you are targeting the wrong operating system. */
// #if defined(__linux__)
// #error "Cross compiler required: https://wiki.osdev.org/GCC_Cross-Compiler"
// #endif
 
// #if !defined(__i386__)
// #error "Cross compiler required: https://wiki.osdev.org/GCC_Cross-Compiler"
// #endif


void kernel_main(){
string_serial("booted\n");
init();
string_serial("\n");
newline();
/*
uint16_t curd = cursorx;
uint16_t cury = cursory;
while(1){
	cursorx = curd;
	uint16_t count = 0;
	while(!is_key_waiting()){count++; hexword(count); cursorx = curd;}
	newline();
	hexbyte(scancode(get_key_buffer()));
	cursory = cury;
}
//*/
port_wiz();
hang();
Qshutdown();
}