//#include "Grove_I2C_Motor_Driver.h"
#include "Wire.h"
#include "ITG3200.h"
#include "Ultrasonic.h"
ITG3200 gyro;
Ultrasonic ultrasonic;

////////////////////////////////Variables////////////////////////////////////////////
unsigned int timer2_counter = 0;
float angle = 0.00;
int target=180;
int state=1;
int counterH=0;
int counterL=0;
int Motor_step=0;
int lf=6;
int lb=5;
int rf=9;
int rb=10;
int Pin1 = 3; 
int Pin2 = 4; 
int Pin3 = 7; 
int Pin4 = 8; 
int utr = 12;
int _step = 0;
int Stepper_Final = 4200;
int Capture_Points = 12;
int R;
int theta=0;
int offset_angle=0;
boolean Stepper_Zero = HIGH;
boolean Stepper_Dir = true;// false=clockwise, true=Stepper_Counterer clockwise
unsigned long int Stepper_Counter=1;

////////////////////////////////Initilization///////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(lb,OUTPUT);
  pinMode(lf,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(rf,OUTPUT);
  pinMode(rb,OUTPUT);
  pinMode(2,INPUT_PULLUP);
  /*
  noInterrupts();           // disable all interrupts
  TCCR2A = 0;
  TCCR2B = 0;  
  TCNT2 = timer2_counter;   // preload timer
  TCCR2B = 0x07;    // 1024 prescaler 
  TIMSK2 = 0x01;   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
  */
}

/*ISR(TIMER2_OVF_vect)
{
  TCNT2 = timer2_counter;   // preload timer
  //digitalWrite(13, digitalRead(13) ^ 1);
  gyro_read=1;
}*/

/////////////////////////////////////Main//////////////////////////////////////////
void loop() {
    
    //angle=get_angle(angle);  
    Move('f',48);


      //digitalWrite(lf,1);
      //delayMicroseconds(50000);
      //delay(50);
      //digitalWrite(lf,0);
      //delayMicroseconds(50000);
    //analogWrite(lf, 255);
    //analogWrite(rb,255);
    //digitalWrite(lb,0);
    //Serial.println(state);
    //analogWrite(rf, __speed);
    //turn(target);
    while(1);
    //Move('f',100);

}


/////////////////////////////////Read_Motor_Step///////////////////////////////////
void Read_Motor_Step()
{
      if(analogRead(A3)<=14)
      {
        counterL++;
        counterH=0;
        if((counterL==5)&&(state==1))
        {
          state=0;
          ++Motor_step;      
          counterL=0;
        }
      }
      else
      {
        counterL=0;
        counterH++;
        if((counterH==5)&&(state==0))
        {
          state=1;
          ++Motor_step;
          counterH=0;
        }
      }  
}


void Move(char _direction, unsigned int input)
{
  
  if(_direction=='s')
  {
    digitalWrite(lf, 0);
    digitalWrite(rf, 0);
    digitalWrite(lb, 0);
    digitalWrite(rb, 0);
  }
  if(_direction=='f')
  {
    while(Motor_step<input)
    {
      Read_Motor_Step();
      analogWrite(lf, 255);
      analogWrite(rf, 242);
      digitalWrite(lb, 0);
      digitalWrite(rb, 0);
    }
    Motor_step=0; 
    digitalWrite(lf, 0);
    digitalWrite(rf, 0);
    digitalWrite(lb, 1);
    digitalWrite(rb, 1);
    delay(20);
    digitalWrite(lf, 0);
    digitalWrite(rf, 0);
    digitalWrite(lb, 0);
    digitalWrite(rb, 0);
  }
  if(_direction=='b')
  {
    while(Motor_step<input)
    {
      Read_Motor_Step();
      analogWrite(lb, 250);
      analogWrite(rb, 255);
      digitalWrite(lf, 0);
      digitalWrite(rf, 0);
    }
    Motor_step=0; 
    digitalWrite(lb, 0);
    digitalWrite(rb, 0);
    digitalWrite(lf, 1);
    digitalWrite(rf, 1);
    delay(10);
    digitalWrite(lf, 0);
    digitalWrite(rf, 0);
    digitalWrite(lb, 0);
    digitalWrite(rb, 0);
  }
  if(_direction=='r')
  {
    while(Motor_step<input)
    {
      Read_Motor_Step();
      analogWrite(lf, 255);
      analogWrite(rb, 245);
      digitalWrite(rf, 0);
      digitalWrite(lb, 0);
    }
    Motor_step=0; 
    digitalWrite(lf, 0);
    digitalWrite(rb, 0);
    digitalWrite(rf, 1);
    digitalWrite(lb, 1);
    delay(input*16);
    digitalWrite(lf, 0);
    digitalWrite(rf, 0);
    digitalWrite(lb, 0);
    digitalWrite(rb, 0);
  }
  if(_direction=='l')
  {
    while(Motor_step<input)
    {
      Read_Motor_Step();
      analogWrite(rf, 255);
      analogWrite(lb, 215);
      digitalWrite(lf, 0);
      digitalWrite(rb, 0);     
    }
    Motor_step=0; 
    digitalWrite(rf, 0);
    digitalWrite(lb, 0);
    digitalWrite(lf, 1);
    digitalWrite(rb, 1);
    delay(input*16);
    digitalWrite(lf, 0);
    digitalWrite(rf, 0);
    digitalWrite(lb, 0);
    digitalWrite(rb, 0);
  }
}



