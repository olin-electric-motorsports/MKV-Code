/*
i2c protocol
*/

/*
* Both pins must be digital pins that have interrupt capabilities
* 
* SCL must have a clock
*/
void init(volatile uint8_t *SDA_PORT, uint8_t SDA_PIN, volatile uint8_t *SCL_PORT, uint8_t SCL_PIN) {
	if (SDA_PORT == &PORTB) {
        DDRB |= _BV(SDA_PIN);
		PCICR |= _BV(PCIE0);
		PCSMK0 |= _BV(SDA_PIN);
    } else if (SDA_PORT == &PORTC) {
        DDRC |= _BV(SDA_PIN);
		PCSMK1 |= _BV(SDA_PIN);
    } else if (SDA_PORT == &PORTD) {
        DDRD |= _BV(SDA_PIN);
		PCSMK2 |= _BV(SDA_PIN);
    } else if (SDA_PORT == &PORTE) {
        DDRE |= _BV(SDA_PIN);
		PCSMK3 |= _BV(SDA_PIN);
    }

	if (SCL_PORT == &PORTB) {
        DDRB |= _BV(SCL_PIN);
		PCSMK0 |= _BV(SDA_PIN);
    } else if (SCL_PORT == &PORTC) {
        DDRC |= _BV(SCL_PIN);
		PCSMK1 |= _BV(SDA_PIN);
    } else if (SCL_PORT == &PORTD) {
        DDRD |= _BV(SCL_PIN);
		PCSMK2 |= _BV(SDA_PIN);
    } else if (SCL_PORT == &PORTE) {
        DDRE |= _BV(SCL_PIN);
		PCSMK3 |= _BV(SDA_PIN);
    }
}
