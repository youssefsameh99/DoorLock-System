

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"

#define PWM_OUTPUT_COMPARE_PORT_ID    PORTB_ID
#define PWM_OUTPUT_COMPARE_PIN_ID     PIN3_ID


/*
 * Description :
 * Function responsible to initialize TIMER0 PWM signal and set duty cycle
 */
void PWM_TIMER0_Start (uint8 duty_cycle);


#endif /* PWM_H_ */
