/**Foot_Scan_ROS_FSRs
  Nick Lemiesz
  Last Edited: 7/15/16

  Upon uploading and running, the program publishes the three FSR data points (found with analogRead()). 
  0 means the FSR is pressed down and any other values means it's not.
  Sensors:
   There are 3 sensors currently built inot this program: the 3 FSRs.
   They are aptly named fsr_FL, (front left of foot), fsr_FR, (front right of foot), and fsr_Bk, (back of heel). 
  
  to run: compile to board
                rosrun rosserial_python serial_node.py /dev/ttyACM0 _baud:=115200
*/

#define USE_USBCON
#include <ros.h>
#include <ArduinoHardware.h>
#include <std_msgs/Int8.h>

ros::NodeHandle nh;

std_msgs::Int8 fsr_FL;
std_msgs::Int8 fsr_FR;
std_msgs::Int8 fsr_Bk;

ros::Publisher fsr_FL_data("fsr_FL", &fsr_FL);
ros::Publisher fsr_FR_data("fsr_FR", &fsr_FR);
ros::Publisher fsr_Bk_data("fsr_Bk", &fsr_Bk);

int fsr_FL_pin = 0;
int fsr_FR_pin = 1;
int fsr_Bk_pin = 2;
int led = 13;


void FSR_FL_Cb(const std_msgs::Int8& incoming_msg) {
  if (incoming_msg.data == 0) {
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(led, LOW);
  }
}

void FSR_FR_Cb(const std_msgs::Int8& incoming_msg) {
  if (incoming_msg.data == 0) {
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(led, LOW);
  }
}

void FSR_Bk_Cb(const std_msgs::Int8& incoming_msg) {
  if (incoming_msg.data == 0) {
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(led, LOW);
  }
}

void blink(){
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(100);
}

ros::Subscriber<std_msgs::Int8> sub_FL("fsr_FL_data", &FSR_FL_Cb);
ros::Subscriber<std_msgs::Int8> sub_FR("fsr_FR_data", &FSR_FR_Cb);
ros::Subscriber<std_msgs::Int8> sub_Bk("fsr_Bk_data", &FSR_Bk_Cb);

void setup() {
  //pinMode(ir_sense,INPUT);
  //pinMode(led, OUTPUT);
  nh.initNode();
  delay(1000);
  nh.advertise(fsr_FL_data);
  nh.subscribe(sub_FL);
  
  nh.advertise(fsr_FR_data);
  nh.subscribe(sub_FR);
  
  nh.advertise(fsr_Bk_data);
  nh.subscribe(sub_Bk);
}

void loop() {
  fsr_FL.data = analogRead(fsr_FL_pin);
  fsr_FL_data.publish(&fsr_FL);
  
  fsr_FR.data = analogRead(fsr_FR_pin);
  fsr_FR_data.publish(&fsr_FR);
  
  fsr_Bk.data = analogRead(fsr_Bk_pin);
  fsr_Bk_data.publish(&fsr_Bk);
  
  delay(5);
  nh.spinOnce(); 
  delay(100);
}

