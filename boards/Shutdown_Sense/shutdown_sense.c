/*
Header:
	Code for the Shutdown Sense Board located in the low-voltage box
Author:
	@author Jack Greenberg
    @date   01/23/2020
*/

/*----- Includes -----*/
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "can_api.h"
#include "log_uart.h"

/*----- Macro Definitions -----*/
/* ATMega 16m1 Pins */
#define GLVMS                PD6
#define ESTOP                PD7
#define BSPD                 PB3
#define CRASH_SENSOR         PB4
#define BOTS                 PC7
#define ESTOP_DRIVER         PB5
#define HVD                  PB6
#define CONNECTOR_2_HVD      PB7
#define MAIN_PACK_CONNECTOR  PD0
#define BMS                  PC0
#define IMD                  PD1
#define TSMS                 PC1

/* CAN Message Positions */
// Should be from another file (CAN API)

#define UPDATE_STATUS          0

volatile uint8_t gTimerFlag = 0x01;
volatile uint8_t can_recv_msg[8] = {};

void initTimer(void) {
	TCCR0A = _BV(WGM01); // Set up 8-bit timer in CTC mode

	TCCR0B = 0x05; // Set clock prescaler to (1/1024) - page 89

	TIMSK0 |= _BV(OCIE0A); // Enable Match A interupts - page 90

	OCR0A = 0x27; //Makes the timer reset everytime it hits 39 (~100 Hz) - page 90
}

ISR(TIMER0_COMPA_vect) {
	gTimerFlag = _BV(UPDATE_STATUS);
}

ISR(CAN_INT_vect) {
	CANPAGE = (0 << MOBNB0);
	if (bit_is_set(CANSTMOB, RXOK)) {

	}
}

int main(void) {
	// Set up LEDs
	DDRB |= _BV(BSPD) | _BV(CRASH_SENSOR) | _BV(ESTOP_DRIVER) | _BV(HVD) | _BV(CONNECTOR_2_HVD);
	DDRC |= _BV(BOTS) | _BV(BMS) | _BV(TSMS); 
	DDRD |= _BV(MAIN_PACK_CONNECTOR)| _BV(IMD);

	sei(); // Initializes interrupts
	initTimer();

	// Pin Change Interrupt Control Register
	// Allow pin interupts on PCIE2
	PCICR |= _BV(PCIE2);

	// Enable interupts on PCINT22 and 23
	PCMSK2 |= _BV(PCINT22) | _BV(PCINT23);

	CAN_init(CAN_ENABLED);

	int delay = 50;

	while(1) {
		PORTB ^= _BV(BSPD);
		_delay_ms(delay);
		PORTB ^= _BV(CRASH_SENSOR);
		_delay_ms(delay);
		PORTC ^= _BV(BOTS);
		_delay_ms(delay);
		PORTB ^= _BV(ESTOP_DRIVER);
		_delay_ms(delay);
		PORTB ^= _BV(HVD);
		_delay_ms(delay);
		PORTB ^= _BV(CONNECTOR_2_HVD);
		_delay_ms(delay);
		PORTD ^= _BV(MAIN_PACK_CONNECTOR);
		_delay_ms(delay);
		PORTC ^= _BV(BMS);
		_delay_ms(delay);
		PORTD ^= _BV(IMD);
		_delay_ms(delay);
		PORTC ^= _BV(TSMS);
		_delay_ms(delay);
		PORTC ^= _BV(TSMS);
		_delay_ms(delay);
		PORTD ^= _BV(IMD);
		_delay_ms(delay);
		PORTC ^= _BV(BMS);
		_delay_ms(delay);
		PORTD ^= _BV(MAIN_PACK_CONNECTOR);
		_delay_ms(delay);
		PORTB ^= _BV(CONNECTOR_2_HVD);
		_delay_ms(delay);
		PORTB ^= _BV(HVD);
		_delay_ms(delay);
		PORTB ^= _BV(ESTOP_DRIVER);
		_delay_ms(delay);
		PORTC ^= _BV(BOTS);
		_delay_ms(delay);
		PORTB ^= _BV(CRASH_SENSOR);
		_delay_ms(delay);
		PORTB ^= _BV(BSPD);
		_delay_ms(delay);
	}
}
