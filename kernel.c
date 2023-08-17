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
init();
newline();
draw_rect(0, 0, 1024, 768, 0x00f0f0);
// for(volatile uint32_t i = 0; i < 0xfffffff; i++);
print_string("test");
newline();
// while(true){
// 	if(is_key_waiting()){
// 		putchar(get_key_buffer());
// 	}
// }
hang();
Qshutdown();
}