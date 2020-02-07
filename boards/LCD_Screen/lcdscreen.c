#include <avr/io.h>

/*
using an or function, compare DDRB with PC_pos, so it turns a zero
*/
DDRB |= _BV(PC0);


int main(void) {
  PORTB |= BV(PC0);
}
