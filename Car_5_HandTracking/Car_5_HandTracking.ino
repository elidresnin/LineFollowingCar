//****************************************************************
// Include libraries
//****************************************************************

#include <Servo.h>
#include <Wire.h>

//****************************************************************
// Declare variables for servo
//****************************************************************

Servo steering_servo;
#define servo_pin 2

//****************************************************************
// Declare variables for drive motors
//****************************************************************

#define forward 0
#define back 1
byte motor_speed;
byte motor_direction;
byte motor_stop = 0;
byte motor_speed_fast = 255;
#define ena_pin 5
#define enb_pin 6
#define motor_left_1_pin 8
#define motor_left_2_pin 9
#define motor_right_1_pin 10
#define motor_right_2_pin 11

//****************************************************************
// Declare other variables
//****************************************************************

byte data_counter = 0;
byte ir_data[16];
byte tangential_position = 4;
byte flag = 0; // if flag = 0 on track, if = 1 car off ground, if = 2 car off track
byte threshold = 100;
int counter_steering_timer = 0;
int counter_steering_duration = 200; // Adjust this value to control the duration of counter-steering

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
  steering_servo.write(90); // Set the servo angle to keep the car moving straight

}

void loop()
{
  //****************************************************************
  // Read the IR sensor data
  //****************************************************************
  {
    Wire.requestFrom(9, 16); // request 16 bytes from slave device #9
    while (Wire.available()) // slave may send less than requested
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

    if ((ir_data[0] < threshold / 2) && (ir_data[14] < threshold / 2)) // When the car leaves the ground, stop working
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
    if (flag == 0) // Car on track
    {
      // Set the servo angle to keep the car moving straight
      steering_servo.write(90);
    }
    else if (flag == 1) // Car off ground
    {
      // Stop the car
      motor_direction = back;
      motor_speed = motor_stop;
      steering_servo.write(90);
    }
    else // Car off track
    {
      if (counter_steering_timer <= 0) // If counter-steering timer is 0 or less, perform normal steering
      {
        if (tangential_position == 1)
          steering_servo.write(75); // Turn left sharply
        else if (tangential_position == 2)
          steering_servo.write(80); // Turn left moderately
        else if (tangential_position == 3)
          steering_servo.write(85); // Turn left slightly
        else if (tangential_position == 5)
          steering_servo.write(95); // Turn right slightly
        else if (tangential_position == 6)
          steering_servo.write(100); // Turn right moderately
        else if (tangential_position == 7)
          steering_servo.write(105); // Turn right sharply
          
  
      }
      else // Perform counter-steering
      {
        // Invert the servo angle temporarily to counter-steer
        if (steering_servo.read() < 90)
          steering_servo.write(steering_servo.read() + 20); // Increase angle towards right
        else
          steering_servo.write(steering_servo.read() - 20); // Decrease angle towards left
        counter_steering_timer -= 1; // Decrease the counter-steering timer
      }

      motor_direction = forward;
      motor_speed = motor_speed_fast;
    }
  }
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

    analogWrite(ena_pin, motor_speed); // Write motor_speed to ena_pin, if motor_speed is high, allow left motor rotate
    analogWrite(enb_pin, motor_speed); // Write motor_speed to enb_pin, if motor_speed is high, allow right motor rotate
  }
}
