#ifndef kernel_header_included
#define kernel_header_included

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static uint16_t cursorx = 0;
static uint16_t cursory = 0;

void hexdig(uint8_t val);
void hexbyte(uint8_t val);
void newline(void);
void print_string(char* string);
void putchar(char letter);
void arbitraryfunc(uint32_t funcaddr);
void memset(uint32_t base, uint8_t val, uint32_t length);
void byteout(int port, unsigned char data);
int wordin(int port);
void wordout(int port, unsigned short data);
int bytein(int port);
void Qshutdown(void);
void kernel_main(void);
extern void kernel_init(void);
extern void hang(void);

#endif