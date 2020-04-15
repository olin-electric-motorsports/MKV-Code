/*
 * Board: Accelerometer Module
 * Version: v1.0
 * Description: The goal of this board is to measure the frequency of vibrations in the chassis
 * in order to complete modal analysis.
 */

/****** Libraries ******/
#include <SPI.h>
#include <SD.h>

/****** LED Macros ******/
// Voltage Indicator LED
#define DDR_VLED DDRC
#define PORT_VLED PORTC
#define HIGH_VLED PC2
#define MED_VLED PC1
#define LOW_VLED PC0

// Status LED
#define DDR_SLED DDRD
#define PORT_SLED PORTD
#define ON_LED PD5
#define READY_LED PD6


/****** Button Macros ******/
#define BUTTON PD7
#define DDR_BUTTON DDRD


/****** SPI Macros ******/

#define MOSI PB3
#define MISO PB4
#define SCK PB5
// Accelerometer
#define DDR_SPI DDRB
#define PORT_SPI PORTB
#define ACC_CS PB2

#define READ 1
#define WRITE 0
#define MULTIPLE 1
#define SINGLE 0
#define RWbit 0 // Location of Read/Write Bit (Accelerometer)
#define MSbit 0 // Location of Multiple/Single Bit (Accelerometer)
#define WHOAMI 0x0F // Location if WHO AM I register (Accelerometer)

void setup() {
  unsigned int result = 0;
  
  DDR_SLED |= _BV(READY_LED);
  DDR_BUTTON &= ~_BV(BUTTON);
  DDR_VLED |= _BV(HIGH_VLED);

  // Accelerometer CS Pin
  DDR_SPI |= _BV(ACC_CS);

  // Set SPI pins at Output
  DDR_SPI |= _BV(SCK) | _BV(MOSI);

  
  if (result == 0b00110011) {
    PORT_VLED |= _BV(HIGH_VLED);
  }

  
//  PCICR |= _BV(PCIE2);
//  PCMSK2 |= _BV(PCINT23);
//  sei();
}

//ISR(PCINT2_vect) {
//  PORTD ^= _BV(PD6);
//}

void loop() {
  unsigned int result = 0;
  
  
  // Accelerometer Datasheet - Max Speed 1MHz
  // Page 27 - SPI MODE 0 - Output on Falling Edge, Data Capture on Rising Edge
  SPI.beginTransaction(SPISettings(400000, MSBFIRST, SPI_MODE0));
  PORT_SPI &= ~_BV(ACC_CS);
  result = SPI.transfer(0b10001111);
  SPI.endTransaction();
  

  
  
  if (result == 0b00110011) {
    PORT_SPI |= _BV(ACC_CS);
  }
  
  if (bit_is_set( PINB, PB1 )) {
    PORT_SLED ^= _BV(READY_LED);
    delay(1000);
  }
}
