#include <Wire.h>
#include "Adafruit_VCNL4010.h"

Adafruit_VCNL4010 vcnl;

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

int led = 13;

void setup() {
  pinMode(led, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Waiting for start");

  
  
  Serial.println("VCNL4010 test");

  if (! vcnl.begin()){
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Found VCNL4010");

  Wire.begin();
}


void loop() {
   int val = vcnl.readProximity();
   
   Serial.print("Prox: ");
   Serial.println(val);
   
   delay(500);
   
   if (val >= 3000 || val <= 8000) {
     digitalWrite(led, HIGH);
   }
   else {
     digitalWrite(led, LOW);
   }
}

