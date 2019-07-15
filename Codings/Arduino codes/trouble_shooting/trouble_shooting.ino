//#include "Grove_I2C_Motor_Driver.h"
#include "Wire.h"
#include "ITG3200.h"
#include "Ultrasonic.h"
ITG3200 gyro;
Ultrasonic ultrasonic;

////////////////////////////////Variables////////////////////////////////////////////
boolean gyro_read = 0;
boolean update_speed=0;
unsigned int timer2_counter = 0;
float angle = 0.00;
int target=180;
float _pre_error;
float _error;
unsigned int _speed=150;
int speed2;
int state=1;
int counterH=0;
int counterL=0;
int counterS=0;
int lf=6;
int lb=5;
int rf=9;
int rb=10;

////////////////////////////////Initilization///////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(2,INPUT_PULLUP);
  
  //digitalWrite(8,HIGH);
  gyro.init();
  gyro.zeroCalibrate(200,10);

  noInterrupts();           // disable all interrupts
  TCCR2A = 0;
  TCCR2B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 65474;   // preload timer 65536-16MHz/256/1000Hz
  //timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  //timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  
  TCNT2 = timer2_counter;   // preload timer
  TCCR2B = 0x05;    // 128 prescaler 
  TIMSK2 = 0x01;   // enable timer overflow interrupt
  interrupts();             // enable all interrupts

}

ISR(TIMER2_OVF_vect)
{
  TCNT2 = timer2_counter;   // preload timer
  //digitalWrite(13, digitalRead(13) ^ 1);
  gyro_read=1;
}


/////////////////////////////////////Main//////////////////////////////////////////
void loop() {

    
    //angle=get_angle(angle);
    //_speed = target -angle;
    //delay(10);
    if(analogRead(A3)<=12)
    {
      counterL++;
      counterH=0;
      if((counterL==4)&&(state==1))
      {
        state=0;
        ++counterS;      
        counterL=0;
      }
    }
    else
    {
      counterL=0;
      counterH++;
      if(counterH==4)
      {
        state=1;
        counterH=0;
      }
    }
   

    if(counterS==8)
    {
      Move('s',0);
      delay(1000);
      counterS=0;
      //angle=0.00;
    }
    else
    {
      Move('f',_speed);
      //delayMicroseconds(500);
    }
    //Serial.println(angle);

    
    //while(1);

}

void Move(char _direction, unsigned int __speed)
{
  
  if(__speed<=60)
  {
    __speed=60;
  }
  if(__speed>=254)
  {
    __speed=254;
  }
  
  if(_direction=='s')
  {
    digitalWrite(lf, 1);
    digitalWrite(rf, 1);
    digitalWrite(lb, 1);
    digitalWrite(rb, 1);
    //digitalWrite(lf, 0);
    //digitalWrite(rf, 0);
    //digitalWrite(lb, 0);
    //digitalWrite(rb, 0);
  }
  if(_direction=='f')
  { 
    angle=get_angle(angle);   
    if(angle>1)
    {
      speed2=247;
    }
    if((angle<=1)&&(angle>=-1))
    {
      speed2=249;
    }
    if(angle<-1)
    {
      speed2=252;
    }
    analogWrite(lf, 250);
    analogWrite(rf, speed2);
    digitalWrite(lb, 0);
    digitalWrite(rb, 0);
  }
  if(_direction=='b')
  {
    digitalWrite(lf, 0);
    digitalWrite(rf, 0);
    analogWrite(lb, __speed);
    analogWrite(rb, __speed);
  }
  if(_direction=='l')
  {
    digitalWrite(lf, 0);
    analogWrite(rf, __speed);
    analogWrite(lb, __speed);
    digitalWrite(rb, 0);
  }
  if(_direction=='r')
  {
    analogWrite(lf, __speed);
    digitalWrite(rf, 0);
    digitalWrite(lb, 0);
    analogWrite(rb, __speed);
  }
}



///////////////////////////////////////Degree////////////////////////////////////////
float get_angle(float _angle)
{
    float az;
    int Z;

    if(gyro_read)
    {
      digitalWrite(lf, 0);
      digitalWrite(rf, 0);
      digitalWrite(lb, 0);
      digitalWrite(rb, 0);
      gyro.getAngularVelocity(&az);
      if((Z-(int)az)<120)
      {
        Z=az;
      }
      _angle=_angle+0.0166*Z;
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
    float ki=5,kp=7;
    float _angle=0.00;
    TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt

    while((_angle<_target-1)||(_angle>_target+1))
    {
        
      _angle=get_angle(_angle);
      
      //__speed = _target - _angle;
      
      
        /*if((update_speed))
        {
          pre_error=error;
          error=_target-_angle;
          __speed=kp*(error-pre_error)+ki*error;
          update_speed=0;
        }*/
        
        __speed=_target-_angle;

        if((__speed>-30)&&(__speed<0))
        {
            __speed=-30;
        }
        if((__speed<30)&&(__speed>0))
        {
            __speed=30;
        }
        
        

        /*if((_angle>=_target-5)||(__speed<30))
        {
          __speed=30;
        }*/
    

        

        //Serial.println(_angle);
    }
    TIMSK1 |= (0 << TOIE1);   // disable timer overflow interrupt

}

