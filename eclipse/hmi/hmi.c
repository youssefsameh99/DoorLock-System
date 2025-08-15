/*
 * hmi.c
 *
 *  Created on: Oct 30, 2024
 *      Author: Sameh Fawzy
 */


#include "uart.h"
#include "std_types.h"
#include "common_macros.h"
#include "timer.h"
#include "lcd.h"
#include "gpio.h"
#include "keypad.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#define password_length 5
#define controlmcready  90
#define buzzeron        100
#define openingdoor     110
#define lockingdoor     120
#define repeat          130
#define timerovf        2
#define EnterPass       0
#define ConfirmPass     1
#define SendToMC2       2
#define Home            3
#define ConfirmWithMC2  4
#define opendoor        5
#define ChangePassword  6
#define wrongpass       7






uint8 password[password_length];
uint8 password2[password_length];
uint8 attempt;
uint8 timertick;
uint8 timerfinish;
uint8 error=0;
uint8 step=0;
uint8 function_button;
uint8 verify;
uint8 Locked;


void timercounter(void){
	timertick++;
}
void timerfunction(uint8 seconds){
	Timer_ConfigType Timer_Configurations = {0, 0 , TIMER1 , F_CPU_64 , NORMAL};
	Timer_init(&Timer_Configurations);
	Timer_setCallBack(timercounter,TIMER1);
	while ( timertick != (  (timerovf) * (seconds))){}
	Timer_deInit(TIMER1);
	if (timertick >= timerovf) {
		timertick = 0;
		timerfinish = 1;
	}
}
uint8 enterpassword1(void) {
	if(error == 1){
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,5,"Unmatched");
		_delay_ms(2000);
	}
	uint8 button ;


	LCD_clearScreen();
	LCD_displayString("Plz enter pass:");
	LCD_moveCursor(1,0);


	for(uint8 j=0;j<=password_length;j++){
		button=KEYPAD_getPressedKey();
		_delay_ms(250);
		if(j==password_length){
			while(button != '='){
				button=KEYPAD_getPressedKey();
				_delay_ms(250);
			}
		}
		password[j] = button ;
		LCD_displayCharacter('*');
	}
	return ConfirmPass;
}
uint8 checkmatch(void){
	error = 0;

	for(uint8 j=0;j<password_length;j++){
		if(password2[j] != password[j]){
			error=1;
			return EnterPass;
		}
	}
	return SendToMC2;
}

void enterpassword2(void) {
	uint8 button ;

	LCD_clearScreen();
	LCD_displayString("Plz re-enter the");
	LCD_displayStringRowColumn(1,0,"same pass: ");

	for(uint8 j=0;j<=password_length;j++){
		button=KEYPAD_getPressedKey();
		_delay_ms(250);

		if(j==password_length){
			while(button != '='){
				button=KEYPAD_getPressedKey();
				_delay_ms(250);
			}

		}
		password2[j] = button ;
		LCD_displayCharacter('*');
	}
}

void sendbyuart(void){

	for (uint8 j = 0; j < password_length; j++) {
		UART_sendByte(password[j]);
		_delay_ms(250);
	}
}

void DisplayHome(void){
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"+ : Open Door");
	LCD_displayStringRowColumn(1,0,"- : Change Pass");

	function_button= KEYPAD_getPressedKey();
	_delay_ms(250);

	if(function_button=='+' || function_button=='-' ){
		step= ConfirmWithMC2;
	}
	else step=Home;
}

uint8 Error_Handler(void) {
	attempt++;
	if (attempt >= 3) {
		Locked = 1;
		attempt=0;
		return wrongpass;
	}
	return 5;
}

uint8 checkpassword(void){
	if (verify == 0) {
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,5,"Unmatched");
		_delay_ms(1000);
		return Error_Handler();
	} else if (verify == 1) {
		LCD_clearScreen();
		LCD_displayString("matched");
		_delay_ms(1000);
		attempt=0;
		if(function_button=='+') return opendoor;
		else if(function_button=='-') return ChangePassword;
	}
	return 0;
}
uint8 systemlock(void){
	UART_sendByte(8);
	_delay_ms(200);

	LCD_clearScreen();
	LCD_displayString("System Locked ");
	timerfunction(60);

	Locked = 0;
	LCD_clearScreen();
	LCD_displayString("System on ");
	_delay_ms(1000);

	return Home;
}
uint8 OpeningDoor(void){
	LCD_clearScreen();
	LCD_displayString("Wait for people ");
	LCD_displayStringRowColumn(1,3,"To enter");

	UART_sendByte(0);
	_delay_ms(200);

	while(UART_recieveByte() != 1);

	LCD_clearScreen();
	LCD_displayString("Door is locking ");
	timerfunction(15);

	return Home;
}
void enterandsendpass(void){
	uint8 pressed_Key;
	for (uint8 j= 0; j <= password_length; j++) {
		pressed_Key = KEYPAD_getPressedKey();
		_delay_ms(250);
		if(j==password_length){
			while(pressed_Key != '='){
				pressed_Key=KEYPAD_getPressedKey();
				_delay_ms(250);
			}
			break;
		}
		password2[j] = pressed_Key ;
		LCD_displayCharacter('*');
	}

	for (uint8 j = 0; j < password_length; j++) {
		UART_sendByte(password2[j]);
		_delay_ms(200);
	}


}

int main(void){
	SREG|=(1<<7);
	UART_ConfigType uartconfig = {BITS_8,DISABLED , BITS_1 , 9600};
	LCD_init();
	UART_init(&uartconfig);
	while(1){
		switch(step){
		case EnterPass:
			step=enterpassword1();
			break;
		case ConfirmPass:
			enterpassword2();
			step=checkmatch();
			break;
		case SendToMC2:
			sendbyuart();
			step=Home;
			break;
		case Home:
			DisplayHome();
			break;
		case ConfirmWithMC2:
			LCD_clearScreen();
			LCD_displayString("Plz enter ur");
			LCD_displayStringRowColumn(1,0,"pass:");
			enterandsendpass();
			verify = UART_recieveByte();
			step=checkpassword();
			break;
		case opendoor:
			step=OpeningDoor();
			break;
		case ChangePassword:
			step=0;
			break;
		case wrongpass:
			step=systemlock();
			break;
		}
	}

}


