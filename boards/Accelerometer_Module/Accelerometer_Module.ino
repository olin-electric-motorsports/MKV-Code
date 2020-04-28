/*
 * Board: Accelerometer Module
 * Version: v1.0
 * Description: The goal of this board is to measure the frequency of vibrations in the chassis
 * in order to perform modal analysis.
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


/****** I2C Macros ******/
#define INA260_ADDR 1000000
#define VOLTAGE_REG 0x02

// Variable Declaration
File data;
int xyzData[6];

void setup() {

  // TODO: Add SD.begin(___)
  
  DDR_SLED |= _BV(READY_LED);
  DDR_BUTTON &= ~_BV(BUTTON);
  DDR_VLED |= _BV(HIGH_VLED);

  // Accelerometer CS Pin
  DDR_SPI |= _BV(ACC_CS);

  // Set SPI pins at Output
  DDR_SPI |= _BV(SCK) | _BV(MOSI);

  
//  PCICR |= _BV(PCIE2);
//  PCMSK2 |= _BV(PCINT23);
//  sei();
}

//ISR(PCINT2_vect) {
//  PORTD ^= _BV(PD6);
//}

void loop() {
  
  memset(xyzData,0,6); // set xyzData to zeros

  // TODO: Turn READ operation into function
  // Accelerometer Datasheet - Max Speed 1MHz
  // Page 27 - SPI MODE 0 - Output on Falling Edge, Data Capture on Rising Edge
  SPI.beginTransaction(SPISettings(400000, MSBFIRST, SPI_MODE0));
  PORT_SPI &= ~_BV(ACC_CS);
  xyzData[ 0 ] = SPI.transfer(0b10101000); // OUT_X_L
  PORT_SPI |= _BV(ACC_CS);
  PORT_SPI &= ~_BV(ACC_CS);
  xyzData[ 1 ] = SPI.transfer(0b10101001); // OUT_X_H
  PORT_SPI |= _BV(ACC_CS);
  PORT_SPI &= ~_BV(ACC_CS);
  xyzData[ 2 ] = SPI.transfer(0b10101010); // OUT_Y_L
  PORT_SPI |= _BV(ACC_CS);
  PORT_SPI &= ~_BV(ACC_CS);
  xyzData[ 3 ] = SPI.transfer(0b10101011); // OUT_Y_H
  PORT_SPI |= _BV(ACC_CS);
  PORT_SPI &= ~_BV(ACC_CS);
  xyzData[ 4 ] = SPI.transfer(0b10101100); // OUT_Z_L
  PORT_SPI |= _BV(ACC_CS);
  PORT_SPI &= ~_BV(ACC_CS);
  xyzData[ 5 ] = SPI.transfer(0b10101101); // OUT_Z_H
  PORT_SPI |= _BV(ACC_CS);
  SPI.endTransaction();

  
  // creates/opens file "data.txt"
  // starts writing at END of file
  data = SD.open("data.txt", FILE_WRITE);
  for ( int i = 0; i < 3; i++ ) {
    data.println(xyzData[ i*2 ] >> 6 | xyzData[ (i*2)+1 ] << 2);
  }
  data.close();
  

  // I2C Slave Address (INA260) -- 1000000
  // Bus Voltage register (0x03)
  // Alert Limit register (0x07)
 
  
  if (bit_is_set( PINB, PB1 )) {
    PORT_SLED ^= _BV(READY_LED);
    delay(1000);
  }
}
