#ifndef kernel_header_included
#define kernel_header_included
#define version "beta 1.0"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "./font.h"

//a mask used to check for "errors" when the bootloader does not provide enough information.
//bits set:
//vbe info flag bit
//bootloader name flag bit set
//memory map flag bit set
#define req_multiboot_flags 0b101001000000

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

//from func 00
struct vbe_control_info{
	//VESA. NOT NULL TERMINATED
	char vesa_string[4];
	uint16_t vbe_ver;
}__attribute__((packed));

//from func 01
struct vbe_mode_info{
	uint16_t mode_attributes;
	uint8_t window_a_attributes;
	uint8_t window_b_attributes;
	uint16_t window_granularity;
	uint16_t window_size;
	uint16_t window_a_segment;
	uint16_t window_b_segment;
	uint32_t window_function_p;
	uint16_t bytes_scanline;
	uint16_t x_res;
	uint16_t y_res;
	uint8_t char_x;
	uint8_t char_y;
	uint8_t num_planes;
	uint8_t bits_per_pixel;
	uint8_t num_banks;
	uint8_t mem_model;
	uint8_t bank_size;
	uint8_t num_image_pages;
	uint8_t res_pages;
	uint8_t red_mask_size;
	uint8_t red_field_pos;
	uint8_t green_mask_size;
	uint8_t green_field_pos;
	uint8_t blue_mask_pos;
	uint8_t blue_field_pos;
	uint8_t rsvd_mask_size;
	uint8_t rsvd_field_position;
	uint8_t direct_colour_mode_info;
	//
	uint32_t phys_addr;
	uint32_t off_screen_mem_offset;
	uint32_t off_screen_mem_size;
}__attribute__((packed));



static struct vbe_control_info vbe_control_info;
static struct vbe_mode_info vbe_info;
static struct MultiBootInfoStruct stateinfo;
static uint16_t cursorx = 0;
static uint16_t cursory = 0;
static uint16_t x_res = 0;
static uint16_t y_res = 0;
static uint16_t x_char_res = 0;
static uint16_t y_char_res = 0;

//function prototypes
uint32_t pow(uint16_t number, uint16_t exponent);
void decint(uint16_t number);
void putcharxyc(uint16_t x, uint16_t y, char character);
void putcharxy(uint16_t x, uint16_t y, char character);
void graphics_init(void);
void putpixel(uint16_t x, uint16_t y, uint32_t colour);
void info(void);
void checks(void);
void err_fat(uint8_t err_code);
void err_Nfat(uint8_t err_code);
void binbyte(uint8_t val);
void binword(uint16_t val);
void bindword(uint32_t val);
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