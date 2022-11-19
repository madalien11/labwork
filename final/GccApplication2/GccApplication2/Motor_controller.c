/*
 * Encoder.c
 *
 * Created: 11/16/2022 3:25:54 PM
 *  Author: User
 */ 

#include "can_interrupt.h"

#include <stdio.h>
#include "sam.h"
//#include <delay.h>
#include "printf-stdarg.h"
#include "uart.h"
#include "can_controller.h"
#include "pwm_module.h"

void delay(uint32_t ticks){
	SysTick->CTRL = 0x00000001;
	SysTick->LOAD = ticks;
	while( !(SysTick->CTRL & (1 << 16) ) ){} 
	
}

void motor_init(){
	
	//DACC init set speed
	PMC->PMC_PCER1 |=(1<<6);
	DACC->DACC_CR = 1; // reset
	DACC->DACC_WPMR = 0x44414300;
	DACC->DACC_MR |= (1 << 16);
	DACC->DACC_IDR = 0xffffffff;
	DACC->DACC_CHER = (1 << 1);
	DACC->DACC_CDR = 0xfff;
	//set motor enable high mj1 en pin30 pd9
	PMC->PMC_PCER0 |=(1<<14);
	PIOD->PIO_PER |= PIO_PD9; //pin enable register
	PIOD->PIO_OER |= PIO_PD9;// enable output on pin
	PIOD->PIO_PUDR |= PIO_PD9;
	PIOD->PIO_SODR |= PIO_PD9;
	//choose direction mj1 dir pin32 pd10
	PIOD->PIO_PER |= PIO_PD10; //pin enable register
	PIOD->PIO_OER |= PIO_PD10;// enable output on pin
	PIOD->PIO_PUDR |= PIO_PD10;
	
	
	
	//set output pins
	//!OE
	PIOD->PIO_PER |= PIO_PD0; //pin enable register
	PIOD->PIO_OER |= PIO_PD0;// enable output on pin
	PIOD->PIO_PUDR |= PIO_PD0;//disable pull up
	//SEL
	PIOD->PIO_PER |= PIO_PD2; //pin enable register
	PIOD->PIO_OER |= PIO_PD2;// enable output on pin
	PIOD->PIO_PUDR |= PIO_PD2;//disable pull up
	
	
	//set input pins
	PIOC->PIO_PER |= PIO_PC8 | PIO_PC7 | PIO_PC6 | PIO_PC5 | PIO_PC4 | PIO_PC3 | PIO_PC2 | PIO_PC1; //pin enable register
	PMC->PMC_PCER0 |=(1<<13);// 0xffffffff;// ID_PIOC;
	PIOC->PIO_ODR |= PIO_PC8 | PIO_PC7 | PIO_PC6 | PIO_PC5 | PIO_PC4 | PIO_PC3 | PIO_PC2 | PIO_PC1 ; // disable output on pin
	PIOC->PIO_PUDR |= PIO_PC8 | PIO_PC7 | PIO_PC6 | PIO_PC5 | PIO_PC4 | PIO_PC3 | PIO_PC2 | PIO_PC1 ;//disable pull up disable register
	//reset  pin26 pd1
	PIOD->PIO_PER |= PIO_PD1; //pin enable register
	PIOD->PIO_OER |= PIO_PD1;// enable output on pin
	PIOD->PIO_PUDR |= PIO_PD1;//disable pull up
	
	PIOD->PIO_SODR |= PIO_PD9;
	DACC->DACC_CDR = 0x7ff;
	PIOD->PIO_CODR |= PIO_PD10;
	delay(6000000);
	
	
	PIOD->PIO_CODR |= PIO_PD1;
	delay(100000);
	PIOD->PIO_SODR |= PIO_PD1;
	
	DACC->DACC_CDR = 0x7ff;
	
}

uint16_t read_encoder(){
	
	//!OE is pin 25 and thats PD0 set to low
	PIOD->PIO_CODR |= PIO_PD0;
	
	//sel is pin 27, PD2 set to low
	PIOD->PIO_CODR |= PIO_PD2;
	
	//wait for 20us
	delay(140000);
	
	//read from pin33-40, PC1-8 MJ2
	uint16_t mj2;
	mj2 =  (PIOC->PIO_PDSR & (PIO_PC8 | PIO_PC7 | PIO_PC6 | PIO_PC5 | PIO_PC4 | PIO_PC3 | PIO_PC2 | PIO_PC1)) << 8;
	//set sel to high
	PIOD->PIO_SODR |= PIO_PD2;
	//wait for 20us
	delay(140000);
	
	//read mj2 
	mj2 |= PIOC->PIO_PDSR & (PIO_PC8 | PIO_PC7 | PIO_PC6 | PIO_PC5 | PIO_PC4 | PIO_PC3 | PIO_PC2 | PIO_PC1);
	//set !oe to high 
	PIOD->PIO_SODR |= PIO_PD0;
	
	//printf("%d\n\r",mj2);
	if(mj2 > 65500 || mj2 < 60000)
		return 255;
	if(mj2 < 62700)
		return 0;
		
	return (mj2-62700)/11;
}