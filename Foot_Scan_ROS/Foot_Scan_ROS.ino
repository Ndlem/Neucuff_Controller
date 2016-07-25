/*Nick Lemiesz, 7/11/16
 *Arduino Neucuff Leg Tracker
 *   The system consists of 6 input sensors: 1 IR beacon, 2 proximity readers, and 3 Force-Sensitive Resistors (FSRs).
 *   It is to track a person's walking with the various sensors and activate the Neucuff if necesary.
 *   There is also  a set of vibro-tacilte snesors that output a vibration for to give the user a bit of feedback.
 *   
 *   This is the version that should work with the ROS system. 
 *   
 *   Libraries used: 
 *      Sharp IR: http://playground.arduino.cc/Main/SharpIR
 *      Proximity Sensor: https://github.com/adafruit/Adafruit_VCNL4010
 *      I2C Library (turning different pins into SCL/SDA for more proximity sensors): http://playground.arduino.cc/Main/SoftwareI2CLibrary
 */
//ros packages
#define USE_USBCON
#include <ros.h>
#include <ArduinoHardware.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
 

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

//Node setup
ros::NodeHandle_<ArduinoHardware, 2, 2, 80, 105> nh;

//Publishers
std_msgs::Int8 msg_ir;

std_msgs::Float32 msg_prox;

//std_msgs::Int8 msg_fsr_1;
//std_msgs::Int8 msg_fsr_2;
//std_msgs::Int8 msg_fsr_3;

  //IR Publisher
  ros::Publisher IR("IR", &msg_ir);

  //Proximity Publishers
  ros::Publisher ProxF("Prox_Frt", &msg_prox);
 
  //FSR Publishers
  //ros::Publisher FSR1("FSR_1", &msg_fsr_1);
  //ros::Publisher FSR2("FSR_2", &msg_fsr_2);
  //ros::Publisher FSR3("FSR_3", &msg_fsr_3);


//Input
u8 IR_pin = A0;
u8 FSR = A1;

//Outputs
u8 led = 13;
u8 vib_1 = 11;

//Defining pins for SCL/SDA


//Check methods
//IR check
void call_IR (const std_msgs::Int8 &IR_Read) {
  //Checks if IR detector find foot out of bounds of 90 cm and within 15 cm
  if (IR_Read.data <= 15 || IR_Read.data >= 90) {
    digitalWrite(led, HIGH);
  }
}

//Proximity checks
void call_Prox (const std_msgs::Float32 &P_Read) {
  //Checks if Prox detector finds object within 10 mm
  if (P_Read.data <= 10.0) {
    digitalWrite(led, HIGH);
    digitalWrite(vib_1, HIGH);
  }
}

//FSR checks
void call_FSR (boolean FSR_read) {
  //Checks if the FSR is pressed
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

//Subscribers (callbacks above)
ros::Subscriber<std_msgs::Int8> IR_read("IR_data", &call_IR);
ros::Subscriber<std_msgs::Float32> Prox_read("Prox_data", &call_Prox);
//ros::Subscriber<std_msgs::Bool> FSR_read("FSR_data", &call_FSR);



//Setup methods
void setup() {
  //Input pin
  pinMode(FSR, INPUT);
  
  //Output pins
  pinMode(led, OUTPUT);
  //pinMode(vib_1, OUTPUT);
  
  Serial.begin(9600);
  delay(500);
  
  //Simple blink function to determine that the setup is done
  blink(250);

  if (! vcnl.begin()){
    Serial.println("Prox sensor(s) not found");
    while (1);
  }
  Serial.println("Found VCNL4010");

  //Setting up publishers
  nh.advertise(IR);
  
  nh.advertise(ProxF);
    
  //nh.advertise(FSR1);
  //nh.advertise(FSR2);
  //nh.advertise(FSR3);

  //Setting up subscribers
  nh.subscribe(IR_read);
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
  msg_ir.data = sharp.distance();
 //msg_prox_f.data = prox_convert(vcnl.readProximity());
 //boolean FSR_trig = bool_convert(analogRead(FSR));


  //Publish data
  IR.publish(&msg_ir);
  //ProxF.publish(&msg_prox_f);
  //FSR1.publish(&msg_fsr_1);
  
  nh.spinOnce();

  //Delay between sensor readings
  delay(1000);
}

//float prox_convert (float prox) {
  //Converts values of Proximity sensor (2^16, 0 to 65536) to millimeters (0 to 20)
  //float var = map(prox, 65536, 0, 0, 200);
  //var = var / 10;
  //return var;
//}

//boolean bool_convert (int value) {
  //Converts the value given by the FSR into a boolean
  //if (value <= 15){
    //return true;
  //}
  //else {
    //return false;
  //}
//}






