

Adafruit_INA260 ina260 = Adafruit_INA260();

void setup() {
  Serial.begin(9600);
  // Wait until serial port is opened
  while (!Serial) { delay(10); }

  Serial.println("Adafruit INA260 Test");

  if (!ina260.begin()) {
    Serial.println("Couldn't find INA260 chip");
    while (1);
  }
  Serial.println("Found INA260 chip");
}

void loop() {
  Serial.print(ina260.readCurrent());  x

  Serial.println();
  delay(1000);
}

