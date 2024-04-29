#include "headers.h"

// uint8_t

void disk_init(uint8_t disk_num){
disk_num++;
}

void poll_master_hdd(){
	wait_sec(2);
}

uint8_t ata_data[512];

//curent driver only accepts 1 sector reads
uint8_t read_sector(volatile struct disk_sector* memloc, uint32_t sector_address, uint8_t disknum){
uint16_t curb;
/*
//reset drive first
byteout(0x3f6, 4);
curb = cursorx;
while(!is_key_waiting()){cursorx = curb; binbyte(bytein(0x1f7));wait_tick(1);}newline();get_key_buffer();
byteout(0x3f6, 0);
wait_sec(1);
bytein(0x1f1);
for(uint16_t i = 0; i < 256; i++){
	ata_data[i] = wordin(0x1f0);
}
print_string("Drive rest");


curb = cursorx;
while(!is_key_waiting()){cursorx = curb; binbyte(bytein(0x1f7));wait_tick(1);}newline();get_key_buffer();
*/
byteout(0x1f6, 0xE0 | 0);//select master drive
byteout(0x1f1, 0x0);//null byte
byteout(0x1f2, 0x1);//sector count
byteout(0x1f3, 0x1);//low byte
byteout(0x1f4, 0x0);
byteout(0x1f5, 0x0);
byteout(0x1f7,0x20);//read_sectors

curb = cursorx;
while(!is_key_waiting()){cursorx = curb; binbyte(bytein(0x1f7));wait_tick(1);}newline();get_key_buffer();
print_string("reading (fingers crossed)");
hexword(wordin(0x1f0));
newline();
binbyte(bytein(0x1f7));
newline();
binbyte(bytein(0x1f1));
hang();
}

uint8_t write_sector( volatile struct disk_sector* memloc, uint32_t sector_address, uint8_t disknum){
//todo, implement
memloc++;
sector_address++;
disknum++;
return(1);
}