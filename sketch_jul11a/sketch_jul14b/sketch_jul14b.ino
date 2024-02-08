
#include <Servo.h>
Servo myservo;

//****************************************************************
// Declare varibles for drive motors
//****************************************************************

#define ena_pin 5
#define enb_pin 6
#define motor_l_1 8
#define motor_l_2 9
#define motor_r_1 10
#define motor_r_2 11

#define servo_pin 2

#define motor_speed 150

void setup()
{
  {
    pinMode(ena_pin, OUTP UT);
    pinMode(enb_pin, OUTPUT);
    pinMode(motor_l_1, OUTPUT);
    pinMode(motor_l_2, OUTPUT);
    pinMode(motor_r_1, OUTPUT);
    pinMode(motor_r_2, OUTPUT);
    myservo.attach(servo_pin);
  }

    myservo.write(60);
  analogWrite(ena_pin, motor_speed);
  analogWrite(enb_pin, motor_speed);
  
  backwards();
  delayWithMotorOn(1000);
  
  myservo.write(120);
  backwards();
  delayWithMotorOn(620);
  
  myservo.write(90);
  forwards();
  delayWithMotorOn(200);
  
  myservo.write(75);
  forwards();
  delayWithMotorOn(300);
  
  myservo.write(110);
  backwards();
  delayWithMotorOn(300);
  
  myservo.write(60);
  forwards();
  delayWithMotorOn(200);
  
  myservo.write(110);
  backwards();
  delayWithMotorOn(400);
  
  myservo.write(60);
  forwards();
  delayWithMotorOn(300);
  
  myservo.write(90);
  
  // Stop the motor
  analogWrite(ena_pin, 0);
  analogWrite(enb_pin, 0);
}

void loop()
{

}

void forwards()
{
    digitalWrite(motor_l_1,HIGH);
    digitalWrite(motor_l_2,LOW);
    digitalWrite(motor_r_1,HIGH);
    digitalWrite(motor_r_2,LOW);
}

void backwards()
{
    digitalWrite(motor_l_1,LOW);
    digitalWrite(motor_l_2,HIGH);
    digitalWrite(motor_r_1,LOW);
    digitalWrite(motor_r_2,HIGH);
}
void delayWithMotorOn(unsigned long duration) {
  unsigned long start_time = millis();
  while (millis() - start_time < duration) {
    analogWrite(ena_pin, motor_speed);
    analogWrite(enb_pin, motor_speed);
  }
}