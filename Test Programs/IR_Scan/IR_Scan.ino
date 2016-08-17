int sensorpin = 0;                 // analog pin used to connect the sharp sensor
int val = 0;                      // variable to store the values from sensor(initially zero)
int val_1 = 0; int val_2 = 0; int val_3 = 0; int val_4 = 0; int val_5 = 0; 
int led = 13;

void setup()
{
  Serial.begin(9600);               // starts the serial monitor
  pinMode(led, OUTPUT);
}
  
void loop() {
  val = analogRead(sensorpin);       // reads the value of the sharp sensor
  Serial.println(val);            // prints the value of the sensor to the serial monitor
  delay(100);                    // wait for this much time before printing next value
  if (val >= 200){
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(led, LOW);
  }
}
