
int analogPin = A0;
int val = 0;

void setup() {
  Serial.begin(9600);

}

void loop() {
  val = analogRead(analogPin);  // read the input pin
  Serial.println(val);

}
