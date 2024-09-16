#include "headers.h"

#define default_xres 480
#define default_yres 360
#define default_coldepth 3
#define default_screen_addr 0xb8000

static uint16_t scanlinewidth;
static uint32_t screen_address;
static uint16_t colour_depth;
//error reporting for non-fatal errors

void clear_screen(){
	draw_rect(0, 0, x_res, y_res, 0x000000);
	cursorx = 0;
	cursory = 0;
}

void grtest(){
	memcpy((void*)screen_address, scanlinewidth*(y_res>>1), (void*)screen_address+(scanlinewidth*(y_res>>1)));
}

//initialise several graphics variables (and a few others)
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
y_char_res = y_res>>4;
memset(screen_address, 0x00, (x_res*y_res)<<2);
//memory sttuff
lower_mem = stateinfo.mem_lower<<10;
upper_mem = stateinfo.mem_upper<<10;
cursorx = 0;
cursory = 0;
}

//set the pixel at x, y
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

void draw_rect(uint16_t x, uint16_t y, uint16_t x_size, uint16_t y_size, uint32_t colour){
uint32_t* rect_corner = (uint32_t*)(((vbe_info.x_res*y)<<2)+(x<<2)+screen_address);
uint16_t pitch = x_res;
for(; y_size > 0; y_size--){
	for(uint16_t j = 0; j < x_size; j++){
		*(rect_corner+j) = colour;
	}
	rect_corner += pitch;
}
}

//put a character at x,y (pixel coordinates)
void putcharxy(uint16_t x, uint16_t y, char character){
for(uint8_t i = 0; i < 8; i++){
	for(uint8_t j = 0; j < 8; j++){
		if((font_basic[(uint16_t)character][i]>>j)&1){
			putpixel(x+j, (y+i)<<1, 0xffffff);
			putpixel(x+j, ((y+i)<<1)+1, 0xffffff);
		}else{
			putpixel(x+j, (y+i)<<1, 0x0);
			putpixel(x+j, ((y+i)<<1)+1, 0x0);
		}
	}
}
}

//put a character at x,y (character coordinates)
void putcharxyc(uint16_t x, uint16_t y, char character){
putcharxy(x<<3, y<<3, character);
}