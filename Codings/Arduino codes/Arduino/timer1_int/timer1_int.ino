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
boolean toggle0 = 0;
int distance1 = 0;
int _speed = 0;
int num;
int no_points=8;
int largest_dis=0;
int speed1=0,speed2=0;
//float degree=0.00;

////////////////////////////////Initilization///////////////////////////////////////
void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);
  gyro.init();
  gyro.zeroCalibrate(200,10);
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  //check();

  cli();//stop interrupts

//set timer0 interrupt at 2kHz
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
  

  sei();//allow interrupts
}

ISR(TIMER0_COMPA_vect){//timer0 interrupt 2kHz toggles pin 8
//generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle0){
    digitalWrite(13,HIGH);
    toggle0 = 0;
  }
  else{
    digitalWrite(13,LOW);
    toggle0 = 1;
  }
}

/////////////////////////////////////Main//////////////////////////////////////////
void loop() {


  Serial.println(digitalRead(13));
  //distance1 = ultrasonic.MeasureInCentimeters(5);
  //delay(1);
  /*
  if(distance1>30)
  {
    straight(90,20);
  }
  else
  {
    check();
  }*/
  //turn(90);

  //distance1 = ultrasonic.MeasureInCentimeters(5);
  //Serial.println(distance1+8);

  //Motor.stop(MOTOR1);
  //Motor.stop(MOTOR2);
  //while(1);

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
      gyro.getAngularVelocity(&az);
      Z=az;
      adder=adder+0.0058*Z;
      delay(1);
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

    while((degree<_angle-0.5)||(degree>_angle+0.5))
    {
        distance1 = ultrasonic.MeasureInCentimeters(5);
        Serial.print(distance1+8);
        Serial.print("    ");
        Serial.println(degree);
        
        degree=Degree(degree);
    
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


        
        //Serial.println(degree);
    }

    
/*    while((degree<_angle-1)||(degree>_angle+1))
    {
      __speed=_angle-degree;
      
      if((__speed>-30)&&(__speed<0))
      {
        __speed=-30;
      }
      if((__speed<30)&&(__speed>0))
      {
        __speed=30;
      }
      
      degree=Degree(degree);
      

      //distance1 = ultrasonic.MeasureInCentimeters(5);

      Serial.println(distance1);

      Motor.speed(MOTOR1,__speed);
      Motor.speed(MOTOR2,(-1)*__speed);
    }*/

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
    if((largest_dis<distance)&&(distance<300))
    {
      largest_dis=distance;
      num=counter;
    }
  }

  //_speed=(distance-point)*10;
  distance = ultrasonic.MeasureInCentimeters(5);
  Serial.println(distance1);

  if((largest_dis>distance)||(distance>300))
   {
     for(counter=0;counter<(no_points-num);++counter)
     {
       turn(-360/no_points);
     }
   }
   
}

