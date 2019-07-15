#include "Grove_I2C_Motor_Driver.h"
#define I2C_ADDRESS 0x0f // default I2C address is 0x0f
#include <Wire.h>
#include "rgb_lcd.h"
#include "ITG3200.h"
#include "Ultrasonic.h"
rgb_lcd lcd;
ITG3200 gyro;
Ultrasonic ultrasonic;
int colorR = 0;
int colorG = 0;
int colorB = 0;

////////////////////////////////Variables////////////////////////////////////////////
int distance1 = 0;
int _speed = 0;
int num;
int no_points=8;
int largest_dis=0;
int speed1=0,speed2=0;
float data[100];
int data_counter;
int deg=360;

////////////////////////////////Initilization///////////////////////////////////////
void setup() {
  Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);
  gyro.init();
  gyro.zeroCalibrate(200,10);
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
}

/////////////////////////////////////Main//////////////////////////////////////////
void loop() {

  //Angle test
  turn(deg);

  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);
  
  for(int i=0;i<data_counter;++i)
  {
    Serial.println(data[i]);
  }
  
  while(1);

}


///////////////////////////////////////Degree////////////////////////////////////////
float Degree(float adder)
{
    float az;
    int Z;

    for(int i=0;i<10;++i)
    {
      gyro.getAngularVelocity(&az);             //d
      //Z=az;
      adder=adder+0.0029*az;
      delayMicroseconds(50);                                         //d
    }
    
    return adder;
}

//////////////////////////////////////Turn//////////////////////////////////////////
void turn(float _angle)
{
    int __speed;
    float pre_error;
    float error=1;
    float increment;
    float ki=5,kp=7;
    float degree=0.00;

    data_counter=0;

    while((degree<_angle-0.1)||(degree>_angle+0.1))
    {
        
        degree=Degree(degree);                          //d
        data[data_counter]=degree;
        
        pre_error=error;
        error=_angle-degree;
        __speed=kp*(error-pre_error)+ki*error;

        if((__speed>-30)&&(__speed<0))
        {
            __speed=-30;
        }
        if((__speed<30)&&(__speed>0))
        {
            __speed=30;
        }
        
    
        Motor.speed(MOTOR1,__speed);
        Motor.speed(MOTOR2,(-1)*__speed);

        data_counter++;
        
    }

 

}


