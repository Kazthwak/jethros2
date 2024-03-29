#include "headers.h"

void wait_tick(uint32_t ticks){
for(uint64_t dest = time+ticks;time < dest;){__asm__("hlt");}
}

void wait_sec(uint16_t secs){
	wait_tick(timer_hz * secs);
}

void timer_phase(int hz){
    uint32_t divisor = 1193180 / hz;
    byteout(0x43, 0x36);
    byteout(0x40, divisor & 0xFF);
    byteout(0x40, divisor >> 8);
}

void timer_handle(struct regs* r){
	r++;// to shut up the compiler
	if(buzz_freq !=0){
		buzz_elap++;
		if(buzz_elap == buzz_freq){
			uint8_t tmp = bytein(0x61);
			text_serial();
			print_string(" KEY");
			binbyte(tmp);
			if((tmp&0b010)>>1){
				tmp &= 0b11111101;
			}else{
				tmp |= 0b010;
			}
			print_string(" ");
			binbyte(tmp);
			text_screen();
			byteout(0x61, tmp);
			buzz_elap = 0;
		}
	}
	time++;
}