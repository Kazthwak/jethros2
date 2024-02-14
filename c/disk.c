#include "headers.h"

// uint8_t

void disk_init(uint8_t disk_num){
disk_num++;
}

void poll_master_hdd(){
	wait_sec(2);
}


//curent driver only accepts 1 sector reads
uint8_t read_sector(volatile struct disk_sector* memloc, uint32_t sector_address, uint8_t disknum){
uint8_t diskselesct = 0xE0;
if(disknum == 1){diskselesct = 0xE0;}
uint8_t address_top = (sector_address>>24)&0x0F;
diskselesct |= address_top;
byteout(0x1F6, diskselesct);
byteout(0x1F1, 0);
byteout(0x1F2, 1);
byteout(0x1F3, (uint8_t)(sector_address>>0));
byteout(0x1F4, (uint8_t)(sector_address>>8));
byteout(0x1F5, (uint8_t)(sector_address>>16));
byteout(0x1F7, 0x20);

//poll for ready
poll_master_hdd();

//copy to sector
putchar(':');
putchar((uint8_t)bytein(0x1F0));
return(0);
}

uint8_t write_sector( volatile struct disk_sector* memloc, uint32_t sector_address, uint8_t disknum){
//todo, implement
memloc++;
sector_address++;
disknum++;
return(1);
}