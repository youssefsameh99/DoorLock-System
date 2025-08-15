/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.c
 *
 * Description: Source file for the PWM signal control driver
 *
 * Author: Youssef Sameh
 *
 *******************************************************************************/

#include "common_macros.h"
#include "pwm.h"
#include "gpio.h"
#include <avr/io.h>

#define PWM_MAX_DUTY_CYCLE 255

/*
 * Description:
 * Initializes Timer0 in PWM mode and sets the required duty cycle.
 * - Prescaler: F_CPU/64
 * - Non-inverting mode
 * - Configures OC0 as the output pin.
 *
 * Parameters:
 *   - duty_cycle: Percentage (0 to 100%) representing the PWM duty cycle
 */
void PWM_TIMER0_Start(uint8 duty_cycle) {
    /* Calculate the compare value based on the duty cycle (0-100% mapped to 0-255) */
    OCR0 = (uint8)(((float)duty_cycle / 100.0) * 255);

    /* Configure TIMER0:
     * - Fast PWM mode (WGM01 and WGM00 set)
     * - Non-inverting mode (COM01 set)
     * - Prescaler of F_CPU/64 (CS01 and CS00 set)
     */
    TCCR0 = (1 << WGM01) | (1 << WGM00) | (1 << COM01) | (1 << CS01) | (1 << CS00);

    /* Configure OC0 (PWM output pin) as output */
    GPIO_setupPinDirection(PWM_OUTPUT_COMPARE_PORT_ID, PWM_OUTPUT_COMPARE_PIN_ID, PIN_OUTPUT);

    /* Initialize Timer0 counter */
    TCNT0 = 0;
}
