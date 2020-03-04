#include <avr/io.h>
#include <util/delay.h>

void ADC_init(void) {
    /* Get the Analog to Digital Converter started (ADC)
     * Set ADC Enable, and set AD Prescaler to 0x101
     * Divides clock frequency by 32 for AD clock */
    ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS0);

    //Enable interal reference voltage
    ADCSRB &= _BV(AREFEN);

    //Set internal reference voltage as AVcc
    ADMUX |= _BV(REFS0);
}

uint16_t readPotentiometer (void) {

		ADMUX = _BV(REFS0);
		ADMUX |= 8; // Potentiomenter pin is ADC8
		ADCSRA |= _BV(ADSC);
		loop_until_bit_is_clear(ADCSRA, ADSC);
		uint16_t val = ADC;

		// Uncomment when we need it. -Corey
		// Don't forget to uncomment the .h file at the top
		//LOG_init();
		//char uart_buf[64];
  	// sprintf(uart_buf, "Cooling Pressure: %d", val);
  	// LOG_println(uart_buf, strlen(uart_buf));

    return val;
}

void sendCANMessage (uint8_t msg[]) {

  CAN_transmit(MOB_BROADCAST_CRITICAL,
								CAN_ID_AIR_CONTROL_CRITICAL,
								CAN_LEN_AIR_CONTROL_CRITICAL,
								msg);
}

int main (void) {
    ADC_init();
    DDRB |= _BV(PB5) | _BV(PB6) | _BV(PB7);
    // DDRB |= _BV(PB6);
    // DDRB |= _BV(PB7);
    uint16_t potVolt = 0;

    while(1) {
        PORTB ^= _BV(PB5);
        potVolt = readPotentiometer();

        if(potVolt > 615) {
            PORTB |= _BV(PB6);
        } else {
            PORTB &= ~_BV(PB6);
        }

        _delay_ms(500);
    }


}
