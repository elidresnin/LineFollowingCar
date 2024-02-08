#include <Servo.h>
#include <Wire.h>
#define uchar unsigned char
uchar t;
uchar data[16];

Servo myservo;

#define ENA_PIN   5 // ENA attach to pin5  
#define ENB_PIN   6// ENB attach to pin6
#define MOTOR_L_1 8 // MOTOR_L_1 attach to pin8
#define MOTOR_L_2 9 // MOTOR_L_1 attach to pin9
#define MOTOR_R_1 10 // MOTOR_R_1 attach to pin10
#define MOTOR_R_2 11 // MOTOR_R_1 attach to pin11

#define FORWARD 0  //define forward=0,car move forward
#define BACK    1 //define back=1,car move back

#define ANGLE_MIDDLE        90

#define ANGLE_RIGHT_HALF1   95
#define ANGLE_RIGHT_HALF2   102
#define ANGLE_RIGHT_MAX     115

#define ANGLE_LEFT_HALF1   85
#define ANGLE_LEFT_HALF2   70
#define ANGLE_LEFT_MAX     60

#define SPEED 150 //the rotate speed of motor

uchar flag = 4;
uchar angle = 90;

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  t = 0;
  /* set below pins as OUTPUT */
  pinMode(ENA_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);
  pinMode(MOTOR_L_1, OUTPUT);
  pinMode(MOTOR_L_2, OUTPUT);
  pinMode(MOTOR_R_1, OUTPUT);
  pinMode(MOTOR_R_2, OUTPUT);
  myservo.attach(2);//servo attach to pin2
  myservo.write(90);//set the angle of servo
}

void loop()
{ 
  //read the value of the linefollwing sensor
  Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
  while (Wire.available())   // slave may send less than requested
  {
    data[t] = Wire.read(); // receive a byte as character
    if (t < 15)
      t++;
    else
      t = 0;
  }
  if ((data[0] < 50) && (data[14] < 50))                //When the car leaves the ground, stop working
    flag = 0;
  else if (data[0] < 100)                            
    flag = 1;
  else if (data[2] < 100)                          
    flag = 2;
  else if (data[4] < 100)                       
    flag = 3;
  else if ((data[6] < 100) || (data[8] < 100)) 
    flag = 4;
  else if (data[10] < 100)                
    flag = 5;
  else if (data[12] < 100)             
    flag = 6;
  else if (data[14] < 100)         
    flag = 7;
  else                         
    flag = 8;

  switch (flag)  
  {
    case 0: angle = ANGLE_MIDDLE;     break;
    case 1: angle = ANGLE_LEFT_MAX;   break;
    case 2: angle = ANGLE_LEFT_HALF2;  break;
    case 3: angle = ANGLE_LEFT_HALF1;  break;
    case 4: angle = ANGLE_MIDDLE;     break;
    case 5: angle = ANGLE_RIGHT_HALF1; break;
    case 6: angle = ANGLE_RIGHT_HALF2; break;
    case 7: angle = ANGLE_RIGHT_MAX;  break;
    case 8: angle = angle;   break;
    default: break;
  }
  if ((flag == 8) || (flag == 0))
  {
    myservo.write(angle);//set the angle of servo
    CAR_move(BACK, 0, 0); 
  }
  else
  {
    myservo.write(angle);//set the angle of servo
    if (angle == ANGLE_MIDDLE)
    {
      CAR_move(FORWARD, SPEED, SPEED); //car move forward with speed 180
    }
    else
    CAR_move(FORWARD, 120, 120);
  }
}

void CAR_move(unsigned char direction, unsigned char speed_left, unsigned char speed_right)
{
  switch (direction)
  {
    //car move forward with speed 180
    case 0: digitalWrite(MOTOR_L_1, HIGH); digitalWrite(MOTOR_L_2, LOW); //left motor clockwise rotation
      digitalWrite(MOTOR_R_1, HIGH); digitalWrite(MOTOR_R_2, LOW); break; //right motor clockwise rotation
    //car move back with speed 180
    case 1: digitalWrite(MOTOR_L_1, LOW); digitalWrite(MOTOR_L_2, HIGH);
      digitalWrite(MOTOR_R_1, LOW); digitalWrite(MOTOR_R_2, HIGH); break;
    default: break;
  }
  analogWrite(ENA_PIN, speed_left); //write speed_left to ENA_PIN,if speed_left is high,allow left motor rotate
  analogWrite(ENB_PIN, speed_right); //write speed_right to ENB_PIN,if speed_right is high,allow right motor rotate
}
