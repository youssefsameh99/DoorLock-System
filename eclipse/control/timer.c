 /*
 * timer.c
 *
 *  Created on: Oct 30, 2024
 *      Author: Sameh Fawzy
 */


#include "timer.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h> /* For  ISR */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr0)(void) = NULL_PTR;
static volatile void (*g_callBackPtr1)(void) = NULL_PTR;
static volatile void (*g_callBackPtr2)(void) = NULL_PTR;
//static volatile Timer_ID_Type g_Timer_Type = 0;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr0 != NULL_PTR)
	{
		/* Call the Call Back function in the application  */
		(*g_callBackPtr0)();
	}
}


ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr0 != NULL_PTR)
	{
		/* Call the Call Back function in the application  */
		(*g_callBackPtr0)();
	}
}



ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr1 != NULL_PTR)
	{
		/* Call the Call Back function in the application  */
		(*g_callBackPtr1)();
	}
}




ISR(TIMER1_COMPB_vect)
{
	if(g_callBackPtr1 != NULL_PTR)
	{
		/* Call the Call Back function in the application  */
		(*g_callBackPtr1)();
	}
}



ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr1 != NULL_PTR)
	{
		/* Call the Call Back function in the application  */
		(*g_callBackPtr1)();
	}
}



ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtr2 != NULL_PTR)
	{
		/* Call the Call Back function in the application  */
		(*g_callBackPtr2)();
	}
}


ISR(TIMER2_OVF_vect)
{
	if(g_callBackPtr2 != NULL_PTR)
	{
		/* Call the Call Back function in the application  */
		(*g_callBackPtr2)();
	}
}



/*******************************************************************************
 *                      Functions Definitions                                  *
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

void Timer_init(const Timer_ConfigType * Config_Ptr)  {


    if ( Config_Ptr->timer_ID == TIMER0){

    	/* configure the timer
    	 * 1. Non PWM mode FOC0=1
    	 *2-MODE configurable
    	 * 3. clock configurable
    	 * 4-intial value
    	 */

    	TCCR0 = (1<<FOC0) ;
    	//TCCR0 =  (TCCR0 & 0xB7) | (((Config_Ptr->timer_mode) & (0XFD)) << 2) | (((Config_Ptr->timer_mode) & (0XFE)) << 6) ;
    	switch(Config_Ptr->timer_mode){

    	   case NORMAL:

    		   TCCR0 &= ~(1<< WGM01) & ~(1<<WGM00);/*WGM00=0 ,WGM01=0 */
       		   TIMSK |= (1<<TOIE0); // Enable Timer0 NORMAL Interrupt
    		   break;

    	   case COMPARE:

    		   TCCR0 |= (1<< WGM01) ;/*WGM00=0 ,WGM01=1 */
    		   TCCR0 &= ~(1<<WGM00);

     		  /*No need for OC0 in this example so COM00=0 & COM01=0*/
     		  OCR0 = (uint8)Config_Ptr->timer_compare_MatchValue ;
     		  TIMSK |= (1<<OCIE0); // Enable Timer0 Compare Interrupt

    		   break;

    	}

    	TCCR0 =  (TCCR0 & 0xF8) | ((Config_Ptr->timer_clock)  & (0x07) )  ;
    	TCNT0 =Config_Ptr->timer_InitialValue ;





     } else if (Config_Ptr->timer_ID == TIMER1){

     	/* configure the timer
     	 * 1. Non PWM mode FOC1A and F0C1B= 1
     	 *2-MODE configurable
     	 * 3. clock configurable
     	 * 4-intial value
     	 */

    	 TCCR1A = (1<<FOC1A) | (1<<FOC1B) ;
    	 //TCCR1A =  (TCCR1A & 0xFC) ;
    	// TCCR1B =   (TCCR1B & 0xE7) | (((Config_Ptr->timer_mode) & (0XFD)) << 2) | (((Config_Ptr->timer_mode) & (0XFE)) << 4);

     	switch(Config_Ptr->timer_mode){

     	   case NORMAL:

     		  TCCR1A &= ~(1<< WGM11) & ~(1<< WGM10);/*WGM11=0 ,WGM10=0 */
     		  TCCR1B &= ~(1<<  WGM13) & ~(1<<  WGM12);/*WGM13=0 , WGM12=0 */

              TIMSK |= (1<<TOIE1); // Enable Timer1 NORMAL Interrupt

     		   break;

     	   case COMPARE:

     		  TCCR1A &= ~(1<< WGM11) & ~(1<< WGM10);/*WGM11=0 ,WGM10=0 */
     		  TCCR1B &= ~(1<<  WGM13) ;/*WGM13=0 , WGM12=1*/
     		  TCCR1B |= (1<<  WGM12);

     		   break;

     	   case COMPAREA:

     		  TCCR1A &= ~(1<< WGM11) & ~(1<< WGM10);/*WGM11=0 ,WGM10=0 */
     		  TCCR1B &= ~(1<<  WGM13) ;/*WGM13=0 , WGM12=1*/
     		  TCCR1B |= (1<<  WGM12);
    		  /*No need for OC1A and OC1B in this example so COM1A0 and COM1B0=0 & COM1A1 and COM1B1=0*/
     		 OCR1A = (uint8)Config_Ptr->timer_compare_MatchValue ;
     		TIMSK |= (1<<OCIE1A);// Enable Timer1 CompareA Interrupt

     		   break;

     	   case COMPAREB:

     		  TCCR1A &= ~(1<< WGM11) & ~(1<< WGM10);/*WGM11=0 ,WGM10=0 */
     		  TCCR1B &= ~(1<<  WGM13) ;/*WGM13=0 , WGM12=1*/
     		  TCCR1B |= (1<<  WGM12);
     		  OCR1B = (uint8)Config_Ptr->timer_compare_MatchValue ;


         	  TIMSK |=  (1<<OCIE1B); // Enable Timer1 CompareB Interrupt
     		   break;

     	}

    	 TCCR1B =  (TCCR1B & 0xF8) | ((Config_Ptr->timer_clock)  & (0x07) )   ;
    	 TCNT1  =Config_Ptr->timer_InitialValue ;





     } else if (Config_Ptr->timer_ID == TIMER2){

    	 /* configure the timer
    	   * 1. Non PWM mode FOC2=1
    	   *2-MODE configurable
    	    * 3. clock configurable
    	   * 4-intial value
    	    */
    	 TCCR2 = (1<<FOC2) ;
    	// TCCR2 =  (TCCR0 & 0xB7) | (((Config_Ptr->timer_mode) & (0XFD)) << 2) | (((Config_Ptr->timer_mode) & (0XFE)) << 6) ;

     	switch(Config_Ptr->timer_mode){

     	   case NORMAL:

     		  TCCR2 &= ~(1<< WGM20) & ~(1<<WGM21);/*WGM20=0 ,WGM21=0 */
       		  TIMSK |= (1<<TOIE2); // Enable Timer2 NORMAL Interrupt
     		    break;

     	   case COMPARE:

      		  TCCR2 &= ~(1<< WGM20);/*WGM20=0 ,WGM21=1 */
     		  TCCR2 &= ~(1<<WGM21);
     		  /*No need for OC2 in this example so COM00=0 & COM01=0*/
     		 OCR2 =(uint8) Config_Ptr->timer_compare_MatchValue ;
    		 TIMSK |= (1<<OCIE2); // Enable Timer2 Compare Interrupt
     		   break;

     	}

    	 TCCR2 =  (TCCR0 & 0xF8) | ((Config_Ptr->timer_clock)  & (0x07) )  ;
    	 TCNT2 = Config_Ptr->timer_InitialValue ;





     }

}

