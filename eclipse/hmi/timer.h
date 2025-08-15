/*
 * timer.h
 *
 *  Created on: Oct 30, 2024
 *      Author: Sameh Fawzy
 */


#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	TIMER0,TIMER1 , TIMER2
}Timer_ID_Type;

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024,EX_FAILING,EX_RISING
}Timer_ClockType;

typedef enum
{
	 NORMAL , COMPARE ,PWM ,FAST_PWM, COMPAREA , COMPAREB
}Timer_ModeType;



typedef struct
{
uint16 timer_InitialValue;
uint16 timer_compare_MatchValue;     /*it will be used in compare mode only*/
Timer_ID_Type  timer_ID;
Timer_ClockType timer_clock;
Timer_ModeType  timer_mode;
}Timer_ConfigType;



/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/


/*
 * Description :
 * Function to initialize the Timer driver
 * 1.Set timer id
 * 2.Set the timer mode type
 * 3.Choose the pre-scalar
 * 4-choose each of intial value or compare match according to the mode type
 * Inputs: pointer to the configuration structure with type Timer_ConfigType.
 *
 */

void Timer_init(const Timer_ConfigType * Config_Ptr)  ;

/*
 * Description :
 * Function to disable the Timer via Timer_ID.
 *
 * Inputs: Timer_ID
 *
 */

void Timer_deInit(Timer_ID_Type timer_type);

/*
 * Description :
 * Function to set the Call Back function address to the required Timer.
 *
 * Inputs: pointer to Call Back function and Timer Id you want to set The Callback to it.
 *
 */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID );



#endif /* TIMER_H_ */

