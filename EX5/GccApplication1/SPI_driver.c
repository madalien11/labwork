#include "SPI_driver.h"
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

void SPI_master_init(){
	
	/* Set MOSI and SCK output, all others input */
	DDRB = (1<<DDB5) | (1<<DDB7) | (1<<DDB4);
	//enable SPI, master, set clock rate to fck/16
	SPCR = (1<<SPE) |(1<<MSTR) | (1<<SPR0);
	
	PORTB |= (1<<DDB4);
}

void SPI_MasterTransmit(char cData)
{
	char flush_buffer;
	/* Start transmission */
	//printf("cdata is %c\n", cData);
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	flush_buffer = SPDR;
	
}

char SPI_MasterReceive(){
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

	
	
void SPI_SlaveInit(void)
{
	/* Set MISO output, all others input */
	DDRB = (1<<DDB6);
	/* Enable SPI */
	SPCR = (1<<SPE);
}

char SPI_SlaveReceive(void)
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)))
	;
	/* Return data register */
	return SPDR;
}