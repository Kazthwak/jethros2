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
struct vbe_mode_info* minfo = (struct vbe_mode_info*)stateinfo.vbe_mode_info;
vbe_info = *minfo;
struct vbe_control_info* cinfo = (struct vbe_control_info*)stateinfo.vbe_control_info;
vbe_control_info = *cinfo;
graphics_init();
checks();
graphics_init();
info();
newline();
decint(65535);
newline();
hexword(65535/100);
hang();
Qshutdown();
}