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
float data_distance[100];
float data_degree[100];
int data_counter;
//float degree=0.00;

////////////////////////////////Initilization///////////////////////////////////////
void setup() {
  Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);
  gyro.init();
  gyro.zeroCalibrate(200,10);
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  //check();
}

/////////////////////////////////////Main//////////////////////////////////////////
void loop() {

  /*//find route
  distance1 = ultrasonic.MeasureInCentimeters(5);
  delay(1);
  
  if(distance1>30)
  {
    straight(90,2000);
  }
  else
  {
    check();
  }*/
  
  //straight(90,10000);

  /*//mapping
  for(int i=0;i<20;++i)
  {
    distance1 = ultrasonic.MeasureInCentimeters(5)+8;
    delay(1);
    turn(18);
    Motor.stop(MOTOR1);
    Motor.stop(MOTOR2);
    Serial.println(distance1);
  }
  
  distance1 = ultrasonic.MeasureInCentimeters(5)+8;
  Serial.println(distance1);
  
  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);

  //turn(-180);

  straight(90,500);
  Serial.println("**************************");*/

  //Angle test
  turn(360);

  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);
  while(1);
  
}

///////////////////////////////////Straight/////////////////////////////////////////
void straight(int __speed, unsigned long int t_ms)
{
    float az;
    int Z;
    

    speed1=__speed;
    speed2=__speed;

    if(t_ms==0)
    {
      
    
      for(int i=0;i<100;++i)
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

        //delayMicroseconds(10);
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
        //distance1 = ultrasonic.MeasureInCentimeters(5); //d
        //Serial.print(distance1+8);
        //Serial.print("     ");
        //Serial.println(degree);
        //data_distance[data_counter]=distance1+8;
        //data_degree[data_counter]=degree;
        
        degree=Degree(degree);                          //d
        //Serial.println(degree);
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

        ++data_counter;
        

    }

 

}

/////////////////////////////////////Check////////////////////////////////////////
void check()
{
  int counter=0;
  int distance=0;
  
  for(counter=0;counter<no_points;++counter)
  {
    distance = ultrasonic.MeasureInCentimeters(5);
    Serial.println(distance);
    turn(360/no_points);
    if((largest_dis<distance)&&(distance<400))
    {
      largest_dis=distance;
      num=counter;
    }
  }

  //_speed=(distance-point)*10;
  distance = ultrasonic.MeasureInCentimeters(5);
  Serial.println(distance1);

  if((largest_dis>distance)||(distance>400))
   {
     for(counter=0;counter<(no_points-num);++counter)
     {
       turn(-360/no_points);
     }
   }
   
}

