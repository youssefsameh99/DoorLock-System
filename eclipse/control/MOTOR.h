/*
 * MOTOR.h
 *
 *  Created on: Oct 8, 2024
 *      Author: Sameh Fawzy
 */

#ifndef MOTOR_H_
#define MOTOR_H_




#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define MOTOR_CHANNEL_ID      PORTD_ID
#define MOTOR_IN1_PIN_ID      PIN6_ID
#define MOTOR_IN2_PIN_ID      PIN7_ID


/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/


typedef enum {

	MOTOR_OFF , MOTOR_ON

}DcMotor_Logic;


typedef enum {

	CW , AW , STOP

}DcMotor_State;




/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initializes the DC motor:
 * 1. Setup the Motor pins directions by use the GPIO driver.
 * 2. Stop Motor before starting.
 */
void DcMotor_Init(void);

/*
 * Description :
 * Function responsible for Controling the motor's state (Clockwise/Anti-Clockwise/Stop) and adjusts the speed based
on the input duty cycle.
 *
 */


void DcMotor_Rotate(DcMotor_State state, uint8 speed);





#endif /* MOTOR_H_ */
