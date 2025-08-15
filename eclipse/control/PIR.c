/*
 * PIR.c
 *
 *  Created on: Oct 29, 2024
 *      Author: Sameh Fawzy
 */
#include "PIR.h"
#include "common_macros.h"
#include "gpio.h"


void PIR_init(void){
	GPIO_setupPinDirection(PIR_PORT_ID,PIR_PIN_ID,PIN_OUTPUT);
	GPIO_writePin(PIR_PORT_ID,PIR_PIN_ID,0);

}

uint8 PIR_getState(void){
	return GPIO_readPin(PIR_PORT_ID,PIR_PIN_ID);
}
