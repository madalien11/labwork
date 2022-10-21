#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "SPI_driver.h"

char read(char address){
	SS_Enable();
	char data;
	SPI_MasterTransmit(0x03);
	SPI_MasterTransmit(address);
	data = SPI_MasterReceive();
	SS_Disable();
	return data;
}
	
void write(char address, char data){
	SS_Enable();
	SPI_MasterTransmit(0x02);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(data);
	SS_Disable();	
}
	
void RTS(char buffer){
	char mask = 0x07;
	char instruction = ((buffer & mask) | 0x80);
	SS_Enable();
	SPI_MasterTransmit(instruction);
	SS_Disable();
	
}
	
char read_status(){
	SS_Enable();
	char data;
	SPI_MasterTransmit(0xA0);
	data = SPI_MasterReceive();
	SS_Disable();
	return data;
}
	
void bit_modify(char address, char mask, char data){
	SS_Enable();
	SPI_MasterTransmit(0x05);
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(mask);
	SPI_MasterTransmit(data);
	SS_Disable();
}

	
void reset(){
	SS_Enable();
	SPI_MasterTransmit(0xC0);
	SS_Disable();
}
	
	
char read_rx_status(){
	SS_Enable();
	char data;
	SPI_MasterTransmit(0xB0);
	data = SPI_MasterReceive();
	SS_Disable();
	return data;
}
	