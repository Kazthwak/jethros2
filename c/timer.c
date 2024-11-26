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
	time++;
	if(cursoron == 1 && time%20 == 0){
		static uint32_t last_cursor;
		draw_rect(8*(cursorx+1),16*cursory,8,16,last_cursor);
		if(last_cursor == 0x0){last_cursor = 0xffffff;}
		else{last_cursor = 0x0;}
	}
	if(timerflags){
		if(timerflags&1){
			r->eip = (uint32_t)uber_debug;
		}
	}
}