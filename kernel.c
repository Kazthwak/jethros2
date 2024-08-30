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
while(0){
uint16_t curd = cursorx;
hexqword(time);
cursorx = curd;
}
text_serial();
hexdword(vbe_info.phys_addr);
hang();
mem_debug();
hexdword((uint32_t)page_directory);
newline();
hexdword((uint32_t)page_tables[0]);
newline();
hexdword(((uint32_t*)page_tables[1])[0]);
// newline();
// hexdword(phys_page_state[(num_pages/32)-1]);
// hang();
// test_phys_pages();
hang();
Qshutdown();
}