#include "headers.h"

void wait_tick(uint32_t ticks){
for(uint64_t dest = time+ticks;time < dest;){__asm__("hlt");}
}

void wait_sec(uint16_t secs){
	wait_tick(timer_hz * secs);
}

void timer_phase(int hz){
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    byteout(0x43, 0x36);             /* Set our command byte 0x36 */
    byteout(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    byteout(0x40, divisor >> 8);     /* Set high byte of divisor */
}

void timer_handle(struct regs* r){
	r++;// to shut up the compiler
	time++;
}