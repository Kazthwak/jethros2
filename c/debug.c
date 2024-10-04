#include "headers.h"

void timestamp(void){
	print_string(" with timestamp ");
	hexqword(time);
}


void port_wiz(){
	testval = 1;
	clear_screen();
	print_string("Welcome to the interactive port debugger. I'm Wizzy, your friendly neighbourhood ");
	print_string("wizard!\n");
	print_string("Option: \n1: Read \n2: Write \n0: Exit \n");
	uint8_t num = get_num_in(8);
	if(num == 0){print_string("\nGOODBYE\n"); return;}
	while(1){
		newline();
		if(num == 1){
			//read
			print_string("Size (0 for 8 and 1 for 16) :");
			uint8_t size = get_num_in(8);
			print_string("\nPort to read :");
			uint32_t port = get_num_in(32);
			print_string("\nPort Read Value of: ");
			if(size == 0){
				uint8_t tmp = bytein(port);
				hexbyte(tmp);
				text_serial();
				print_string("\nRead ");
				hexbyte(tmp);
				print_string(" from port ");
				hexdword(port);
				timestamp();
				text_screen();
			}else{
				uint16_t tmp = wordin(port);
				hexword(tmp);
				text_serial();
				print_string("\nRead ");
				hexword(tmp);
				print_string(" port ");
				hexdword(port);
				timestamp();
				text_screen();
			}
			clear_key_buffer();
			wait_for_enter();
		}else{
			//write
			print_string("Size (0 for 8 and 1 for 16) :");
			uint8_t size = get_num_in(8);
			print_string("\nPort to write :");
			uint32_t port = get_num_in(32);
			print_string("\nValue to write :");
			uint16_t value = get_num_in(16);
			if(size == 0){
				byteout(port, value);
				text_serial();
				print_string("\nWrote ");
				hexbyte(value);
				print_string(" to port ");
				hexdword(port);
				timestamp();
				text_screen();
			}else{
				wordout(port, value);
				text_serial();
				print_string("\nWrote ");
				hexword(value);
				print_string(" to port ");
				hexdword(port);
				timestamp();
				text_screen();
			}
		}
		clear_screen();
		print_string("Option: \n1: Read \n2: Write \n0:Exit \n");
		num = get_num_in(8);
		if(num == 0){break;}
	}
	print_string("\nGOODBYE\n");
}

void test_phys_pages(){
	while(1){
	clear_screen();
	print_string("Testing the page mapper. \nWhat address do you wish to test> : ");
	uint32_t addr = get_num_in(32);
	addr = addr/(page_size);
	print_string("\nPage holds value : ");
	hexdword(addr);
	newline();
	hexbyte((phys_page_state[addr/32]>>(addr%32))&1);
	wait_for_enter();
	}
}

void mem_debug(){
		print_string("Memory address to dump : ");
		uint32_t addr = get_num_in(32);
		clear_screen();
		print_string("Dumping 256 bytes from ");
		hexdword(addr);
		for(uint16_t y = 0; y < 256; y++){
			hexbyte(*(uint8_t*)(addr+y));
			print_string(" ");
			if(y%32 == 0){newline();}
		}
		wait_for_enter();
		clear_screen();
}

void uber_debug(){
	clear_screen();
	print_string("DEBUGGER OPENED\n");
	string_serial("\nDEBUGGER OPENED\n");
}