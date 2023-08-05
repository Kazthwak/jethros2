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
struct MultiBootInfoStruct* bootinfo = (struct MultiBootInfoStruct*)ebx_boot;
stateinfo = *bootinfo;
struct vbe_control_info* cinfo = stateinfo.vbe_control_info;
vbe_info = *cinfo;
print_string(cinfo);
// print_string("testing");
// newline();
// setpixel(100, 10, 0x3);
// uint32_t* videomem = 0xa0000;
// *videomem = 0xffffffff;
hexdword(stateinfo.vbe_control_info);
hang();
Qshutdown();
}