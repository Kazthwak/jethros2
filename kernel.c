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
gdt_init();
struct MultiBootInfoStruct* bootinfo = (struct MultiBootInfoStruct*)ebx_boot;
stateinfo = *bootinfo;
struct vbe_mode_info* minfo = (struct vbe_mode_info*)stateinfo.vbe_mode_info;
vbe_info = *minfo;
struct vbe_control_info* cinfo = (struct vbe_control_info*)stateinfo.vbe_control_info;
vbe_control_info = *cinfo;
checks();
graphics_init();
info();
newline();
draw_rect(0, 0, 1024, 768, 0x00f0f0);
// arbitraryfunc(isr0);
install_idt();
irq_init();
intt0();
hang();
Qshutdown();
}