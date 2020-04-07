#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "log_uart.h"
#include "can_api.h"
#COMMENT; get rid of the includes you don't need
#COMMENT; you should always run python3 make.py and just say n for flash to make sure you code has a clean build

// gFlag
#define UPDATE_STATUS 0

#define MOB_WHEEL_SPEED_SEND 0
#define CLOCK_SPEED_HZ 50 #COMMENT; remove, unused

volatile uint8_t gFlag = 0x00;
#COMMENT; wheel_speed_current_count can be a uint8_t
// look at cells F2:F5 in this spreadsheet https://docs.google.com/spreadsheets/d/15h0nwC2RD0I5i1UGHuLMANMKE26jd5kQXHEKAXZubcw/edit#gid=0
// max tick frequency is 880Hz, I want to report the speed at 200Hz (see time init comment)
// so there's no way we'll exceed 255 ticks per report
#ENDCOMMENT;
uint16_t wheel_speed_current_count = 0; #COMMENT; make volatile because it's edited in an interrupt
uint8_t wheel_speed_msg[2] = {0, 0}; #COMMENT; no change needed here, length is still 2 becuase 0th byte is for error codes, see MKV CAN address space on drive

ISR(PCINT0_vect) {
    // Interrupt for pin PB4
    // Whenever PB4 is triggered, flip the LED
    #COMMENT; remove the next two lines of comments after ENDTODO
    // PB5 is the sense_in pin
    // not necessary to add any additional logic in this interrupt since
    // PB5 (PCINT5) is the only interrupt enabled (see PCMSK0 in main)
    #ENDCOMMENT;
    // TODO: Only flip LED when PB4 is triggered, not anything
    // on PCINT0
    PORTB ^= _BV(PB0); #COMMENT; make macros LED_WRITE_REGISTER -> PORTB and LED_PIN -> PB0
    wheel_speed_current_count += 1;
}

void initTimer(void) {
  #COMMENT; don't use hex here (makes it harder to read), also update comments
  // I know we copied and pasted this from my old code but I was dumb back then haha
  // here's how I would write this now, also changed OCR0A to 20 (0x14) to get ~200Hz
  TCCR0A = _BV(WGM01); // Clear Timer on Compare match (CTC) mode
  TCCR0B |= _BV(CS02) | _BV(CS00); // clkio/1024 prescaler
  OCR0A = 20; // set value of compare match A
  // 4MHz CPU, prescaler 1024, compare match on 20 -> (4000000/1024/20) ~= 196 Hz
  TIMSK0 |= _BV(OCIE0A); // enable interrupt on compare match A
  #ENDCOMMENT;
	TCCR0A = _BV(WGM01);   // Set up 8-bit timer in CTC mode
	TCCR0B = 0x05;         // clkio/1024 prescaler
	TIMSK0 |= _BV(OCIE0A); // Every 1024 cycles, OCR0A increments
	OCR0A = 0x4e; //dec 78  // until 0xff, 255, which then calls for
	// the TIMER0_COMPA_vect interrupt
	// currently running at 100Hz
}

ISR(TIMER0_COMPA_vect) {
  #COMMENT; get rid of this multi-line comment since it's better said in the initTimer function above
	/*
	   Timer/Counter0 compare match A
	   If the clock frequency is 4MHz then this is called 16 times per second
	   MATH: (4MHz/1024)/255 = ~16
	 */
	gFlag |= _BV(UPDATE_STATUS);
}

#COMMENT; make sure you check the can_api header file (can_api.h), the macro names in this function aren't right
// probably should just do this once in the beginning of main(), it shouldn't change during operation
// you can make CAN_ID a global uint16_t and run this function once with the init stuff in main() before the while loop
// you can set the length here too, even though it's the same regardless of location, just makes it easier/clearer
#ENDCOMMENT;
uint16_t getCANID() {
    /**
     * Check which board this is and set the ID based on that
     */
    #COMMENT; front_set and left_set not declared with type, will throw error on build
    front_set = bit_is_set(PINB, PB3); #COMMENT; make macros FRONT_READ_REGISTER -> PINB and FRONT_PIN -> PB3
    left_set = bit_is_set(PINB, PB4); #COMMENT; make macros LEFT_READ_REGISTER -> PINB and LEFT_PIN -> PB4

    if(front_set && left_set) {
        return CAN_ID_WHEEL_SPEED_FRONT_LEFT #COMMENT; probably need a semicolon here
    } else if (front_set && !left_set) {
        return CAN_ID_WHEEL_SPEED_FRONT_RIGHT #COMMENT; probably need a semicolon here
    } else if (!front_set && left_set) {
        return CAN_ID_WHEEL_SPEED_BACK_LEFT #COMMENT; probably need a semicolon here
    } else if (!front_set && !left_set) {
        return CAN_ID_WHEEL_SPEED_BACK_RIGHT #COMMENT; probably need a semicolon here
    }
    #COMMENT; you need to have a default return
    // our compiler isn't smart enough to see that one of those if statements will necessarily get hit
    #ENDCOMMENT;
}

void reportSpeed() {
    /**
     * Report speed over CAN
     */
    #COMMENT; looks like you're trying to turn the 16 bit integer into two 8-bit integers
    // your logic isn't right though
    // either way you won't need to do this since it won't exceed 255 at the 200Hz frequency anyway
    // see above comments on the declaration of wheel_speed_current_count and wheel_speed_msg and the timer init
    // so this should look like...
    wheel_speed_msg[0] = 0x00; // no error codes defined yet
    wheel_speed_msg[1] = wheel_speed_current_count;
    // but here's what it would look like if you did need to do this...
    wheel_speed_msg[0] = wheel_speed_current_count & 0xff; // get the low byte
    wheel_speed_msg[1] = wheel_speed_current_count >> 8; // get the high byte
    // another time when things are harder to read when you use hex
    // same code but easier to read might be...
    wheel_speed_msg[0] = wheel_speed_current_count & 0b11111111; // get the low byte
    wheel_speed_msg[1] = wheel_speed_current_count >> 8; // get the high byte
    #ENDCOMMENT;
    wheel_speed_msg[0] = wheel_speed_current_count & 0x0f;
    wheel_speed_msg[1] = wheel_speed_current_count & 0xf0 >> 8;

    #COMMENT; if you change getCANID to only run once and set a global and also set length similarly then don't forget to update this line accordingly
    CAN_transmit(MOB_WHEEL_SPEED_SEND, getCANID(), CAN_LEN_WHEEL_SPEED, wheel_speed_msg); #COMMENT; check can_api.h, CAN_LEN_WHEEL_SPEED not defined, won't build

    wheel_speed_current_count = 0; #COMMENT; do this right after setting wheel_speed_msg (before sending CAN message) to ensure no lost counts
}


int main(void) {
    // Set LED to output
    DDRB |= _BV(PB0); #COMMENT; make macros LED_DDR -> DDRB and LED_PIN -> PB0
    
    // Enable interrupts
    sei();
    PCICR |= _BV(PCIE0);
    PCMSK0 |= _BV(PCINT5);

    // UART logging
    LOG_init(); #COMMENT; comment this out or remove it since you're not using it
    
    #COMMENT; you need to init timer and CAN
    initTimer();
    CAN_init(CAN_ENABLED);
    #ENDCOMMENT;

    while(1) {
        if(bit_is_set(gFlag, UPDATE_STATUS)) {
            #COMMENT; unset the update status flag here
            // gFlag &= ~_BV(UPDATE_STATUS);
            // otherwise after the first time it gets set we'll just run the code in this
            // if block constantly
            #ENDCOMMENT;
            reportSpeed();
        }
    }
}

