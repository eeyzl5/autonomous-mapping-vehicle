#include "Grove_I2C_Motor_Driver.h"
#define I2C_ADDRESS 0x0f // default I2C address is 0x0f
#include <Wire.h>
#include "ITG3200.h"
#include "Ultrasonic.h"
ITG3200 gyro;
Ultrasonic ultrasonic;

////////////////////////////////Variables////////////////////////////////////////////
boolean gyro_read = 0;
boolean update_speed=0;
unsigned int timer1_counter = 64286;
int _speed = 0;
int speed1=0,speed2=0;
int target=360;

////////////////////////////////Initilization///////////////////////////////////////
void setup() 
{
  //pinMode(13, OUTPUT);
  Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);
  gyro.init();
  gyro.zeroCalibrate(200,10);

  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 65474;   // preload timer 65536-16MHz/256/1000Hz
  //timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  //timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}

ISR(TIMER1_OVF_vect)
{
  TCNT1 = timer1_counter;   // preload timer
  //digitalWrite(13, digitalRead(13) ^ 1);
  gyro_read=1;
}

/////////////////////////////////////Main//////////////////////////////////////////
void loop() {

  
  turn(target);
  gyro.zeroCalibrate(200,10);
  turn(target);
  //Motor.stop(MOTOR1);
  //Motor.stop(MOTOR2);
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
float get_angle(float _angle)
{
    float az;
    int Z;

    if(gyro_read==1)
    {

      gyro.getAngularVelocity(&az);
      Z=az;
      _angle=_angle+0.0202*az;
      gyro_read=false;
      update_speed=true;
    }
    
    return _angle;
}

//////////////////////////////////////Turn//////////////////////////////////////////
void turn(float _target)
{
    int __speed;
    float pre_error;
    float error=1;
    float increment;
    float ki=1,kp=10;
    float _angle=0.00;
    TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt

    while(_angle<_target)
    {
        //distance1 = ultrasonic.MeasureInCentimeters(5);
        //Serial.print(distance1+8);
        //Serial.print("    ");
        //Serial.println(degree);
        
        _angle=get_angle(_angle);
        
        if((update_speed)&&(_angle<_target-5))
        {
          pre_error=error;
          error=(_target-5)-_angle;
          __speed=kp*(error-pre_error)+ki*error;
          update_speed=0;
        }
        
        //__speed=target-_angle;

        /*if((__speed>-30)&&(__speed<0))
        {
            __speed=-30;
        }
        if((__speed<30)&&(__speed>0))
        {
            __speed=30;
        }*/
        

        if((_angle>=_target-5)||(__speed<30))
        {
          __speed=30;
        }
    
        Motor.speed(MOTOR1,__speed);
        Motor.speed(MOTOR2,(-1)*__speed);
        

        //Serial.println(_angle);
    }
    TIMSK1 |= (0 << TOIE1);   // disable timer overflow interrupt
    Motor.stop(MOTOR1);
    Motor.stop(MOTOR2);

}

