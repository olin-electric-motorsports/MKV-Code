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

void initTimer(void) {
	TCCR0A = _BV(WGM01); // Set up 8-bit timer in CTC mode

	TCCR0B = 0x05; // Set clock prescaler to (1/1024) - page 89

	TIMSK0 |= _BV(OCIE0A); // Enable Match A interupts - page 90

	OCR0A = 0x27; //Makes the timer reset everytime it hits 39 (~100 Hz) - page 90
}

ISR(TIMER0_COMPA_vect) {
	gTimerFlag = _BV(UPDATE_STATUS);
}

// void turnOnLED(int LED, int PORT) {
// 	PORT |= _BV(LED);
// }

// void initADC(void) {
// 	//Get the Analog to Digital Converter started (ADC)
//     ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS0);

//     //Enable interal reference voltage
//     ADCSRB &= _BV(AREFEN);

//     //Set internal reference voltage as AVCC
//     ADMUX |= _BV(REFS0);

//     //Reads by default from ADC0 (pin 11)
//     ADMUX |= _BV(0x00);
// }

int main(void) {
	// Set up LEDs
	DDRB |= _BV(BSPD) | _BV(CRASH_SENSOR) | _BV(ESTOP_DRIVER) | _BV(HVD) | _BV(CONNECTOR_2_HVD);
	DDRC |= _BV(BOTS) | _BV(BMS) | _BV(TSMS); 
	DDRD |= _BV(MAIN_PACK_CONNECTOR)| _BV(IMD);

	sei(); // Initializes interrupts
	initTimer();

	// Pin Change Interrupt Control Register
	// Allow pin interupts on PCIE0
	PCICR |= _BV(PCIE0);

	// Enable interupts on PCINT22 and 23
	PCMSK0 |= _BV(PCINT22) | _BV(PCINT23);

	// initADC();
	CAN_init(CAN_ENABLED);

	// PORTB |= _BV(BSPD) | _BV(CRASH_SENSOR) | _BV(ESTOP_DRIVER) | _BV(HVD) | _BV(CONNECTOR_2_HVD);
	// PORTC |= _BV(BOTS) | _BV(BMS) | _BV(TSMS);
	// PORTD |= _BV(MAIN_PACK_CONNECTOR)| _BV(IMD);

	// PORTB |= _BV(CONNECTOR_2_HVD);

	while(1) {
		// PORTB |= _BV(CONNECTOR_2_HVD);
		// _delay_ms(500);
		// PORTB &= ~_BV(CONNECTOR_2_HVD);
		// _delay_ms(500);

		PORTB ^= _BV(BSPD);
		_delay_ms(1000);
		PORTB ^= _BV(CRASH_SENSOR);
		_delay_ms(1000);
		PORTC ^= _BV(BOTS);
		_delay_ms(1000);
		PORTB ^= _BV(ESTOP_DRIVER);
		_delay_ms(1000);
		PORTB ^= _BV(HVD);
		_delay_ms(1000);
		PORTB ^= _BV(CONNECTOR_2_HVD);
		_delay_ms(10000);
		PORTD ^= _BV(MAIN_PACK_CONNECTOR);
		_delay_ms(1000);
		PORTC ^= _BV(BMS);
		_delay_ms(1000);
		PORTD ^= _BV(IMD);
		_delay_ms(1000);
		PORTC ^= _BV(TSMS);
		_delay_ms(1000);

	}
}
