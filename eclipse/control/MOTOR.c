/*
 * MOTOR.c
 *
 *  Created on: Oct 8, 2024
 *      Author: Sameh Fawzy
 */

#include "common_macros.h" /* For GET_BIT Macro */
#include "MOTOR.h"
#include "gpio.h"
#include "pwm.h"


/*
 * Description :
 * Initializes the DC motor:
 * 1. Setup the Motor pins directions by use the GPIO driver.
 * 2. Stop Motor before starting.
 */
void DcMotor_Init(void) {

	/* Configure the direction for H-Bridge pins as output pins */
		GPIO_setupPinDirection(MOTOR_CHANNEL_ID,MOTOR_IN1_PIN_ID ,PIN_OUTPUT ) ;
		GPIO_setupPinDirection(MOTOR_CHANNEL_ID,MOTOR_IN2_PIN_ID ,PIN_OUTPUT ) ;


		/* Turn OFF the DC Motor on the required pins.  */
	    GPIO_writePin(MOTOR_CHANNEL_ID,MOTOR_IN1_PIN_ID ,MOTOR_OFF);
		GPIO_writePin(MOTOR_CHANNEL_ID,MOTOR_IN2_PIN_ID ,MOTOR_OFF);
		PWM_TIMER0_Start ( 0 );

}


/*
 * Description :
 * Function responsible for Controling the motor's state (Clockwise/Anti-Clockwise/Stop) and adjusts the speed based
on the input duty cycle.
 *
 */


void DcMotor_Rotate(DcMotor_State state, uint8 speed) {

	PWM_TIMER0_Start ( speed );

	if ( state == STOP ){

	    GPIO_writePin(MOTOR_CHANNEL_ID,MOTOR_IN1_PIN_ID ,MOTOR_OFF);
		GPIO_writePin(MOTOR_CHANNEL_ID,MOTOR_IN2_PIN_ID ,MOTOR_OFF);



	}
	else if ( state == CW ){

	    GPIO_writePin(MOTOR_CHANNEL_ID,MOTOR_IN1_PIN_ID ,MOTOR_OFF);
		GPIO_writePin(MOTOR_CHANNEL_ID,MOTOR_IN2_PIN_ID ,MOTOR_ON);


	}else if ( state == AW ) {

	    GPIO_writePin(MOTOR_CHANNEL_ID,MOTOR_IN1_PIN_ID ,MOTOR_ON);
		GPIO_writePin(MOTOR_CHANNEL_ID,MOTOR_IN2_PIN_ID ,MOTOR_OFF);


	}


}







