#include "headers.h"
/*
eax is mode:
	mode 0x0:
		print string in ebx
	mode 0x1:
		sets cursor to (ebx, ecx)
	mode 0x2:
		clear screen
	mode 0x3:
		set pixel at (ebx>>16, ebx%16) to edx
	case 0x4:
		draws a rectangle using ebx and ecx as coords and edx as colour
	case 0x5:
		put the character in ebx to cursor
	case 0x6:
		print_size(ebx, ecx)(ebx num, ecx is the size)
	case 0x7:
		UNIMPLEMENTED
		copy framebuffer at ebx into the framebuffer


	case 0x10:
		load a byte of keyboard data to the pointer in ebx
	case 0x11:
		load a character code from the keyboard to the pointer in ebx
	case 0x12:
		load a RAW character to the pointer in ebx (reads whatever was in the buffer, even a key release/similar)
	case 0x13:
		set ebx to scancode(ebx)
	case 0x14:
		start a 128b (fake) dma loading the keyboard buffer to *ebx
*/

void int48_handle(struct regs* r){
	switch(r->eax){
		case 0x0:	//print string
			print_string((char*)r->ebx);
			break;
		case 0x1: //set cursor position
			cursorx = r->ebx;
			cursory = r->ecx;
			break;
		case 0x2: //clear screen
			clear_screen();
			break;
		case 0x3:
			putpixel(r->ebx>>16, r->ebx%0xffff, r->edx);
			break;
		case 0x4:
			draw_rect(r->ebx>>16, r->ebx%0xffff, (r->ecx>>16)-(r->ebx>>16), (r->ecx&0xffff)-(r->ebx&0xffff), r->edx);
			break;
		case 0x5:
			putchar(r->ebx&0xff);
			break;
		case 0x6:
			print_size(r->ebx, r->ecx);
			break;
		
		case 0x10:
			{
				char* ebx = (char*)r->ebx;
			*ebx = get_key_buffer();
			break;
			}
		case 0x11:
			{
			char* ebx = (char*)r->ebx;
			*ebx = scancode(get_key_buffer());
			break;
			}
		case 0x12:
			{
			char* ebx = (char*)r->ebx;
			*ebx = get_raw_buffer();
			break;
			}
		case 0x13:
			r->ebx = scancode(r->ebx);
			break;
	}
}

void load_program(char* program_name){
uint32_t LBA_prog_address = find_file_by_name(program_name);
uint32_t file_length = get_file_length(LBA_prog_address);
uint32_t num_required_pages  = file_length/(page_size/0x200) + ((file_length&((page_size/0x200)-1)) > 0);
hexdword(file_length);
newline();
hexdword(num_required_pages);
if(num_required_pages > (ONE_MB/page_size)){
	print_string("\nTo much Memory Required\n");
	return;
}
for(uint16_t i = 0; i < num_required_pages; i++){
	map_alloc_to_page(i*page_size);
}
//first num_required_pages now allocated
for(uint32_t i = 0; i < file_length; i++){
disk_read((volatile struct disk_sector*)(i*512),LBA_prog_address+1+i);
}
}