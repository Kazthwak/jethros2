#include "./c/headers.h"
#include "./c/files.h"

//THIS IS UNEEDED FOR ME (MAINLY BC MY IDE REFUSES TO ACKNOWLEDGE THAT __linux__ IS NOT DEFINED)
// /* Check if the compiler thinks you are targeting the wrong operating system. */
// #if defined(__linux__)
// #error "Cross compiler required: https://wiki.osdev.org/GCC_Cross-Compiler"
// #endif
 
#if !defined(__i386__)
#error "Cross compiler required: https://wiki.osdev.org/GCC_Cross-Compiler"
#endif

void kernel_main(){
string_serial("booted\n");
init();
while(true){
// 	if(is_key_waiting()){
// 		putchar(get_key_buffer());
// 	}
	cursorx = 0;
	hexqword(time);
	putchar(' ');
	hexbyte(keypressed[2]);
}
hang();
Qshutdown();
}