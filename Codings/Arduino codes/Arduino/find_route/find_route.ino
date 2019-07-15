#include <Wire.h>
#include "ITG3200.h"
#include "Grove_I2C_Motor_Driver.h"
// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f
#include "Ultrasonic.h"
ITG3200 gyro;
Ultrasonic ultrasonic;

int distance = 0;
int point = 16;
int num=0;
int no_points=8;
int largest_dis=0;
int speed1=0,speed2=0;

// the setup function runs once when you press reset or power the board
void setup() {
  //Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);
  gyro.init();
  gyro.zeroCalibrate(200,10);
  check();
}

// the loop function runs over and over again forever
void loop() {

  distance = ultrasonic.MeasureInCentimeters(5);
  delay(1);
  
  if(distance>30)
  {
    Motor.speed(MOTOR1,100);
    Motor.speed(MOTOR2,100);
  }
  else
  {
    check();
  }

  

  

  

  //Serial.println(distance);
  //delay(1);        // delay in between reads for stability
  

}


void straight(int __speed, unsigned long int t_ms)
{
    float ax,ay,az;
    int Z;
    int speed1,speed2;

    speed1=__speed;
    speed2=__speed;

    if(t_ms==0)
    {
      while(1)
      {
        gyro.getAngularVelocity(&az);
        Z=az;

        if(Z>1)
        {
          speed1--;
        }
        if(Z<-1)
        {
          speed1++;
        }

        Motor.speed(MOTOR1, speed1);
        Motor.speed(MOTOR2, speed2);
        //Serial.println(Z);

        delayMicroseconds(100);
        }
    }
    else
    {
      int counter=0;
      while(counter<t_ms/20)
      {
        ++counter;
        gyro.getAngularVelocity(&az);
        Z=az;

        if(Z>1)
        {
          speed1--;
        }
        if(Z<-1)
        {
          speed1++;
        }

        Motor.speed(MOTOR1, speed1);
        Motor.speed(MOTOR2, speed2);
        //Serial.println(Z);

        delayMicroseconds(50);
       }
    }
}


void left(int _speed, int t_ms)
{
  Motor.speed(MOTOR1, _speed);
  Motor.speed(MOTOR2, _speed*(-1));
  delay(t_ms);
}

void right(int _speed, int t_ms)
{
  Motor.speed(MOTOR1, _speed*(-1));
  Motor.speed(MOTOR2, _speed);
  delay(t_ms);
}

void mstop(int t_ms)
{
  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);
  delay(t_ms);
}


/////////////////////////////////////Check////////////////////////////////////////
void check()
{
  int counter=0;
  int distance=0;
  
  for(counter=0;counter<no_points;++counter)
  {
    distance = ultrasonic.MeasureInCentimeters(5);
    turn(360/no_points);
    if((largest_dis<distance)&&(distance<400))
    {
      largest_dis=distance;
      num=counter;
    }
  }

  //_speed=(distance-point)*10;
  distance = ultrasonic.MeasureInCentimeters(5);

  if((largest_dis>distance)||(distance>400))
   {
     for(counter=0;counter<(no_points-num);++counter)
     {
       turn(-360/no_points);
     }
   }
   
}


