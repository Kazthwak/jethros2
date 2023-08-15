#include "headers.h"

//raise number to the power of the exponent
uint32_t pow(uint16_t number, uint16_t exponent){
uint32_t total = 1;
for(uint16_t i = 0; i < exponent; i++){
	total *= number;
}
return(total);
}