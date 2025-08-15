/*
 * buzzer.c
 *
 *  Created on: Oct 8, 2024
 *      Author: Sameh Fawzy
 */
#include "common_macros.h"
#include "buzzer.h"
#include "gpio.h"


void Buzzer_init(void){
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,0);
}





void Buzzer_on(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,1);
}


void Buzzer_off(void){
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,0);
}




