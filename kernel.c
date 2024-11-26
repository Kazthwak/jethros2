#include "./c/headers.h"
#include "./c/files.h"

//THIS IS UNEEDED FOR ME (also vs code doesn't understand cross compilers)
//I HATE VS CODE
// /* Check if the compiler thinks you are targeting the wrong operating system. */
// #if defined(__linux__)
// #error "Cross compiler required: https://wiki.osdev.org/GCC_Cross-Compiler"
// #endif
 
// #if !defined(__i386__)
// #error "Cross compiler required: https://wiki.osdev.org/GCC_Cross-Compiler"
// #endif

static volatile struct disk_sector disk_sector1;


void kernel_main(){
string_serial("booted\n");
init();
string_serial("\n");
newline();

load_program("program.exe");
// hang();
__asm__ volatile("call 0x00");
print_string("\n\nTest\n");
//mem_debug();
// newline();
// hexdword(phys_page_state[(num_pages/32)-1]);
// hang();
// test_phys_pages();
hang();
Qshutdown();
}