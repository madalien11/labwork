#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "controller.h"

/*
struct controller {
	volatile uint8_t x_axis;
	volatile uint8_t y_axis;
	volatile uint8_t slider_left;
	volatile uint8_t slider_right;
	int joystick_pressed;
	int left_button_pressed;
	int right_button_pressed;
};
*/

struct controller get_controller(){
	volatile char* ADC_address = (char*) 0x1400;
	ADC_address[0] = 0;
	_delay_us(15);
	
	struct controller result;
	
	result.x_axis = ADC_address[0];
	result.y_axis = ADC_address[0];
	result.joystick_pressed = (PINB & (1 << PINB1))<=0;
	result.slider_left = ADC_address[0];
	result.slider_right = ADC_address[0];
	result.left_button_pressed = (PINB & (1 << PINB2))>0;
	result.right_button_pressed = (PINB & (1 << PINB3))>0;
	if(result.y_axis < 50)
		result.pos = DOWN;
	else if(result.y_axis > 200)
		result.pos = UP;
	else if(result.x_axis < 50)
		result.pos = LEFT;
	else if(result.x_axis > 200)
		result.pos = RIGHT;
	else 
		result.pos = NEUTRAL;
		 
	return result;
}