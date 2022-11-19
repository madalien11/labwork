/*
 * GccApplication2.c
 *
 * Created: 10/21/2022 2:55:28 PM
 * Author : User
 */ 


#include "sam.h"
#include "uart.h"
#include "can_controller.h"
#include "printf-stdarg.h"
#include "can_interrupt.h"
#include "can.h"
#include "pwm_module.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Motor_controller.h"
#include <math.h>

#define LED_ON REG_PIOA_ODSR |= PIO_PA19;
#define LED_OFF REG_PIOA_ODSR &= ~PIO_PA19;
#define ARRAYSIZE 100

void PIOC_Handler(void){
	
	printf("pin interrupted: \n\r");
	NVIC_ClearPendingIRQ(ID_PIOC);
}

int main(void)
{
    /* Initialize the SAM system */
    int error [ARRAYSIZE];
	double coeff[ARRAYSIZE];
	int score_flag=0;
	
	SystemInit();
	motor_init();
	configure_uart();
	//uint32_t can_br = CAN_BR_PHASE2(2) | CAN_BR_PHASE1(1) | CAN_BR_PROPAG(1) | CAN_BR_SJW(0) | CAN_BR_BRP(1342);
	can_init_def_tx_rx_mb((2 << CAN_BR_PHASE2_Pos) | (1 << CAN_BR_PHASE1_Pos) | (1 << CAN_BR_PROPAG_Pos) | (1 << CAN_BR_SJW_Pos) | (20 << CAN_BR_BRP_Pos) | (CAN_BR_SMP_ONCE));
	//can_init_def_tx_rx_mb(can_br);
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	uint32_t pin_status;
	PIOC->PIO_PER |= PIO_PC12; //pin enable register 
	PMC->PMC_PCER0 |=(1<<13);// 0xffffffff;// ID_PIOC;
	PIOC->PIO_ODR |= PIO_PC12;// disable output on pin
	PIOC->PIO_PUDR |= PIO_PC12;//disable pull up disable register
	pin_status = PIOC->PIO_PDSR & PIO_PDSR_P26; //read status
	//printf("hello");
	pwm_init();
	int c = 0;
	
	/*
	PIOC->PIO_IDR |= PIO_PC12;
	PIOC->PIO_ESR |= PIO_PC12;
	PIOC->PIO_FELLSR |= PIO_PC12;
	PIOC->PIO_AIMER |= PIO_PC12;
	
	NVIC_EnableIRQ((IRQn_Type) ID_PIOC);
	PIOC->PIO_IER |= PIO_PC12;
	*/
	
	PIOC->PIO_PER |= PIO_PC14; //pin enable register
	PIOC->PIO_OER |= PIO_PC14;// enable output on pin
	PIOC->PIO_PUDR |= PIO_PC14;//disable pull up disable register
	
	
	//PIOD->PIO_SODR |= PIO_PD10;
	double coeff_sum = 0;
	for (int i = 0; i<ARRAYSIZE ; i++)
	{
		coeff[i] = pow(2, -i);
		coeff_sum += coeff[i];
		//printf("%f\n\r",coeff[i]);
	}
	double scale = 16-((coeff_sum-1)*8);
	
	
	while(1){
		
		uint16_t encoder = read_encoder();
		
		//printf("%d %d\n\r",slider_value, encoder);
		
		
		for (int i=1; i<ARRAYSIZE; i++)
		{
			error[i] = error[i-1];
		}
		
		error[0] = slider_value-encoder;
		
		double error_sum = 0;
		for (int i = 0; i<ARRAYSIZE; i++)
		{
			error_sum += ((double)error[i])*coeff[i];
		}
		
		//int error_sum_abs = (int)error_sum
		
		if(error_sum > 0)
		{
			DACC->DACC_CDR = (int)(error_sum*scale);
			PIOD->PIO_CODR |= PIO_PD10;	
		}
		else
		{
			DACC->DACC_CDR = -1*((int)(error_sum*scale));
			PIOD->PIO_SODR |= PIO_PD10;	
		}
		pin_status = PIOC->PIO_PDSR  & PIO_PC12; //read status	
		//printf("photodiode is %d\n\r",pin_status);
		if(!pin_status && !score_flag)
		{
			c++;
			score_flag = 1;
			
			printf("counter is %d\n\r",c);
			//while(!pin_status){pin_status = PIOC->PIO_PDSR  & PIO_PC12;}
		}
		if(pin_status)
		{
			score_flag = 0;	
		}
		
		//printf("pin output status: %d\n\r",PIOB->PIO_OSR);
		
	}
	
	return 0;
}
