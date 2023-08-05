#ifndef kernel_header_included
#define kernel_header_included

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct MultiBootInfoStruct{
	uint32_t flags;
	uint32_t mem_lower;
	uint32_t mem_upper;
	uint32_t boot_device;
	uint32_t cmdline;
	uint32_t mods_count;
	uint32_t mods_addr;
	uint32_t syms_1;
	uint32_t syms_2;
	uint32_t syms_3;
	uint32_t syms_4;
	uint32_t mmap_length;
	uint32_t mmap_addr;
	uint32_t drives_length;
	uint32_t drives_addr;
	uint32_t config_table;
	uint32_t boot_loader_name;
	uint32_t apm_table;
	//func 00
	uint32_t vbe_control_info;
	//func 01
	uint32_t vbe_mode_info;
	uint32_t vbe_mode;
	uint32_t vbe_interface_seg;
	uint32_t vbe_interface_off;
	uint32_t vbe_interface_len;
	uint32_t framebuffer_addr;
	uint32_t framebuffer_pitch;
	uint32_t framebuffer_width;
	uint32_t framebuffer_height;
	uint32_t framebuffer_bpp;
	uint32_t framebuffer_type;
	uint32_t colour_info;
}__attribute__((packed));

struct vbe_control_info{
	char vesa_string[5];
	uint16_t mode_attributes;
	uint8_t window_a_attributes;
	uint8_t window_b_attributes;
	uint32_t window_granularity;
	uint32_t window_size;
	uint32_t window_a_segment;
	uint32_t window_b_segment;
	uint32_t window_function_p;
	uint16_t bytes_scanline;
	uint16_t x_res;
	uint16_t y_res;
	uint8_t char_x;
	uint8_t char_y;
	uint8_t num_planes;
	uint8_t bits_per_pixel;
}__attribute__((packed));



static struct vbe_control_info vbe_info;
static struct MultiBootInfoStruct stateinfo;
static uint16_t cursorx = 0;
static uint16_t cursory = 0;

void checks();
void err_fat(uint8_t err_code);
void err_Nfat(uint8_t err_code);
void binbyte(uint8_t val);
void hexdword(uint32_t val);
void hexword(uint16_t val);
void hexdig(uint8_t val);
void hexbyte(uint8_t val);
void newline(void);
void print_string(char* string);
void putchar(char letter);
void arbitraryfunc(uint32_t funcaddr);
void memset(uint32_t base, uint8_t val, uint32_t length);
void byteout(int port, unsigned char data);
int wordin(int port);
void wordout(int port, unsigned short data);
int bytein(int port);
void Qshutdown(void);
void kernel_main(void);

extern void kernel_init(void);
extern void hang(void);
extern uint32_t eax_boot;
extern uint32_t ebx_boot;

#endif