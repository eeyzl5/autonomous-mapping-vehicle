
#include <Wire.h>
#include "ITG3200.h"
#include "Grove_I2C_Motor_Driver.h"
// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f
ITG3200 gyro;

int speed1=0,speed2=0;

void setup()
{
    Motor.begin(I2C_ADDRESS);
    Serial.begin(9600);
    gyro.init();
    gyro.zeroCalibrate(200,10);//sample 200 times to calibrate and it will take 200*10ms
}

void loop()
{
    straight(90,0);
    //Motor.stop(MOTOR1);
    //Motor.stop(MOTOR2);
    //while(1);
}


void straight(int __speed, unsigned long int t_ms)
{
    float az;
    int Z;
    

    speed1=__speed;
    speed2=__speed;

    if(t_ms==0)
    {
      
    
      for(int i=0;i<1000;++i)
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
        Serial.println(Z);

        delayMicroseconds(10);
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

