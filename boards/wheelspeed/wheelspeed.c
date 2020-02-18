#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "log_uart.h"
#include "can_api.h"

// gFlag
#define UPDATE_STATUS 0

#define MOB_WHEEL_SPEED_SEND 0
#define CLOCK_SPEED_HZ 50

volatile uint8_t gFlag = 0x00;
uint16_t wheel_speed_current_count = 0;
uint8_t wheel_speed_msg[2] = {0, 0};

ISR(PCINT0_vect) {
    // Interrupt for pin PB4
    // Whenever PB4 is triggered, flip the LED
    // TODO: Only flip LED when PB4 is triggered, not anything
    // on PCINT0
    PORTB ^= _BV(PB0);
    wheel_speed_current_count += 1;
}

void initTimer(void) {
	TCCR0A = _BV(WGM01);   // Set up 8-bit timer in CTC mode
	TCCR0B = 0x05;         // clkio/1024 prescaler
	TIMSK0 |= _BV(OCIE0A); // Every 1024 cycles, OCR0A increments
	OCR0A = 0x4e; //dec 78  // until 0xff, 255, which then calls for
	// the TIMER0_COMPA_vect interrupt
	// currently running at 100Hz
}

ISR(TIMER0_COMPA_vect) {
	/*
	   Timer/Counter0 compare match A
	   If the clock frequency is 4MHz then this is called 16 times per second
	   MATH: (4MHz/1024)/255 = ~16
	 */
	gFlag |= _BV(UPDATE_STATUS);
}

void reportSpeed() {
    wheel_speed_msg[0] = wheel_speed_current_count & 0x0f;
    wheel_speed_msg[1] = wheel_speed_current_count & 0xf0 >> 8;

    CAN_transmit(MOB_WHEEL_SPEED_SEND, CAN_ID_WHEEL_SPEED, CAN_LEN_WHEEL_SPEED, wheel_speed_msg);

    wheel_speed_current_count = 0;
}


int main(void) {
    // Set LED to output
    DDRB |= _BV(PB0);
    
    // Enable interrupts
    sei();
    PCICR |= _BV(PCIE0);
    PCMSK0 |= _BV(PCINT5);

    // UART logging
    LOG_init();

    while(1) {
        if(bit_is_set(gFlag, UPDATE_STATUS)) {
            reportSpeed();
        }
    }
}

