#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "log_uart.h"

#define TIME_COUNT_MS 100

uint16_t current_count = 0;
char uart_report_msg[6*20+4+5] = "";

ISR(PCINT0_vect) {
    // Interrupt for pin PB4
    // Whenever PB4 is triggered, flip the LED
    // TODO: Only flip LED when PB4 is triggered, not anything
    // on PCINT0
    PORTB ^= _BV(PB1);
}

int main(void) {
    // Set LED to output
    DDRB |= _BV(PB1);
    
    // Enable interrupts
    sei();
    PCICR |= _BV(PCIE0);
    PCMSK0 |= _BV(PCINT4);

    // UART logging
    LOG_init();

    while(1) {
        // Report data over UART
        sprintf(uart_report_msg, "%i", current_count);
        LOG_println(uart_report_msg, strlen(uart_report_msg));
        _delay_ms(TIME_COUNT_MS);

        // Every (insert time step here) clear the current count
        current_count = 0;
    }
}

