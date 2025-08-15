/*
 * control.c
 *
 *  Created on: Oct 30, 2024
 *      Author: Sameh Fawzy
 */


#include "uart.h"
#include "std_types.h"
#include "common_macros.h"
#include "timer.h"
#include "pir.h"
#include "buzzer.h"
#include "MOTOR.h"
#include "external_eeprom.h"
#include "twi.h"
#include "pwm.h"
#include "gpio.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>


#define passwordlength 5
#define timerovf       2
#define controlmcready 90
#define buzzeron        100
#define openingdoor     110
#define lockingdoor     120
#define repeat          130



uint8 password1[passwordlength];
uint8 password2[passwordlength];
uint8 attempts=0;
uint8 timertick=0;
uint8 timer_finish;
uint8 match;
uint8 pir;
uint8 val;

void timerticker(void) {
	timertick++;
}


void timerfunction (uint8 seconds) {

	Timer_ConfigType Timer_Configurations = {0, 0, TIMER1, F_CPU_64, NORMAL};


	Timer_init(&Timer_Configurations);


	Timer_setCallBack(timerticker, TIMER1);


	while (timertick != ((timerovf) * (seconds))) {}


	Timer_deInit(TIMER1);


	timertick = 0;


	if (timertick >= timerovf) {
		timertick = 0;
		timer_finish = 1;
	}
}
void recievepass(uint8* password) {
	for (uint8 i = 0; i < passwordlength; i++) {
		password[i] = UART_recieveByte();
		_delay_ms(200);
	}

}
void SaveinEEPROM(uint8* password) {
	for (uint8 i = 0; i < passwordlength; i++) {
		EEPROM_writeByte(0x01 + i, password[i]);
		_delay_ms(20);
	}
}
uint8 Verify_password(uint8* password) {
	uint8 val;
	for (uint8 i = 0; i < passwordlength; i++) {
		EEPROM_readByte(0x01 + i, &val);
		_delay_ms(20);
		if (val != password[i]) {
			return 0;
		}
	}
	return 1;
}
void opendoor(void) {
	DcMotor_Rotate(CW, 100);
	timerfunction(15);
	pir = PIR_getState();
	DcMotor_Rotate(STOP, 0);
	_delay_ms(500);
	while (pir != 0) {
		pir = PIR_getState();
		_delay_ms(500);
	}

	UART_sendByte(1);

	DcMotor_Rotate(AW, 100);

	timerfunction(15);

	DcMotor_Rotate(STOP, 100);

}
void turnonbuzzer(void) {
	Buzzer_on();


	timerfunction(60);

	Buzzer_off();
}
uint8 check_pass(void){
	uint8 val;
		for (uint8 i = 0; i < passwordlength; i++) {
			EEPROM_readByte(0x01 + i, &val);
			_delay_ms(20);
			if (val != password2[i]) {
				return 0;
			}
		}
		return 1;






}





int main(void){
	SREG|=(1<<7);
	UART_ConfigType UART_Configurations = {BITS_8, DISABLED, BITS_1, 9600};
	TWI_ConfigType TWI_Configurations = {0,400000, 0x01};
	UART_init(&UART_Configurations);
	TWI_init(&TWI_Configurations);
	PIR_init();
	DcMotor_Init();
	Buzzer_init();
	uint8 step=0;
	while(1){

		switch(step){
		case 0:
			recievepass(password1);
			SaveinEEPROM(password1);
			step=1;
			break;
		case 1:
			recievepass(password2);
			match=check_pass();

			UART_sendByte(match);
			_delay_ms(200);

			val=UART_recieveByte();
			if(val==0){
				step=openingdoor;
			}
			else if(val==8) {
				step=buzzeron;
			}
			break;
		case openingdoor:
			opendoor();
			step=1;
			break;
		case buzzeron:
			turnonbuzzer();
			step=1;
			break;
		}

	}

}
