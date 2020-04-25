#include <Wire.h>

#define MLXAddress 0x60;
#define EEPROM_Addres 0x50;

void setup() {
  // put your setup code here, to run once:
  Wire.begin()
  delay(100);
  Wire.beginTransmission(MLXAddress);

}

void loop() {
  // put your main code here, to run repeatedly:

}
