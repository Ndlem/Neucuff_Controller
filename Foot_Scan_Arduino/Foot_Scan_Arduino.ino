/*Nick Lemiesz, 7/19/16
  Arduino Neucuff Leg Tracker
     The system consists of 5 input sensors: 1 IR beacon, 1 proximity reader, and 3 Force-Sensitive Resistors (FSRs).
     It is to track a person's walking with the various sensors and activate the Neucuff if necesary.
     There is also  a set of vibro-tacilte snesors that output a vibration for to give the user a bit of feedback.

     This system works exclusively with the systems built for Arduino. This is NOT hte ROS version of the code.

     Libraries used:
        Sharp IR: http://playground.arduino.cc/Main/SharpIR
        Proximity Sensor: https://github.com/adafruit/Adafruit_VCNL4010

     Current build: 7/19/16
        Added button input that starts recording when held down, and stops when the button is at rest.
*/
//IR package
#include <SharpIR.h>
SharpIR sharp(0, 25, 93, 1080);

//Proximity package
#include <Wire.h>
#include "Adafruit_VCNL4010.h"
Adafruit_VCNL4010 vcnl;
#if defined(ARDUINO_ARCH_SAMD)
#define Serial SerialUSB
#endif

//Input
u8 IR = A0;
u8 FSR_FL = A1;
u8 FSR_FR = A2;
u8 FSR_Bk = A3;
int buttonPin = 8;

//Outputs
u8 led = 13;

//Other
//Initialize timestamp (counter)
int timestamp = 0;

int val = 0;

//Setup methods
void setup() {
  //Input pin
  pinMode(FSR_FL, INPUT);
  pinMode(FSR_FR, INPUT);
  pinMode(FSR_Bk, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  //Output pins
  pinMode(led, OUTPUT);

  Serial.begin(9600);
  delay(500);

  //Simple blink function to determine that the setup is done
  blink(250);

  if (! vcnl.begin()) {
    Serial.println("Prox sensor(s) not found");
    while (1);
  }
  Serial.println("Found VCNL4010");
}

void blink(int d) {
  digitalWrite(led, HIGH);
  delay(d);
  digitalWrite(led, LOW);
  delay(d);
  digitalWrite(led, HIGH);
  delay(d);
  digitalWrite(led, LOW);
}



//Main methods
void loop() {
  val = digitalRead(buttonPin);

  if (val == LOW) {
    //Getting the values from the sensors
    //IR
    int IR_dis = sharp.distance();

    //Proximity
    float Prox_dis = prox_convert(vcnl.readProximity());

    //FSRs
    boolean FSR_FL_trig  = bool_convert(analogRead(FSR_FL));
    boolean FSR_FR_trig  = bool_convert(analogRead(FSR_FR));
    boolean FSR_Bk_trig  = bool_convert(analogRead(FSR_Bk));

    //Print info to serial monitor
    Serial.print("Timestamp (ms): ");
    Serial.println(timestamp);

    //IR Sensor
    Serial.print("Infared (cm): ");
    Serial.println(IR_dis);

    //Proximity Sensor
    Serial.print("Prox (mm): ");
    Serial.println(Prox_dis);

    //FSR Sensors
    Serial.println("FSRs pressed: ");

    Serial.print("  Front Left: ");
    if (FSR_FL_trig == true) {
      Serial.println("Yes");
    }
    else {
      Serial.println("No");
    }


    Serial.print("  Front Right: ");
    if (FSR_FR_trig == true) {
      Serial.println("Yes");
    }
    else {
      Serial.println("No");
    }

    Serial.print("  Back: ");
    if (FSR_FL_trig == true) {
      Serial.println("Yes");
    }
    else {
      Serial.println("No");
    }

    Serial.println();
    Serial.println();


    //Checks the info from the sensors and activates the LED appropriately
    //check_IR(IR_dis);
    //check_Prox(Prox_dis);
    //check_FSR(FSR_FL);
    //check_FSR(FSR_FR);
    //check_FSR(FSR_Bk);
    //check_reset(IR_dis, Prox_dis, FSR_FL_trig, FSR_FR_trig, FSR_Bk_trig);

    //Delay between sensor readings
    delay(250);
    timestamp = timestamp + 250;
  }
}

float prox_convert (float prox) {
  //Converts values of Proximity sensor (2^16, 0 to 65536) to millimeters (0 to 20)
  float var = map(prox, 65536, 0, 0, 200);
  var = var / 10;
  return var;
}

boolean bool_convert (int value) {
  //Converts the value given by the FSR into a boolean
  if (value <= 10) {
    return true;
  }
  else {
    return false;
  }
}




//Check methods
void check_IR (int IR_Read) {
  //Checks if IR detector find foot out of bounds of 90 cm and within 15 cm
  if (IR_Read <= 15 || IR_Read >= 90) {
    digitalWrite(led, HIGH);
  }
}

void check_Prox (int P_Read) {
  //Checks if Prox detector finds object within 10 mm
  if (P_Read <= 10.0) {
    digitalWrite(led, HIGH);
  }
}

void check_FSR (int FSR_read) {
  //Cehcks if the FSR is pressed
  if (FSR_read == 0) {
    digitalWrite(led, HIGH);
  }
}

void check_reset(int IR_dis, float Prox_dis, int FSR_FL_trig, int FSR_FR_trig, int FSR_Bk_trig) {
  //Calls for a reset if the values are back to normal
  if ((IR_dis > 15 && IR_dis < 90) && (Prox_dis > 10) && (FSR_FL_trig != 0) && (FSR_FR_trig != 0) && (FSR_Bk_trig != 0)) {
    digitalWrite(led, LOW);
  }
}


