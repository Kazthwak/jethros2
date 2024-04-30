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

void test(void){
	byteout(0x1f6, 0XE0);
	wait_tick(1);
	byteout(0x1f1, 0x0);
	wait_tick(1);
	byteout(0x1f2, 1);
	wait_tick(1);
	byteout(0x1f3, 0);
	wait_tick(1);
	byteout(0x1f4, 0);
	wait_tick(1);
	byteout(0x1f5, 0);
	wait_tick(1);
	byteout(0x1f7, 0x20);
	print_string("ASD");
	while(1){
		wait_for_enter();
		hexword(wordin(0x1f0));
	}
}

void kernel_main(){
string_serial("booted\n");
init();
string_serial("\n");
newline();
port_wiz();
test();
hang();
Qshutdown();
}