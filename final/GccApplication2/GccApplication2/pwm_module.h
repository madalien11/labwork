#define PWM_FREQUENCY 1000
#define  PERIOD_VALUE 100
#define INIT_DUTY_VALUE 100
#define pwm_channel  5

void pwm_init();
void change_dt(uint8_t new_dt);