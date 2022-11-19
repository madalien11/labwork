/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_interrupt.h"

#include <stdio.h>
#include "sam.h"

#include "printf-stdarg.h"
#include "uart.h"
#include "can_controller.h"
#include "pwm_module.h"
#include "Motor_controller.h"

#define DEBUG_INTERRUPT 1

/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */
void CAN0_Handler( void )
{
	//if(DEBUG_INTERRUPT)printf("\n\n\nCAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
		CAN_MESSAGE message;
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
			can_receive(&message, 1);

		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		
		{
			can_receive(&message, 2);
		}
		else
		{
			//printf("CAN0 message arrived in non-used mailbox\n\r");
		}

		//if(DEBUG_INTERRUPT)printf("message id: %d\n\r", message.id);
		//if(DEBUG_INTERRUPT)printf("message data length: %d\n\r", message.data_length);
		for (int i = 0; i < message.data_length; i++)
		{
			//if(DEBUG_INTERRUPT)printf("%d ", message.data[i]);
		}
		//printf("%d\n\r", message.data[0]);
		change_dt(message.data[0]);
		//printf("CAN0 message arrived in non-used mailbox%d\n\r", message.data[2]);
		if(message.data[2])
			PIOC->PIO_SODR |= PIO_PC14;
		else
			PIOC->PIO_CODR |= PIO_PC14;
		/*
		if(message.data[1] > 200)
		{
			PIOD->PIO_SODR |= PIO_PD9;
			//DACC->DACC_CDR = 0xfff;
			PIOD->PIO_SODR |= PIO_PD10;
		}
		else if(50 < message.data[1] && message.data[1] <= 200)
		{
			//DACC->DACC_CDR = 0x000;
			PIOD->PIO_CODR |= PIO_PD9;
		}
		else
		{
			//DACC->DACC_CDR = 0xfff;
			PIOD->PIO_SODR |= PIO_PD9;
			PIOD->PIO_CODR |= PIO_PD10;
		}
		*/
		slider_value = message.data[3];
		//if(DEBUG_INTERRUPT)printf("\n\r");
	}
	
	if(can_sr & CAN_SR_MB0)
	{
		//if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		
	//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;

	}

	if(can_sr & CAN_SR_ERRP)
	{
		//if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");

	}
	if(can_sr & CAN_SR_TOVF)
	{
		//if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");

	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
	//sei();*/
}
