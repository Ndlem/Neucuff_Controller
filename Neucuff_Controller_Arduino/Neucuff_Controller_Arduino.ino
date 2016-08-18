/*Nick Lemiesz, 8/16/16
  Arduino Neucuff Leg Tracker
     The system consists of 5 input sensors: 1 IR beacon, 1 proximity reader, and 3 Force-Sensitive Resistors (FSRs).
     It is to track a person's walking with the various sensors and activate the Neucuff if necessary.
     There is also a set of vibro-tacilte sensors that output a vibration for to give the user a bit of feedback (To be added).

     This system works exclusively with the systems built for Arduino. This is NOT the ROS version of the code.

     Libraries used:
        Sharp IR: http://playground.arduino.cc/Main/SharpIR
        Proximity Sensor: https://github.com/adafruit/Adafruit_VCNL4010

     Changes: 
     7/19/16
          Added button input that starts recording when held down, and stops when the button is at rest.
     7/25/16
          Added XBee support to output data in the form: [timestamp, IR, Prox, FL FSR, FR FSR, Bk FSR]
     8/16/16
          Cleaned up code a bit by taking out unnecessary parts and commenting out the check() methods. These are to be used
            later for activating the vibro-tactile sensors, as well as the solenoid valves to operate the airflow of the Neucuff.
*/

//SoftwareSerial package for using XBees
#include <SoftwareSerial.h>
SoftwareSerial XBee(2, 3);          // Set RX to pin 2 and TX to pin 3


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

//Outputs
u8 PUMP_IN = 7;
u8 PUMP_OUT = 8;

//(if needed)
u8 led = 13;

//Other
//Initialize timestamp (counter)
int timestamp = 0;

//Initalize counter (for testing solenoids)
int count = 0;

//Setup methods
void setup() {
  //Input pin
  pinMode(FSR_FL, INPUT);
  pinMode(FSR_FR, INPUT);
  pinMode(FSR_Bk, INPUT);
 
  //Output pins
  pinMode(PUMP_IN, OUTPUT);
  pinMode(PUMP_OUT, OUTPUT);
  
  //(If needed)
  pinMode(led, OUTPUT);
  

  //Xbee startup
  XBee.begin(9600);
  
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
    //Getting the values from the sensors
    //IR
    int IR_dis = sharp.distance();

    //Proximity
    float Prox_dis = prox_convert(vcnl.readProximity());

    //FSRs
    boolean FSR_FL_trig  = bool_convert(analogRead(FSR_FL));
    boolean FSR_FR_trig  = bool_convert(analogRead(FSR_FR));
    boolean FSR_Bk_trig  = bool_convert(analogRead(FSR_Bk));

    //Convert values into Strings to print to screen
    String ts = String(timestamp);
    String ir = String(IR_dis);
    String pr = String(Prox_dis);
    
    
    //Converting the FSR values
    String fsr_fl = String(FSR_FL_trig);
    String fsr_fr = String(FSR_FR_trig);
    String fsr_bk = String(FSR_Bk_trig);
    
    XBee.print("[" + ts + ", " + ir + ", " + pr + ", " + fsr_fl + ", " + fsr_fr + ", " + fsr_bk + "]");
    Serial.println("[" + ts + ", " + ir + ", " + pr + ", " + fsr_fl + ", " + fsr_fr + ", " + fsr_bk + "]");
    Serial.println();

//Testing the pumps
  if (count == 0){
    digitalWrite(PUMP_IN, LOW);
    digitalWrite(PUMP_OUT, HIGH);
  }
  else {
    digitalWrite(PUMP_IN, HIGH);
    digitalWrite(PUMP_OUT, LOW);
  }


  //Reset the counter
    if (count >= 2) {
      count = 0; 
    }


    /*//Checks the info from the sensors and activates the LED appropriately
    check_IR(IR_dis);
    check_Prox(Prox_dis);
    check_FSR(FSR_FL);
    check_FSR(FSR_FR);
    check_FSR(FSR_Bk);
    check_reset(IR_dis, Prox_dis, FSR_FL_trig, FSR_FR_trig, FSR_Bk_trig); */

    //Delay between sensor readings
    delay(500);
    timestamp = timestamp + 250;
  }

float prox_convert (float prox) {
  //Converts values of Proximity sensor (2^16, 0 to 65536) to millimeters (0 to 20)
  float var = map(prox, 65536, 0, 0, 2000);
  
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




/*//Check methods
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
}*/


