#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include "Fonts.h"




void write_c(char c){
	volatile char* oled_address_cmd = (char*) 0x1000;
	oled_address_cmd[0] = c;
}

void write_data(char c){
	volatile char* oled_address_data = (char*) 0x1200;
	oled_address_data[0] = c;
}

void oled_init(){
	 write_c(0xae); // display off
	 write_c(0xa1); //segment remap
	 write_c(0xda); //common pads hardware: alternative
	 write_c(0x12);
	 write_c(0xc8); //common output scan direction:com63~com0
	 write_c(0xa8); //multiplex ration mode:63
	 write_c(0x3f);
	 write_c(0xd5); //display divide ratio/osc. freq. mode
	 write_c(0x80);
	 write_c(0x81); //contrast control
	 write_c(0x50);
	 write_c(0xd9); //set pre-charge period
	 write_c(0x21);
	 write_c(0x20); //Set Memory Addressing Mode
	 write_c(0x02);
	 write_c(0xdb); //VCOM deselect level mode
	 write_c(0x30);
	 write_c(0xad); //master configuration
	 write_c(0x00);
	 write_c(0xa4); //out follows RAM content
	 write_c(0xa6); //set normal display
	 write_c(0xaf); // display on	
	oled_reset();
	oled_home();
}

void oled_reset(){
	for (char i = 0x00 ; i < 0x08 ; i++)
	{
		oled_goto_line(i);
		for (int j = 0; j < 128;j++)
			write_data(0x00);
	}
	oled_home();
}

void oled_home(){
	 oled_goto_line(0x00);
	 oled_goto_column(0x00,0x00);
	 
	
}

void oled_goto_line(char lin){
	write_c(0xb0 | lin);
	
}

void oled_goto_column(char col_lsb, char col_msb){
	write_c(0x00 | col_lsb);
	write_c(0x10 | col_msb);
	
}

void oled_clear_line(){
	
}

void oled_pos(){
	
}

void oled_print(char *in, int highlighted){
	char* temp = in;
	char word;
	while(*temp != '\0'){
		for (int i = 0; i < 8 ;i++){
			word = pgm_read_byte(&(font8[*temp - 32][i]));
			highlighted==0? write_data(word):write_data(~word);
		}
		temp++;
	}
}
