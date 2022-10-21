#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "UART_driver.h"
#include "OledDriver.h"
#include "controller.h"
#include "MenuNavigator.h"
#include "CAN_driver.h"
#include "MCP2515_driver.h"
#include "SPI_driver.h"

#define F_CPU 4915200UL

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
	
	alternate_printf();
	SRAM_test();
	
	/*
	while(1){
		struct controller control = get_controller();
		printf("Joystick x-axis digital input %d \n" ,control.x_axis);
		printf("Joystick y-axis digital input %d \n" ,control.y_axis);		
		printf("Slider 1 digital input %d \n" ,control.slider_left);
		printf("Slider 2 digital input %d \n\n" ,control.slider_right);
		printf("Joystick button %d \n",control.joystick_pressed);
		printf("left touch button %d \n",control.left_button_pressed);
		printf("right touch button %d \n",control.right_button_pressed);
		printf("joystick position %d \n\n" , control.pos);
		_delay_ms(5000);
		
	}
	
	
	oled_init();
	oled_home();
	oled_print("420 ",0);
	oled_goto_line(1);
	oled_goto_column(0x00,0x00);
	oled_print("what you ",1);
	oled_goto_line(2);
	oled_goto_column(0x00,0x00);
	oled_print("smoking",0);
	*/
	
	//menuNavigator();
	
	SPI_master_init();
	CAN_init();
	
	struct CAN_msg* msg = malloc(sizeof(struct CAN_msg));
	msg->id = 123;
	msg->length = 8;
	strcpy(msg->data, "Hello");
	
	CAN_transmit(msg);
	
	// DO NOT DELETE THIS DELAY
	_delay_ms(5);
	//printf("transmit return is %d\n", tr_ret);
	//printf("\0");
	
	struct CAN_msg* rx_msg = malloc(sizeof(struct CAN_msg));
	rx_msg->id = 61;
	rx_msg->length = 6;
	
	CAN_receive(rx_msg);
	//printf("\0");
	//printf("\0", rx_ret);
	
	printf("id is %d\n", rx_msg->id);
	printf("length is %d\n", rx_msg->length);
	printf(rx_msg->data);

	
	
	
	
	return 0;
}