/*
 * Description :
 * Function to disable the Timer via Timer_ID.
 *
 * Inputs: Timer_ID
 *
 */

void Timer_deInit(Timer_ID_Type timer_type){

	 if ( timer_type == TIMER0){

		 TCCR0 &= 0xF8 ; /*Disable clock to pause the Timer0*/
		  TIMSK &= 0xFC;  /* Disable interrupt */
			TCNT0=0;
			OCR0=0;



	 }else if (timer_type== TIMER1){

		 TCCR1B &= 0xF8 ; /*Disable clock to pause the Timer1*/
		  TIMSK &= 0xE3;  /* Disable interrupt */
			TCCR1A = 0;
			TCNT1 = 0;
			OCR1A = 0;
			OCR1B = 0;


	 }else if (timer_type == TIMER2){


		 TCCR2 &= 0xF8 ; /*Disable clock to pause the Timer2*/
		  TIMSK &= 0x3F;  /* Disable interrupt */
			TCNT2 = 0;
			OCR2 = 0;


	 }

}

/*
 * Description :
 * Function to set the Call Back function address to the required Timer.
 *
 * Inputs: pointer to Call Back function and Timer Id you want to set The Callback to it.
 *
 */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID ){


	if (a_timer_ID==TIMER0){

	/* Save the address of the Call back function in a global variable */
		g_callBackPtr0 = a_ptr;

	} else if (a_timer_ID==TIMER1){

		/* Save the address of the Call back function in a global variable */
		g_callBackPtr1 = a_ptr;


	}else if (a_timer_ID==TIMER2){

		/* Save the address of the Call back function in a global variable */
		g_callBackPtr2 = a_ptr;


	}



}



