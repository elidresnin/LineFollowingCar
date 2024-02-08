
//****************************************************************
// Include libraries
//****************************************************************

#include <Servo.h>
#include <Wire.h>

//****************************************************************
// Declare varibles for servo
//****************************************************************

Servo steering_servo;
#define angle_centre 90
byte angle_difference_small = 1;
byte angle_difference_medium = 15;
byte angle_difference_large = 30;
byte angle_left_small;
byte angle_left_midsmall;
byte angle_left_medium;
byte angle_left_midlarge;
byte angle_left_large;
byte angle_right_small;
byte angle_right_midsmall;
byte angle_right_medium;
byte angle_right_midlarge;
byte angle_right_large;
#define servo_pin 2

//****************************************************************
// Declare varibles for drive motors
//****************************************************************

#define forward 0
#define back 1
byte motor_speed;
byte motor_direction;
byte motor_stop = 0;
byte motor_speed_fast = 255;
byte motor_speed_slow = 255;
#define ena_pin 5
#define enb_pin 6
#define motor_left_1_pin 8
#define motor_left_2_pin 9
#define motor_right_1_pin 10
#define motor_right_2_pin 11

//****************************************************************
// Declare other varibles
//****************************************************************

byte data_counter = 0;
byte ir_data[16];
byte tangential_position = 4;
byte flag = 0;  //if flag = 0 on track if = 1 car off ground if = 2 car off track
byte angle = 90;
byte threshold = 100;

void setup()
{
  //****************************************************************
  // Misc setup
  //****************************************************************
  
  Wire.begin();
  Serial.begin(9600);

  //****************************************************************
  // Setup pins
  //****************************************************************

  pinMode(ena_pin, OUTPUT);
  pinMode(enb_pin, OUTPUT);
  pinMode(motor_left_1_pin, OUTPUT);
  pinMode(motor_left_2_pin, OUTPUT);
  pinMode(motor_right_1_pin, OUTPUT);
  pinMode(motor_right_2_pin, OUTPUT);

  //****************************************************************
  // Setup servo
  //****************************************************************

  steering_servo.attach(servo_pin);
  steering_servo.write(angle_centre);

  //****************************************************************
  // Determine left and right angles
  //****************************************************************

  angle_left_small = angle_centre - angle_difference_small;
  angle_left_medium = angle_centre - angle_difference_medium;
  angle_left_large = angle_centre - angle_difference_large;

  angle_right_small = angle_centre + angle_difference_small;
  angle_right_medium = angle_centre + angle_difference_medium;
  angle_right_large = angle_centre + angle_difference_large;

}

void loop()
{
  //****************************************************************
  // Read the IR sensor data
  //****************************************************************
  {
    Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
    while (Wire.available())    // slave may send less than requested
    {
      ir_data[data_counter] = Wire.read();
      if (data_counter < 15)
        data_counter++;
      else
        data_counter = 0;
    }
  }
  //****************************************************************
  // Determine tangential position based on IR sensor data
  //****************************************************************
  {
    flag = 0;
    
    if ((ir_data[0] < threshold / 2) && (ir_data[14] < threshold / 2))      //When the car leaves the ground, stop working
      flag = 1;
    else if (ir_data[0] < threshold)
      tangential_position = 1;
    else if (ir_data[2] < threshold)
      tangential_position = 2;
    else if (ir_data[4] < threshold)
      tangential_position = 3;
    else if ((ir_data[6] < threshold) || (ir_data[8] < threshold))
      tangential_position = 4;
    else if (ir_data[10] < threshold)
      tangential_position = 5;
    else if (ir_data[12] < threshold)
      tangential_position = 6;
    else if (ir_data[14] < threshold)
      tangential_position = 7;
    else
      flag = 2;
  }
  //****************************************************************
  // Determine servo motor angle based on tangential position
  //****************************************************************
  {
    if (tangential_position == 1)
      angle = angle_left_large;
    else if (tangential_position == 2)
      angle = angle_left_medium;
    else if (tangential_position == 3)
      angle = angle_left_small;
    else if (tangential_position == 4)
      angle = angle_centre;
    else if (tangential_position == 5)
      angle = angle_right_small;
    else if (tangential_position == 6)
      angle = angle_right_medium;
    else if (tangential_position == 7)
      angle = angle_right_large;
    if (flag == 1)
      angle = angle_centre;
  }
  //****************************************************************
  // Determine drive motor speed and direction based on tangential position
  //****************************************************************
  {
    if ((flag == 1) || (flag == 2))
    {
      motor_direction = back;
      motor_speed = motor_stop;
    }
    else
    {
      motor_direction = forward;
      motor_speed = motor_speed_fast;
    }
  }
  //****************************************************************
  // Write servo motor angle
  //****************************************************************
  {
    steering_servo.write(angle);
  }
  //****************************************************************
  // Write drive motor speed and direction
  //****************************************************************
  {
    if (motor_direction == forward)
    {
      digitalWrite(motor_left_1_pin, HIGH);
      digitalWrite(motor_left_2_pin, LOW);
      digitalWrite(motor_right_1_pin, HIGH);
      digitalWrite(motor_right_2_pin, LOW);
    }
    if (motor_direction == back)
    {
      digitalWrite(motor_left_1_pin, LOW);
      digitalWrite(motor_left_2_pin, HIGH);
      digitalWrite(motor_right_1_pin, LOW);
      digitalWrite(motor_right_2_pin, HIGH);
    }

    analogWrite(ena_pin, motor_speed); //write motor_speed_fast_left to ena_pin,if motor_speed_fast_left is high,allow left motor rotate
    analogWrite(enb_pin, motor_speed); //write motor_speed_fast_right to enb_pin,if motor_speed_fast_right is high,allow right motor rotate
  }
}
