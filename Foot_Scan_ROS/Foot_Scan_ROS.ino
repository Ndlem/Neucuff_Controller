/*Nick Lemiesz, 7/5/16
 *Arduino Neucuff Leg Tracker
 *   The system consists of 6 input sensors: 1 IR beacon, 2 proximity readers, and 3 Force-Sensitive Resistors (FSRs).
 *   It is to track a person's wlaking with the various sensors and activate the Neucuff if necesary.
 *   There is also  a set of vibro-tacilte snesors that output a vibration for to give the user a bit of feedback.
 *   
 *   Libraries used: 
 *      Sharp IR: http://playground.arduino.cc/Main/SharpIR
 *      Proximity Sensor: https://github.com/adafruit/Adafruit_VCNL4010
 *      I2C Library (turning different pins into SCL/SDA): http://playground.arduino.cc/Main/SoftwareI2CLibrary
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

//I2C Library (In progress of intalling/learning how to use)
//#include <SoftI2C.h>


//Node setup
ros::NodeHandle_<ArduinoHardware, 2, 2, 80, 105> nh;

//Publishers
std_msgs::Int8 msg_ir;
std_msgs::Float32 msg_prox_f;
std_msgs::Float32 msg_prox_b;

std_msgs::Bool msg_fsr_1;
std_msgs::Bool msg_fsr_2;
std_msgs::Bool msg_fsr_3;

//IR Publisher
ros::Publisher IR("IR", &msg_ir);

//Proximity Publishers
ros::Publisher ProxF("Prox_Frt", &msg_prox_f);
//ros::Publisher ProxB("Prox_Bck", &msg_prox_b);

//FSR Publishers
ros::Publisher FSR1("FSR", &msg_fsr_1);
//ros::Publisher FSR2("FSR", &msg_fsr_2);
//ros::Publisher FSR3("FSR", &msg_fsr_3);

//Other declarations
u8 led = 13;
u8 FSR_1 = 12;
u8 FSR_2 = 11;
u8 FSR_3 = 10;
u8 vib_3 = 9;
u8 vib_2 = 8;
u8 vib_1 = 7;



//Callback methods
void IR_cb (const std_msgs::Int8 &IR_read) {
  //Checks if Prox detector finds object within 10 and 90 cm
  if (IR_read.data <= 10 || IR_read.data <= 90) {
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(led, LOW);
  }
}

void FSR1_cb (const std_msgs::Bool &FSR_read) {
  
}

void Prox_cb (const std_msgs::Float32 &Prox_read) {
  //Checks if Prox detector finds object within 2 and 18 mm
  if (true) {
    digitalWrite(led, HIGH);
    digitalWrite(vib_1, HIGH);
    //digitalWrite(vib_2, HIGH);
    //digitalWrite(vib_3, HIGH);
  }
  else {
    digitalWrite(led, LOW);
    digitalWrite(vib_1, LOW);
    //digitalWrite(vib_2, LOW);
    //digitalWrite(vib_3, LOW);
  }
}



//Subscribers (callbacks above)
ros::Subscriber<std_msgs::Int8> IR_read("IR_data", &IR_cb);
ros::Subscriber<std_msgs::Float32> Prox_read("Prox_data", &Prox_cb);
ros::Subscriber<std_msgs::Bool> FSR_read("FSR_data", &FSR1_cb);

//defining pins for SCL/SDA




//Setup methods
void setup() {
  //Output pins (LED/vibrators)
  pinMode(led, OUTPUT);
  pinMode(vib_1, OUTPUT);
  //pinMode(vib_2, OUTPUT);
  //pinMode(vib_3, OUTPUT);

  pinMode(FSR_1, INPUT);
  //pinMode(FSR_2, INPUT);
  //pinMode(FSR_3, INPUT);

  //Initalize node, then wait to advertise and subscribe
  nh.initNode();
  blink(250);
  delay(1000);

  //Setting up publishers
  nh.advertise(IR);
  
  nh.advertise(ProxF);
  //nh.advertise(ProxB);
  
  nh.advertise(FSR1);
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



//Main method
void loop() {
  //Getting the values from the sensors
  int IR_dis = sharp.distance();
  msg_ir.data = IR_dis;

  boolean FSR1_info = bool_convert(FSR_1);
  boolean FSR2_info = bool_convert(FSR_2);
  boolean FSR3_info = bool_convert(FSR_3);
  
  float Prox_dis = prox_convert(vcnl.readProximity());
  msg_prox_f.data = Prox_dis;


  //Publish data
  IR.publish(&msg_ir);
  FSR1.publish(&msg_fsr_1);
  ProxF.publish(&msg_prox_f);
  
  delay(100);
  nh.spinOnce();
  
  //Delay between sensor readings
  delay(150);
}



//Coversion methods
float prox_convert (float prox) {
  //Converts values of Proximity sensor (2^16, 0 to 65536) to millimeters (0 to 20)
  float var = map(prox, 65536, 0, 0, 200);
  var = var / 10;
  return var;
}

boolean bool_convert (int value) {
  if (value == 0){
    return true;
  }
  else {
    return false;
  }
}
