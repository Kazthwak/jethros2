#include "headers.h"

volatile char keybuffer[256];
volatile uint32_t keybufferloc = (uint32_t)&keybuffer;

bool isshift = false;

unsigned char kbdmix[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '+', /*'´' */0, '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '<',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '-',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,  '<',
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};


unsigned char kbdse_shift[128] = {
    0,  27, '!', '\"', '#', 0 /* shift+4 */, '%', '&', '/', '(',	/* 9 */
  ')', '=', '?', '`', '\b',	/* Backspace */
  '\t',			/* Tab */

 'Q', 'W', 'E', 'R',   /* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', 'A', 'A', '\n', /* Enter key */
    0,          /* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'O', /* 39 */
 '\'', '>',   0,        /* Left shift */
 '*', 'Z', 'X', 'C', 'V', 'B', 'N',            /* 49 */
  'M', ';', ':', '_',   0,              /* Right shift */

  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   '>',
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};


char scancode(char scancodein){
if(isshift){
	return(kbdse_shift[(uint8_t)scancodein]);
}
return(kbdmix[(uint8_t)scancodein]);
}

bool is_key_waiting(){
	return((uint32_t)keybufferloc != (uint32_t)&keybuffer);
}

bool ispressed(char key){
return(keypressed[(uint8_t)key]);
}

//FIXED
char get_key_buffer(){
if((uint32_t)&keybuffer == keybufferloc){return(0);}
uint8_t output = keybuffer[0];
for(uint8_t i = 0; i < 255; i++){
	keybuffer[i] = keybuffer[i+1];
}
keybufferloc--;
if(output == 0x2a){
	isshift = true;
	return(get_key_buffer());
}
if(output == 0xaa){
	isshift = false;
	return(get_key_buffer());
}



if(output>>7 == 1){return(get_key_buffer());}
return(output);
}

void clear_key_buffer(){
keybufferloc = (uint32_t)&keybuffer;
}

char get_raw_buffer(){
if(keybufferloc == (uint32_t)&keybuffer){return(0);}
char out = keybuffer[0];
for(uint8_t i = 0; i < 0xff; i++){
	keybuffer[i] = keybuffer[i+1];
}
keybufferloc--;
return(out);
}

void print_size(uint64_t num, uint8_t size){
switch(size){
	case 8:
	hexbyte(num);
	break;
	case 16:
	hexword(num);
	break;
	case 32:
	hexdword(num);
	break;
	case 64:
	hexqword(num);
	break;
	default:
	hexqword(num);
	break;
}
}

uint64_t trunc_int(uint64_t num, uint8_t size){
switch(size){
	case 8:
	return((uint8_t)num);
	case 16:
	return((uint16_t)num);
	case 32:
	return((uint32_t)num);
	case 64:
	return((uint64_t)num);
	default:
	return((uint64_t)num);
}
}

uint64_t get_num_in(uint8_t size){
	uint64_t num = 0;
	bool going = true;
	uint16_t curd = cursorx;
	while(going){
		cursorx = curd;
		print_size(num,size);
		while(!is_key_waiting()){}
		uint8_t key = get_key_buffer();
		if(key == ENTER){going = false; break;}
		if(key == BACKSPACE){num = num >> 4;}
		key = scancode(key);
		if((key >= 0x30 && key <= 0x39) || (key >= 0x61 && key <= 0x66)){
			//key is valid number
			num = num<<4;
			key -= 0x30;
			if(key > 0x09){
				key -= (0x30-0x09);
			}
			num+= key;
		}
	}
num = trunc_int(num,size);
return(num);
}

void wait_for_enter(){
while(1){
	while(!is_key_waiting()){}
	uint8_t tmp = get_key_buffer();
	if(tmp == ENTER){
		return;
	}
}
}