#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
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
        printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n ", i, retreived_value, some_value);
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
}

void ADC_test(void){
	volatile char* ADC_address = (char*) 0x1400;
	ADC_address[0] = 0;
	_delay_us(15);
	volatile uint8_t digital_data = ADC_address[0];	
	
	//Create filter function that take the value from the analog joystick and filter the values into intervals which will be used to control the arm of the robot.
	//Slider 2 is right slider
	//slider 1 is left slider
	printf("Joystick x-axis digital input %d \n" ,digital_data);	
	digital_data = ADC_address[0];	
	printf("Joystick y-axis digital input %d \n" ,digital_data);	
	digital_data = ADC_address[0];	
	printf("Slider 1 digital input %d \n" ,digital_data);	
	digital_data = ADC_address[0];	
	printf("Slider 2 digital input %d \n\n" ,digital_data);	
	
	
}
int main(){
	//joystick x-axis second pin on AT90USB1287 board to ADC AIN0
	//joystick y-axis first pin on AT90USB1287 board to ADC AIN1
	//PB3 is the right button from PORTD1 on AT90USB1287 board
	//PB2 is the left button from PORTB7 on AT90USB1287 board
	//PB1 is the joystick button,  third pin on board goes to PB1
	//Filter 4 from AT90USB1287 board is right slider goes to pin AIN3 of ADC
	//Filter 2 from AT90USB1287 board is left slider goes to pin AIN2 of ADC
	SFIOR |= (1 << XMM2);
	MCUCR |= (1 << SRE);
	DDRB = 0x00;
	PORTB = 0x0E;
	
	//UART_SendString("mehmoos");
	alternate_printf();
	SRAM_test();
	while(1){
		ADC_test();
		printf("Joystick button %d \n",(PINB & (1 << PINB1))<=0);
		printf("left touch button %d \n",(PINB & (1 << PINB2))>0);
		printf("right touch button %d \n\n",(PINB & (1 << PINB3))>0);
		_delay_ms(5000);
	}
	return 0;
}