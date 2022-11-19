#include "sam3xa.h"
#include "pwm_module.h"
//#include "core_cm3.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void pwm_init(){
	PIOC->PIO_ABSR = (1 << 19);
	PIOC->PIO_PDR = (1 << 19);
	PMC->PMC_PCER1 |= (1 << 4);
	PWM->PWM_DIS = (1 << pwm_channel);
	PWM->PWM_CLK = (0x00000AFF);
	PWM->PWM_CH_NUM[pwm_channel].PWM_CMR = 0x0000020A;
	PWM->PWM_CH_NUM[pwm_channel].PWM_CDTY = 123; //DUTY_VALUE;
	//74 dt is lowest on the range, 172 dt highest on the range 
	PWM->PWM_CH_NUM[pwm_channel].PWM_CPRD = 1640;//PERIOD_VALUE;
	PWM->PWM_ENA = (1 << pwm_channel);
}

void change_dt(uint8_t new_dt){
	new_dt = (new_dt*98/255 + 74);
	if(new_dt>172)
		PWM->PWM_CH_NUM[pwm_channel].PWM_CDTY = 172;
	else if(new_dt < 74)
		PWM->PWM_CH_NUM[pwm_channel].PWM_CDTY = 74;
	else
		PWM->PWM_CH_NUM[pwm_channel].PWM_CDTY = new_dt;
}