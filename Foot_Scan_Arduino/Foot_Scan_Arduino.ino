/*Nick Lemiesz, 7/7/16
 *Arduino Neucuff Leg Tracker
 *   The system consists of 6 input sensors: 1 IR beacon, 2 proximity readers, and 3 Force-Sensitive Resistors (FSRs).
 *   It is to track a person's wlaking with the various sensors and activate the Neucuff if necesary.
 *   There is also  a set of vibro-tacilte snesors that output a vibration for to give the user a bit of feedback.
 *   
 *   This system works exclusively with the systems built for Arduino. This is NOT hte ROS version of the code. 
 *   
 *   Libraries used: 
 *      Sharp IR: http://playground.arduino.cc/Main/SharpIR
 *      Proximity Sensor: https://github.com/adafruit/Adafruit_VCNL4010
 *      I2C Library (turning different pins into SCL/SDA for more proximity sensors): http://playground.arduino.cc/Main/SoftwareI2CLibrary
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


//I2C Library (In progress)
//#include <SoftI2C.h>

//Input
u8 IR = A0;
u8 FSR = A1;

//Outputs
u8 led = 13;
u8 vib_1 = 11;


//Defining pins for SCL/SDA


//Setup methods
void setup() {
  //Input pin
  pinMode(FSR, INPUT);
  
  //Output pins
  pinMode(led, OUTPUT);
  pinMode(vib_1, OUTPUT);
  
  Serial.begin(9600);
  delay(500);
  
  //Simple blink function to determine that the setup is done
  blink(250);

  if (! vcnl.begin()){
    Serial.println("Prox sensor(s) not found");
    while (1);
  }
  Serial.println("Found VCNL4010");
}

void blink(int d){
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
  int IR_dis = sharp.distance();
  float Prox_dis = prox_convert(vcnl.readProximity());
  boolean FSR_trig = bool_convert(analogRead(FSR));
  
  //Print info to serial monitor
  //IR Sensor
  Serial.print("Infared (CM): ");
  Serial.println(IR_dis);

  //Proximity Sensor
  Serial.print("Prox (MM): ");
  Serial.println(Prox_dis);

  //FSR Sensor
  Serial.print("FSR Pressed?: ");
  if (FSR_trig){
   Serial.println("Yes");    
  }
  else {
   Serial.println("No");
  }
  Serial.println("");
  
  //Checks the info from the sensors and activates the LED appropriately
  check_IR(IR_dis);
  check_Prox(Prox_dis);
  check_FSR(FSR_trig);
  check_reset(IR_dis, Prox_dis, FSR_trig);

  //Delay between sensor readings
  delay(1000);
}

float prox_convert (float prox) {
  //Converts values of Proximity sensor (2^16, 0 to 65536) to millimeters (0 to 20)
  float var = map(prox, 65536, 0, 0, 200);
  var = var / 10;
  return var;
}

boolean bool_convert (int value) {
  //Converts the value given by the FSR into a boolean
  if (value <= 15){
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
    digitalWrite(vib_1, HIGH);
  }
}

void check_FSR (boolean FSR_read) {
  //Cehcks if the FSR is pressed
  if (FSR_read == true) {
    digitalWrite(led, HIGH);
  }
}

void check_reset(int IR_dis, float Prox_dis, boolean FSR_trig) {
  //Calls for a reset if the values are back to normal
  if ((IR_dis > 15 && IR_dis < 90) && (Prox_dis > 10) && (FSR_trig == false)) {
    digitalWrite(led, LOW);
  }
}

