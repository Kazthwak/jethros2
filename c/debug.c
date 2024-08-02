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