#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "MCP2515_driver.h"
#include "CAN_registers.h"
#include "SPI_driver.h"
#include "CAN_driver.h"

void CAN_init(){
	reset();
	write(0x4B,0x0F);
	
	reset();
	_delay_ms(1);
	
	////////////////////////////
	
	
	//Clear masks to RX all messages
	write(RXM0SIDH,0x00);
	write(RXM0SIDL,0x00);

	//Clear filter... really only concerned to clear EXIDE bit
	write(RXF0SIDL,0x00);

	//Set CNF1
	write(CNF1,CAN_125kbps);

	//Set CNF2
	write(CNF2,0x80 | PHSEG1_3TQ | PRSEG_1TQ);

	//Set CNF3
	write(CNF3, PHSEG2_3TQ);

	//Set TXB0 DLC and Data for a "Write Register" Input Message to the MCP25020
	write(TXB0SIDH,0xA0);    //Set TXB0 SIDH
	write(TXB0SIDL,0x00);    //Set TXB0 SIDL
	write(TXB0DLC,DLC_3);    //Set DLC = 3 bytes
	write(TXB0D0,0x1E);      //D0 = Addr = 0x1E
	write(TXB0D1,0x10);      //D1 = Mask = 0x10

	//Set TXB1 DLC and Data for a "READ I/O IRM"
	write(TXB1SIDH,0x50);    //Set TXB0 SIDH
	write(TXB1SIDL,0x00);    //Set TXB0 SIDL
	write(TXB1DLC,0x40 | DLC_8);    //Set DLC = 3 bytes and RTR bit
	
	//Interrupt on RXB0 - CANINTE
	write(CANINTE,0x01);    //

	//Set NORMAL mode
	write(CANCTRL,REQOP_LOOPBACK | CLKOUT_ENABLED);
	
	//Verify device entered Normal mode
	char dummy = read(CANSTAT);
	if (OPMODE_LOOPBACK != (dummy && 0xE0))
		write(CANCTRL,REQOP_LOOPBACK | CLKOUT_ENABLED);
}

uint8_t CAN_transmit(struct CAN_msg *msg) {
	char status = read_status();
	char rts;
	uint8_t address;
	if ((status & (1 << 2)) == 0) {
		address = TXB0SIDH;
		rts = 0x01;
	} else if ((status & (1 << 4)) == 0) {
		address = TXB1SIDH;
		rts = 0x02;
	} else if ((status & (1 << 6)) == 0) {
		address = TXB2SIDH;
		rts = 0x04;
	} else {
		return 0;
	}
	
	write(address, msg->id >> 3);
	write(address+1, msg->id << 5);
	write(address+2, 0);
	write(address+3, 0);
	
	write(address+4, msg->length);
	for(int i = 0; i < msg->length; i++) {
		write(address+5+i, msg->data[i]);
	}
	
	_delay_us(1);
	RTS(rts);
	
	return rts;
}

uint8_t CAN_receive(struct CAN_msg *msg) {
	uint8_t status = read_rx_status();
	uint8_t address;
	
	if (status & (1 << 6)) {
		address = RXB0SIDH;
	} else if (status & (1 << 7)) {
		address = RXB1SIDH;
	} else {
		return 0;
	}
	
	//write(address);
	msg->id = ((uint16_t)read(address)) << 3;
	msg->id |= (read(address+1))>>5;
	msg->length = read(address+4);
	for(int i = 0; i < msg->length; i++) {
		msg->data[i]=read(address+5+i);
	}
	if(status & (1 << 6)){
		bit_modify(CANINTF, (1<<RX0IF),0);
	}
	else {
		bit_modify(CANINTF, (1<<RX1IF),0);	
	}
	
	return (status & 0x07) +1;
		
}