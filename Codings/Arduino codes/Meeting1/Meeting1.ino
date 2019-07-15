#include "Grove_I2C_Motor_Driver.h"
// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f

const unsigned int pushButton1 = 4;
const unsigned int pushButton2 = 2;
const unsigned int trig = 12;
const unsigned int echo = 13;
const unsigned int vcc = 11;
int distance = 0;
int pre_distance=0;
int point = 16;
int _speed = 0;
float sound_speed = 0.115; 
int num=0;
int target = 20;
int counter=0;
int flag=LOW;

void setup() {
  Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);
  pinMode(vcc, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(pushButton1, INPUT_PULLUP);
  pinMode(pushButton2, INPUT_PULLUP);
  //check();
}

void loop() {
  int b5 = digitalRead(5);
  int b6 = digitalRead(6);
  int b7 = digitalRead(7);
  int b8 = digitalRead(8);
  int b9 = digitalRead(9);

  if(b5==LOW)//38
  {
    Motor.stop(MOTOR1);
    Motor.speed(MOTOR2, 100);
    delay(1300);
    Motor.speed(MOTOR1, -100);
    Motor.stop(MOTOR2);
    delay(1800);

    Motor.stop(MOTOR1);
    Motor.stop(MOTOR2);
    delay(2000);
    //////////////////////////////////////
    Motor.stop(MOTOR2);
    Motor.speed(MOTOR1, 100);
    delay(1500);
    Motor.stop(MOTOR1);
    Motor.speed(MOTOR2, 100);
    delay(3000);
    Motor.stop(MOTOR2);
    Motor.speed(MOTOR1, 100);
    delay(2500);
  
    Motor.stop(MOTOR1);
    Motor.stop(MOTOR2);
    delay(2000);
  }
  if(b6==LOW)//follow
  {
    distance = dis();
    delay(5);
    _speed=(distance-target)*10;

    if((_speed>20||_speed<-20))
    {
      Motor.speed(MOTOR1, _speed);
      Motor.speed(MOTOR2, _speed);
    }
    else
    {
      Motor.stop(MOTOR1);
      Motor.stop(MOTOR2);
    }
  }
  if(b7==LOW)//find route
  {
    if(flag==LOW)
    {
      check();
      flag=HIGH;
    }
    
    distance=dis();
    delay(100);

    if(distance>30)
    {
      straight(70);
    }
    else
    {
      mstop(100);
      //delay(100);
      check();
    }
  }
  if(b8==LOW)//Sonar
  {
    long int i = 0;
    int flag = 0;

    i=0;
    digitalWrite(vcc, HIGH);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
  
  
    while(digitalRead(echo) == LOW){;}
    while((digitalRead(echo) == HIGH)&&(i<3000))
    {
      delayMicroseconds(4);
      ++i;
     }

    pre_distance = distance;
    distance = sound_speed*i;
    delay(1);
    //_speed=(distance-point)*10;

    if(counter<44){
      if(counter>0)
      {
        Serial.println(distance);
        //Serial.println(pre_distance);
      }
  
      ++counter;
  
      Motor.speed(MOTOR1, 100);
      Motor.speed(MOTOR2, -100);
      delay(27);
      //Motor.stop(MOTOR1);
      //Motor.stop(MOTOR2);
      //delay(100);
      }

      Motor.stop(MOTOR1);
      Motor.stop(MOTOR2);

    }
    if(b9==LOW)
    {
      long int i = 0;
      int flag = 0;
  
      i=0;
      digitalWrite(vcc, HIGH);
      digitalWrite(trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig, LOW);
  
  
      while(digitalRead(echo) == LOW){;}
      while(digitalRead(echo) == HIGH)
      {
        delayMicroseconds(4);
        ++i;
      }
  
      distance = sound_speed*i;
      delay(10);
      _speed=(distance-point)*6;

      if((distance<=point)&&(distance>=5))
      {
        Motor.stop(MOTOR1);
        Motor.speed(MOTOR2, 50);
    
        if(distance>=5)
        {
          delay(1000);
          Motor.speed(MOTOR1, 50);
          Motor.speed(MOTOR2, 50);
      
          if(distance>=5)
          {
            delay(1000);
            Motor.stop(MOTOR2);
            Motor.speed(MOTOR1, 50);
            delay(1000);
          }
      
        }
    
    
      }
  
      else if(distance<5)
      {
       Motor.speed(MOTOR1, -50);
        Motor.speed(MOTOR2, -50);
        delay(1000);
      }
      else
      {
        Motor.speed(MOTOR1, 50);
        Motor.speed(MOTOR2, 50);
      }
    }
    

}


/****************************************************************/
int dis()
{
  long int i = 0;
  
  i=0;
  digitalWrite(vcc, HIGH);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  while(digitalRead(echo) == LOW){;}
  while(digitalRead(echo) == HIGH)
  {
    delayMicroseconds(4);
    ++i;
  }
  
  return sound_speed*i;
}

void straight(int _speed)
{
  Motor.speed(MOTOR1, _speed);
  if(_speed>0)
  {
    Motor.speed(MOTOR2, _speed-3);
  }
  if(_speed<0)
  {
    Motor.speed(MOTOR2, _speed+3);
  }
  if(_speed=0)
  {
    Motor.speed(MOTOR2, _speed);
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

void check()
{
  int largest_dis=0;
  
  for(int i=0;i<12;++i)
  {
    distance = dis();
    delay(100);
    left(70,200);
    mstop(1);
    //delay(100);
    if((largest_dis<distance)&&(distance<300))
    {
      largest_dis=distance;
      num=i;
    }
    
  }

  distance = dis();
  delay(100);

   if((largest_dis>distance)||(distance>300))
   {
     for(int i=0;i<(12-num);++i)
     {
       right(70,200);
       mstop(101);
       //delay(100);
     }
   }
  
}

