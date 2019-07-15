//#include "Grove_I2C_Motor_Driver.h"
#include "Wire.h"
#include "Ultrasonic.h"
Ultrasonic ultrasonic;

////////////////////////////////Variables////////////////////////////////////////////
unsigned int timer2_counter = 0;
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
int Capture_Points = 100;
float R;
float theta=0.00;
int offset_angle=0;
boolean Stepper_Zero = HIGH;
boolean Stepper_Dir = true;// false=clockwise, true=Stepper_Counterer clockwise
unsigned int Stepper_Counter=1;

////////////////////////////////Initilization///////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  pinMode(Pin1, OUTPUT);  
  pinMode(Pin2, OUTPUT);  
  pinMode(Pin3, OUTPUT);  
  pinMode(Pin4, OUTPUT);
  pinMode(lb,OUTPUT);
  pinMode(lf,OUTPUT);
  pinMode(rf,OUTPUT);
  pinMode(rb,OUTPUT);

  findZero();

  /*noInterrupts();           // disable all interrupts
  TCCR2A = 0;
  TCCR2B = 0; 
  TCNT2 = timer2_counter;   // preload timer
  TCCR2B = 0x07;    // 128 prescaler 
  TIMSK2 = 0x01;   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
  */
}
/*
ISR(TIMER2_OVF_vect)
{
    TCNT2 = timer2_counter;   // preload timer
    //digitalWrite(13, digitalRead(13) ^ 1);
    gyro_read=1;   
}*/


/////////////////////////////////////Main//////////////////////////////////////////
void loop() {

    Map_at(0,0);
    Move('f',16);
    //Map_at(20,0);
    //Move('r',8);
    //Move('f',16);
    //Move('r',8);
    //Move('f',16);
    //Move('r',8);
    //Move('f',16);
    //Move('r',8);
    //Map_at(0,20);
    
    Serial.println("***********");
    while(1);
    //Serial.println(angle);

    
    //while(1);

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

  }
  if(_direction=='f')
  {
    while(Motor_step<input)
    {
      Read_Motor_Step();
      analogWrite(lf, 255);
      analogWrite(rf, 240);
      digitalWrite(lb, 0);
      digitalWrite(rb, 0);
    }
    digitalWrite(lf, 0);
    digitalWrite(rf, 0);
    digitalWrite(lb, 1);
    digitalWrite(rb, 1);
    delay(20);
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
    digitalWrite(lb, 0);
    digitalWrite(rb, 0);
    digitalWrite(lf, 1);
    digitalWrite(rf, 1);
    delay(10);
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
    digitalWrite(lf, 0);
    digitalWrite(rb, 0);
    digitalWrite(rf, 1);
    digitalWrite(lb, 1);
    delay(input*16);
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
    digitalWrite(rf, 0);
    digitalWrite(lb, 0);
    digitalWrite(lf, 1);
    digitalWrite(rb, 1);
    delay(input*16);
  }
  
  Motor_step=0; 
  digitalWrite(lf, 0);
  digitalWrite(rf, 0);
  digitalWrite(lb, 0);
  digitalWrite(rb, 0);
  delay(500);
}


/////////////////////////////////stepper////////////////////////////////////////
void stepper()
{
  
  switch(_step){ 
   case 0: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, HIGH); 
   break;  
   case 1: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, HIGH); 
     digitalWrite(Pin4, HIGH); 
   break;  
   case 2: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, HIGH); 
     digitalWrite(Pin4, LOW); 
   break;  
   case 3: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, HIGH); 
     digitalWrite(Pin3, HIGH); 
     digitalWrite(Pin4, LOW); 
   break;  
   case 4: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, HIGH); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
   break;  
   case 5: 
     digitalWrite(Pin1, HIGH);  
     digitalWrite(Pin2, HIGH); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
   break;  
     case 6: 
     digitalWrite(Pin1, HIGH);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
   break;  
   case 7: 
     digitalWrite(Pin1, HIGH);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, HIGH); 
   break;  
   default: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
   break;  
 } 

 if(Stepper_Dir){ 
   _step++; 
 }else{ 
   _step--; 
 } 
 if(_step>7){ 
   _step=0; 
 } 
 if(_step<0){ 
   _step=7; 
 }
 
 delayMicroseconds(700);   
 Stepper_Counter++;
 
}

///////////////////////////////////findZero////////////////////////////////
void findZero()
{
 
 Stepper_Dir = true;
 
 while(Stepper_Counter!=500)
 {
   stepper();
 }
 
 Stepper_Dir = false;

 do
 {
  //Filter and Read A1
  if(analogRead(A1)<=20)
  {
      counterL++;
      counterH=0;
      if((counterL==4)&&(state==1))
      {
        state=0;    
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
  
  stepper();
  
 }while(state);

 do
 {
  //Filter and Read A1
  if(analogRead(A1)<=20)
  {
      counterL++;
      counterH=0;
      if((counterL==4)&&(state==1))
      {
        state=0;    
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
  
  stepper();
  
 }while(!state);
 
 digitalWrite(Pin1, LOW);  
 digitalWrite(Pin2, LOW); 
 digitalWrite(Pin3, LOW); 
 digitalWrite(Pin4, LOW); 
 delay(1000);

 Stepper_Counter=1;
 Stepper_Dir=true;  
 
}

//////////////////////////////////////Map_at////////////////////////////////////
void Map_at(float desired_x,float desired_y)
{
  
 while(Stepper_Counter<=Stepper_Final)
 {
  
  stepper();//Turn the stepper
 
  if(Stepper_Counter%(int)(Stepper_Final/Capture_Points)==1)
  {
      digitalWrite(Pin1, LOW);  
      digitalWrite(Pin2, LOW); 
      digitalWrite(Pin3, LOW); 
      digitalWrite(Pin4, LOW); 
      
      R = ultrasonic.MeasureInCentimeters(utr);
      delay(10);
      
      if(Stepper_Dir)
      {
        theta += 360/Capture_Points;
      }
      else
      {
        theta -= 360/Capture_Points;
      }

      theta+=offset_angle;

      if(R<100)
      {
      float x = R*cos(theta*3.14/180);
      float y = R*sin(theta*3.14/180);
      x+=desired_x;
      y+=desired_y;
      
      Serial.print(x);
      Serial.print(",");
      Serial.println(y);
      }
  }//end if
  
 }//end while
 
 digitalWrite(Pin1, LOW);  
 digitalWrite(Pin2, LOW); 
 digitalWrite(Pin3, LOW); 
 digitalWrite(Pin4, LOW);
 
 
 if(Stepper_Dir)
 {
   Stepper_Dir=false;
 }
 else
 {
   Stepper_Dir=true;
 }
 
 Stepper_Counter=1;
  
}



