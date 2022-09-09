#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include "UART_driver.h"


void SRAM_test(void) {
	volatile char* ext_ram = (char*) 0x1800; // S t a r t a d d r e s s f o r the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	// rand ( ) stores some internal state , so calling this function in a loop ?-will
	// yield different seeds each time ( unless srand ( ) is called before this ?-function)
	uint16_t seed = rand();
	// Write phase : Immediately check t h a t the c o r r e c t v al u e was s t o r e d
	srand(seed);
	for(uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if(retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// R e t r i e v a l phase : Check t h a t no v al u e s were changed du rin g o r a f t e r the?- write phase
	srand(seed);
	// r e s e t the PRNG t o the s t a t e i t had b e f o r e the w ri t e phase
	for(uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if(retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n %4d errors in write phase and\n %4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
	printf("\n\n\n\n");
	
}

int main(){


	SFIOR |= (1 << XMM2);
	MCUCR |= (1 << SRE);
	alternate_printf();
	SRAM_test();

	return 0;
}