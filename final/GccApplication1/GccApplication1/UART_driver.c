/*

	ATmega 16 UART echo program
	http://www.electronicwings.com

*/ 

#define F_CPU 4915200UL			/* Define frequency here its 4.9152MHz */
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "UART_driver.h"

//#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


void UART_init(long USART_BAUDRATE)
{
	UBRR0L = BAUD_PRESCALE;		/* Load lower 8-bits of the baud rate value */
	UBRR0H = (BAUD_PRESCALE >> 8);	/* Load upper 8-bits*/
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);/* Turn on transmission and reception */
	UCSR0C |= (1 << URSEL0) | (1<<USBS0) | (3 << UCSZ00);/* Use 8-bit character sizes */
	
}

unsigned char UART_RxChar()
{
	while ((UCSR0A & (1 << RXC0)) == 0);/* Wait till data is received */
	return(UDR0);			/* Return the byte*/
}

void UART_TxChar(char ch)
{
	while (! (UCSR0A & (1<<UDRE0)));	/* Wait for empty transmit buffer*/
	UDR0 = ch ;
}

void UART_SendString(char *str)
{
	unsigned char j=0;
	
	while (str[j]!=0)		/* Send string till null */
	{
		UART_TxChar(str[j]);	
		j++;
	}
}

void alternate_printf()
{
	UART_init(9600);
	UART_SendString("\n\t UART initialized ");
	
	fdevopen(UART_TxChar,UART_RxChar);	
}
