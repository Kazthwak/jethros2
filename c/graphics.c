#include "headers.h"

#define default_xres 480
#define default_yres 360
#define default_coldepth 3
#define default_screen_addr 0xb8000

static uint16_t scanlinewidth;
static uint32_t screen_address;
static uint16_t colour_depth;

void graphics_init(){
if((stateinfo.flags>>11)&1){
	scanlinewidth = 4*vbe_info.x_res;
	screen_address = vbe_info.phys_addr;
	x_res = vbe_info.x_res;
	y_res = vbe_info.y_res;
	colour_depth = 4;
}
else{
	scanlinewidth = 4* default_xres;
	screen_address = default_screen_addr;
	colour_depth = default_coldepth;
}
x_char_res = x_res>>3;
y_char_res = y_res>>3;
memset(screen_address, 0x00, (x_res*y_res)<<2);
}

void putpixel(uint16_t x, uint16_t y, uint32_t colour){
uint32_t yoff = (vbe_info.x_res*y)<<2;
uint32_t* screen_loc = (uint32_t*)(yoff+(x<<2)+screen_address);
if(colour_depth == 3){
	*screen_loc = 0xffffff&colour;
}
else{
	*screen_loc = colour;
}
}

void putcharxy(uint16_t x, uint16_t y, char character){
for(uint8_t i = 0; i < 8; i++){
	for(uint8_t j = 0; j < 8; j++){
		if((font_basic[(uint16_t)character][i]>>j)&1){
			putpixel(x+j, y+i, 0xffffff);
		}
	}
}
}

void putcharxyc(uint16_t x, uint16_t y, char character){
putcharxy(x<<3, y<<3, character);
}