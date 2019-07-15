#include <Wire.h>
#include "ITG3200.h"
#include "rgb_lcd.h"
#include "Ultrasonic.h"
#include "Grove_I2C_Motor_Driver.h"
// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f

Ultrasonic ultrasonic;
rgb_lcd lcd;

int colorR = 0;
int colorG = 0;
int colorB = 0;

int speed1,speed2;
int target = 20;
int distance = 20;
int _speed = 0;


ITG3200 gyro;

void setup()
{
  Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  gyro.init();
  gyro.zeroCalibrate(200,10);
}
void loop()
{
  long int distance;
  
  distance = ultrasonic.MeasureInCentimeters(5); // two measurements should keep an interval
  delay(1);
  _speed=(distance-target)*10;

  if(_speed>90)
  {
    _speed=90;
  }
  if(_speed<-90)
  {
    _speed=-90;
  }

  if((_speed>20||_speed<-20))
  {
    straight(_speed,100);
    colorR = 0;
  }
  else
  {
    Motor.stop(MOTOR1);
    Motor.stop(MOTOR2);
    colorR = 255;
  }
  
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print("              ");
  
}

void straight(int __speed, unsigned long int delay_ms)
{
    float az;
    int Z;
    int counter=0;

    if(__speed!=speed2)
    {
      speed1=__speed;
      speed2=__speed;
    }


      while(counter<delay_ms/20)
      {
        ++counter;
        gyro.getAngularVelocity(&az);
        Z=az;

        if(Z>1)
        {
          speed2++;
        }
        if(Z<-1)
        {
          speed2--;
        }

        Motor.speed(MOTOR1, speed1);
        Motor.speed(MOTOR2, speed2);
        delayMicroseconds(50);
    }
}


