
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
    pinMode(ena_pin, OUTPUT);
    pinMode(enb_pin, OUTPUT);
    pinMode(motor_l_1, OUTPUT);
    pinMode(motor_l_2, OUTPUT);
    pinMode(motor_r_1, OUTPUT);
    pinMode(motor_r_2, OUTPUT);
    myservo.attach(servo_pin);
  }

    myservo.write(60);
    analogWrite(ena_pin,motor_speed);
    analogWrite(enb_pin,motor_speed);
    
    backwards();
    delay(1000);
    myservo.write(120);
    backwards();
    delay(620);
    myservo.write(90);
    forwards();
    delay(200);
    myservo.write(75);
    forwards();
    delay(300);
    myservo.write(110);
    backwards();
    delay(300);
    myservo.write(60);
    forwards();
    delay(200);
    myservo.write(110);
    backwards();
    delay(400);
    myservo.write(60);
    forwards();
    delay(300);
    myservo.write(90);
    
    
    
    //backwards();
    analogWrite(ena_pin,motor_speed);
    analogWrite(enb_pin,motor_speed);
   
    
    analogWrite(ena_pin,0);
    analogWrite(enb_pin,0);
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
