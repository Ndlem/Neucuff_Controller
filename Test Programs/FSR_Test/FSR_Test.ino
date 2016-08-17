int LED = 13;           //Set up the LED on pin 13
int FSR = A0;           //Set up the FSR on pin A0

void setup() {
  Serial.begin (9600);
  pinMode (FSR, INPUT);
  pinMode (LED, OUTPUT);
}

void loop() {
  int val = analogRead(FSR);
  if (val <= 15) {
    digitalWrite(LED, HIGH);
  }
  else {
    digitalWrite(LED, LOW);
  }
  Serial.println(val);
  delay(500);
}
