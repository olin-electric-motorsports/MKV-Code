#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "log_uart.h"

#define TIME_COUNT_MS 100
#define HOLES_PER_REV 1

uint16_t current_count = 0;
float rpm = 0;
char uart_report_msg[6*20+4+5] = "";

ISR(PCINT0_vect) {
    // Interrupt for pin PB4
    // Whenever PB4 is triggered, flip the LED
    // TODO: Only flip LED when PB4 is triggered, not anything
    // on PCINT0
    PORTB ^= _BV(PB1);
    current_count += 1;
}

uint16_t get_speed(uint16_t counts) {
    /**
     * Calculates RPS from the counts in the last timestep
     *
     * Returns: RPS * 1000
     */
    return (uint16_t) ((double) counts / (double) HOLES_PER_REV) / ((double) TIME_COUNT_MS / 1000.0) * 1000;
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
        sprintf(uart_report_msg, "%u count, %u rpm", current_count, get_speed(current_count));
        LOG_println(uart_report_msg, strlen(uart_report_msg));

        // Every time step clear the current count
        current_count = 0;

        _delay_ms(TIME_COUNT_MS);
    }
}

