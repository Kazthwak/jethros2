#include "headers.h"

char keypressed[128];
char keybuffer[256];
uint32_t keybufferloc = (uint32_t)&keybuffer;

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

char get_key_buffer(){
	begining:
if((uint32_t)&keybuffer == keybufferloc){return(0);}
uint8_t output = keybuffer[0];
for(uint8_t i = 0; i < 255; i++){
	keybuffer[i] = keybuffer[i+1];
}
keybufferloc--;
if(output == 0x2a){
	isshift = true;
	goto begining;
}
if(output == 0xaa){
	isshift = false;
	goto begining;
}



if(output>>7 == 1){goto begining;}
return(scancode(output));
}

void clear_key_buffer(){
keybufferloc = (uint32_t)&keybuffer;
}