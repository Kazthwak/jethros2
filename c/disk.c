#include "headers.h"

#define master_drive 0xE0
#define slave_drive  0xF0

void print_string_len(char* str, uint16_t len){
	for (; len > 0; len--){
		putchar(*str);
		str++;
	}
}

bool disk_poll(void){
	
	//400ns delay
	for(volatile uint8_t i = 0; i < 14; i++){
		bytein(0x1f7);
	}
	
	while(1){
	uint8_t status = bytein(0x1f7);
	uint8_t BSY, DRQ, ERR, DF;
	BSY = (status&0b10000000)>>7;
	DRQ = (status&0b00001000)>>3;
	ERR = (status&0b00000001)>>0;
	 DF = (status&0b00100000)>>5;
	if(ERR == 1 || DF == 1){
		//AN ERROR HAS OCCURRED
		return(false);
	}
	if(BSY == 0 && DRQ == 1){
		//ready to progress
		return(true);
	}
	}
}

bool disk_read(volatile struct disk_sector* sector_address, uint32_t LBA){
//starting disk read
//select the master drive
byteout(0x1f6, slave_drive | ((LBA >> 24) & 0x0F));
//requenst 1 sector
byteout(0x1f2, 1);
//send the sector address
byteout(0x1f3, LBA&0xff);
byteout(0x1f4, (LBA>>8)&0xff);
byteout(0x1f5, (LBA>>16)&0xff);

//send the read command
byteout(0x1f7, 0x20);

//poll for ready
if(!disk_poll()){return(false);}

for(uint16_t i = 0; i<256; i++){
	uint16_t val = wordin(0x1f0);
	sector_address->data[i*2] = val&0xff;
	sector_address->data[i*2+1]=val>>0x8;
	val = ((val&0xff)<<8) | (val>>8);
	// hexword(val);
	// putchar(' ');
}
return(true);
}

#define TAR_length_offset 124
#define TAR_type_offset 156
#define TAR_name_offset 0
#define TAR_identifier_offset 257
#define TAR_identifier "ustar "

//returns an index to the header of the file, or -1 if no file
int64_t find_file_by_name(char* name){
uint32_t addr = 0;
struct disk_sector buffer;
while(1){
	disk_read(&buffer, addr);
	//check valid
	bool valid = text_match(((void*)&buffer)+TAR_identifier_offset, TAR_identifier, 6);
	if(!valid){
		print_string("Non-ustar encountered. failiure condition. This probably means the file was not found\n");
		return(-1);
	}
	//check for match, returning addr if a match
	bool iscorrectfile = text_match(((void*)&buffer)+TAR_name_offset, name, 100);
	if(iscorrectfile){
		print_string("Name match found. success!\n");
		return(addr);
	}
	//increment addr by length
	uint32_t length = octal_get(((void*)&buffer)+TAR_length_offset, 11);
	uint32_t sectorlen = (length>>9) + (1) + ((0x1ff&length)>0);
	addr += sectorlen;
	print_string("Wrong file found. Adding to addr: ");
	hexdword(sectorlen);
	newline();
}
}