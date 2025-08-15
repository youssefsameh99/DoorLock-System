/*
 * PIR.h
 *
 *  Created on: Oct 29, 2024
 *      Author: Sameh Fawzy
 */

#ifndef PIR_H_
#define PIR_H_
#include "std_types.h"
#define PIR_PORT_ID            PORTC_ID
#define PIR_PIN_ID             PIN2_ID

void PIR_init(void);

uint8 PIR_getState(void);


#endif /* PIR_H_ */
