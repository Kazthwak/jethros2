#include "headers.h"

//#define mem_lim 0x400*0x400*0x400*0x4 //4gbs
#define page_size (0x400*0x4) //4096b (4kb)
#define num_pages 0x100000 //number of 4kb pages in 4gb
#define num_page_entries (num_pages/32)
#define ONE_MB (0x400*0x400)
#define FOUR_MB (0x4*ONE_MB)

#define page_address(page_n) page_n*page_size

//1 is free
//0 is used
//page n is [i/32] bit <<i%32
//([i/32]>>(i%32))
uint32_t phys_page_state[num_page_entries];

uint32_t* page_directory;

void* page_tables[1024];

void mem_init(){
	//initialise the pointer to the end of used ram
	end_of_used_ram = &_end_of_bss;
	beg_of_used_ram = &_beg_of_mbh;

	uint32_t max_mem = stateinfo.mem_upper;
	max_mem *= 1024; //now contains the size of upper memory in bytes
	max_mem += 1024*1024; //Now adjusted for the ignored first MB of memory
	first_mem_hole = max_mem;

	init_phys_pages();


	page_directory = alloc_phys_page(ONE_MB);
	page_directory_init();
#if 1
	load_page_directory(page_directory);
	enable_paging();
#endif
}


//pages on the boundaries of usability may be flagges as unusable when they are usable (off by one error)
//I am being conservative to ensure no memory-mapped memory is used
uint8_t init_page_valid(uint32_t mem_addr){
	//end of page is after start of OS and start of page is before end of OS
	//means page includes OS image. May malfunction if the OS image is less than 1 page in size
	if((mem_addr+page_size) >= (uint32_t)beg_of_used_ram && mem_addr <= (uint32_t)end_of_used_ram){
		return(0);
	}
	//if the address is less than 1MB
	if(mem_addr < 0x400*0x400){
		//before the EBDA. this is the free memory, and the IVT and BDA which are free to be overwritten
		if(mem_addr < 0x80000){
			return(1);
		}
		//memory is after the start of the EBDA, which means it is a part of the EBDA, is otherwise reserved
		else{
			return(0);
		}
	}
	//address is in upper memory and not part of OS
	else{
		//end of page is before memory hole
		if((mem_addr+page_size) < first_mem_hole){
			return(1);
		}
		//page is past first memory hole (may be usable but to start with I am marking as unusable)
		else{
			return(0);
		}
	}

}

void init_phys_pages(){
	for(uint32_t i = 0; i < num_pages; i++){
		//i is page num
		uint32_t i_addr = page_address(i);
		uint8_t page_val = init_page_valid(i_addr);
		if(i%32 == 0){phys_page_state[i/32] = 0;}
		phys_page_state[i/32] |= page_val<<(i%32);
	}
	phys_page_state[num_page_entries-1] = 0xffffffff;
}

/*
//NEEDS TO BE FIXED
void* malloc(uint32_t size_bytes){
	uint32_t size_in_pages = (size_bytes+page_size)/page_size;
	uint32_t page_addr = malloc_int(size_bytes, &(phys_page_state[0]), num_pages);
	return((void*));
}

uint32_t malloc_int(uint32_t size, uint8_t* page_arr, uint32_t arr_len){
	for(uint32_t arr_index = 1; arr_index < arr_len-size; arr_index++){
		//arr_index iterates through the pages
		for(uint32_t size_check = 0; size_check < size; size_check++){
			//check if the page is usable
			if(page_arr[arr_index+size_check] != 0){
				goto not_big_enough;
			}
		}
		for(uint32_t use_ram = 0; use_ram < size; use_ram++){
			//mark as used
			page_arr[arr_index+use_ram] = 3;
		}
		return(arr_index);
		not_big_enough:
	}
	return(0);
}
*/

void* alloc_phys_page(uint32_t start_addr){
	for(uint32_t i = (start_addr/32/page_size); i < num_page_entries; i++){
		/*
		text_serial();
		string_serial("\nPage_state : "); hexdword(phys_page_state[i]);
		text_screen();
		*/
		if(phys_page_state[i] > 0){
			uint32_t state = phys_page_state[i];
			uint32_t pow2 = 1;
			for(uint8_t j = 0; state > 0; j++){
				if((state&0x1) == 1){
					phys_page_state[i] &= (pow2^0xffffffff);
					return((void*)(page_size*((i*32)+j)));
				}
				pow2 <<= 1;
				state >>= 1;
			}
			return((void*)0xffffffff);
		}
	}
	return((void*)0xffffffff);
}


void page_directory_init(){
	for(uint16_t i = 0; i < 1024; i++){
		page_tables[i] = 0;
	}
	for(uint16_t i = 0; i < 1024; i++){
		page_directory[i] = 0x00000002;
	}
	//OLD Method
	/*
	//page_tables 0 contains a pointer to an empty first page table
	page_tables[0] = alloc_phys_page();
	uint32_t* page_table_1 = page_tables[0];
	for(uint16_t i = 0; i < 1024; i++){
		page_table_1[i] = (i*page_size) | 0b11;
	}
	page_directory[0] = (uint32_t)page_tables[0] | 3;
	*/
	//identity page the first 8MB of ram for the OS to use
	init_nonidentity_page();
	install_identity_page(FOUR_MB);
	//identity page the Video memory so it works
	install_identity_page(page_dir_align(vbe_info.phys_addr));
}

uint32_t page_dir_align(uint32_t mem_addr){
	uint32_t tmp = mem_addr/(page_size)/0x400;
	return(tmp*page_size*0x400);

}

//ADDDRESS MUST BE 4MB ALIGNED
void install_identity_page(uint32_t mem_address){

//the offset in the page directory and the page table number
uint32_t dir_addr = mem_address/(page_size)/(0x400);
page_tables[dir_addr] = alloc_phys_page(ONE_MB);
uint32_t* ref_page_table = page_tables[dir_addr];
for(uint16_t i = 0; i < 1024; i++){
	//janky way of getting the correct offset in there
	ref_page_table[i] = ((dir_addr*(page_size)*0x400) + (i*(page_size))) | 0b11;
}
	page_directory[dir_addr] = (uint32_t) page_tables[dir_addr] | 3;
}

//identity pages the first 4MB of memory
void init_nonidentity_page(){
page_tables[0] = alloc_phys_page(ONE_MB);
uint32_t* ref_page_table = page_tables[0];
//start at 1 MB to leave the first 1MB free and unmapped
for(uint16_t i = 0; i < 1024; i++){
	//janky way of getting the correct offset in there
	if(i < (ONE_MB/page_size)){
		ref_page_table[i] = 0b10;
	}else{
		ref_page_table[i] = ((i*(page_size))) | 0b11;
	}
}
	page_directory[0] = (uint32_t) page_tables[0] | 3;
}

//allocates a physical page, then maps it to the virtual address virt_address
void map_alloc_to_page(uint32_t virt_address){
	uint32_t physical_page_address = (uint32_t)alloc_phys_page(0);
	uint16_t directory_address = virt_address/page_size/0x400;
	uint16_t table_address = (virt_address/page_size)%0x400;
	uint32_t* relevant_table = page_tables[directory_address];
	relevant_table[table_address] = physical_page_address | 0b11;
	text_serial();
	print_string("Mapped ");
	hexdword(physical_page_address);
	print_string(" to ");
	hexdword(virt_address);
	print_string("\n");
	text_screen();
}