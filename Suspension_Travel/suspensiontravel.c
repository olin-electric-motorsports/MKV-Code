#include <avr/io.h>

int main (void) {
    DDRB |= _BV(PC0);
    
    while(1) {
        PORTB |= _BV(PC0);
    }
}
