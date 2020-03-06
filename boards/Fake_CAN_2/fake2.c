/*----- Includes -----*/
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "can_api.h"

/*----- MACROS -----*/
#define LED1		PB0
#define LED2		PB1
#define LED3     	PB6

#define LED_PORT    PORTB

#define MOB_BRAKE        0
#define MOB_AIR_CRIT     1
#define MOB_DASH     	2

/*----- gFlag -----*/
#define UPDATE_STATUS       0
#define FLAG_AIRPLUS_AUX    1
#define FLAG_AIRMINUS_AUX   2

volatile uint8_t gFlag = 0x00; // Global Flag
volatile uint8_t LEDtimer = 0x00;

uint8_t dash_msg[8] = {0,0,0,0,0,0,0,0};
uint8_t brake_msg[8] = {0,0,0,0,0,0,0,0};

ISR(TIMER0_COMPA_vect) {
	/*
	   Timer/Counter0 compare match A
	   If the clock frequency is 4MHz then this is called 16 times per second
	   MATH: (4MHz/1024)/255 = ~16
	 */
	gFlag |= _BV(UPDATE_STATUS);
	// LEDtimer++;
	// if(LEDtimer > 100) {
	//  LEDtimer = 0;
	//  LED_PORT ^= _BV(LED2);
	// }
}


ISR(CAN_INT_vect) {
	PORTB ^= _BV(LED2);
	CANPAGE = (MOB_AIR_CRIT << MOBNB0); // Switch to MOb 0, the one we're listening on.
	if(bit_is_set(CANSTMOB, RXOK)) {

		// PORTB ^= _BV(LED2);

		dash_msg[0] = CANMSG;

		// if (dash_msg[0] > 0x7F) {
		// 	PORTB ^= _BV(LED1);
		// } else {
		// 	PORTB &= ~_BV(LED1);
		// }
		
		// volatile uint8_t msg = CANMSG;     //grab the first byte of the CAN message

		// if(msg == 0xFF) {
		//  PORTB ^= _BV(LED2);
		// } else {
		// PORTB &= ~_BV(LED2);
		// }

		//Setup to Receive Again
		CANSTMOB = 0x00;
		CAN_wait_on_receive(MOB_AIR_CRIT, CAN_ID_AIR_CONTROL_CRITICAL, CAN_LEN_AIR_CONTROL_CRITICAL, 0xFF);

	}
}


// ISR(PCINT1_vect) {
//  if(bit_is_set(PINC,PC6)) {
//      gFlag |= _BV(FLAG_AIRPLUS_AUX);
//  } else {
//      gFlag &= ~_BV(FLAG_AIRPLUS_AUX);
//  }
//  if(bit_is_set(PINC,PC7)) {
//      gFlag |= _BV(FLAG_AIRMINUS_AUX);
//  } else {
//      gFlag &= ~_BV(FLAG_AIRMINUS_AUX);
//  }
//
// }


void initTimer(void) {
	TCCR0A = _BV(WGM01);   // Set up 8-bit timer in CTC mode
	TCCR0B = 0x05;         // clkio/1024 prescaler
	TIMSK0 |= _BV(OCIE0A); // Every 1024 cycles, OCR0A increments
	OCR0A = 0x27; //dec 39  // until 0xff, 255, which then calls for
	// the TIMER0_COMPA_vect interrupt
	// currently running at 100Hz
}


int main (void) {
	initTimer();
	sei(); //Inititiates interrupts for the ATMega
	CAN_init(CAN_ENABLED);
	//CAN_wait_on_receive(0, CAN_ID_TUTORIAL6, CAN_LEN_TUTORIAL6, 0xFF);

	// Enable interrupt
	// PCICR |= _BV(PCIE0) | _BV(PCIE1);

	//Sets these pins at outputs
	DDRB |= _BV(LED1) | _BV(LED2);
	DDRB |= _BV(LED3);

	CAN_wait_on_receive(MOB_AIR_CRIT, CAN_ID_AIR_CONTROL_CRITICAL, CAN_LEN_AIR_CONTROL_CRITICAL, 0xFF);

	uint8_t count = 0;

	while(1) {
		if(bit_is_set(gFlag, UPDATE_STATUS)) {
			gFlag &= ~_BV(UPDATE_STATUS);

			count++;

			if (count == 0xFF) {
				count = 0;
			}

			dash_msg[0] = count;

			CAN_transmit(MOB_DASH, CAN_ID_DASHBOARD, CAN_LEN_DASHBOARD, dash_msg);
			// PORTD ^= _BV(LED1);
			// PORTD ^= _BV(LED2);
			// PORTB ^= _BV(LED3);
			// _delay_ms(500);


			// if(bit_is_set(gFlag,FLAG_AIRPLUS_AUX)){
			//     LED_PORT |=  _BV(LED1);
			// msg[0] = 0x0F;
			// CAN_transmit(MOB_AIR_CRIT,CAN_ID_AIR_CONTROL_CRITICAL,CAN_LEN_AIR_CONTROL_CRITICAL,msg);
			// _delay_ms(2000);
			// air_msg[0] = 0xFF;
			// CAN_transmit(MOB_AIR_CRIT,CAN_ID_AIR_CONTROL_CRITICAL,CAN_LEN_AIR_CONTROL_CRITICAL,air_msg);
			// LED_PORT ^= _BV(LED2);
			//
			//
			// brake_msg[2] = 0xFF;
			// CAN_transmit(MOB_BRAKE,CAN_ID_BRAKE_LIGHT,CAN_LEN_BRAKE_LIGHT,brake_msg);
			// LED_PORT ^= _BV(LED2);
			// _delay_ms(5000);

			// } else {
			//     msg[0] = 0x00;
			//     LED_PORT &= ~_BV(LED1);
			// }




		}
	}
}
