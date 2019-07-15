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
  pinMode(Pin1, OUTPUT);  
  pinMode(Pin2, OUTPUT);  
  pinMode(Pin3, OUTPUT);  
  pinMode(Pin4, OUTPUT);
  pinMode(lb,OUTPUT);
  pinMode(lf,OUTPUT);
  pinMode(rf,OUTPUT);
  pinMode(rb,OUTPUT);

  gyro.init();
  gyro.zeroCalibrate(200,10);

  findZero();

  noInterrupts();           // disable all interrupts
  TCCR2A = 0;
  TCCR2B = 0; 
  TCNT2 = timer2_counter;   // preload timer
  TCCR2B = 0x07;    // 128 prescaler 
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

    //Map_at(0,0);
    //turn(90);
    Move('f',20);
    //Map_at(0,20);
    
    Serial.println("***********");
    while(1);
    //Serial.println(angle);

    
    //while(1);

}

void Move(char _direction, unsigned int input)
{
  unsigned int __speed=input;
  
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
    while(Motor_step<input/2.5)
    {
      if(analogRead(A3)<=12)
      {
        counterL++;
        counterH=0;
        if((counterL==4)&&(state==1))
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
        if(counterH==4)
        {
          state=1;
          counterH=0;
        }
      }  
      analogWrite(lf, 170);
      analogWrite(rf, 135);
      digitalWrite(lb, 0);
      digitalWrite(rb, 0);
    }
    digitalWrite(lf, 1);
    digitalWrite(rf, 1);
    digitalWrite(lb, 1);
    digitalWrite(rb, 1);
    delay(100);
    digitalWrite(lf, 0);
    digitalWrite(rf, 0);
    digitalWrite(lb, 0);
    digitalWrite(rb, 0);
    Motor_step=0;  
  }
  if(_direction=='b')
  {   
    while(Motor_step<input/2.5)
    {
      if(analogRead(A3)<=12)
      {
        counterL++;
        counterH=0;
        if((counterL==4)&&(state==1))
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
        if(counterH==4)
        {
          state=1;
          counterH=0;
        }
      }  
      digitalWrite(lf, 0);
      digitalWrite(rf, 0);
      analogWrite(lb, 170);
      analogWrite(rb, 140);
    }
    digitalWrite(lf, 1);
    digitalWrite(rf, 1);
    digitalWrite(lb, 1);
    digitalWrite(rb, 1);
    delay(100);
    digitalWrite(lf, 0);
    digitalWrite(rf, 0);
    digitalWrite(lb, 0);
    digitalWrite(rb, 0);
    Motor_step=0;   
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



///////////////////////////////////////get_angle////////////////////////////////////////
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
      _angle=_angle+0.01695*Z;
      gyro_read=false;
      update_speed=true;

    }
    return _angle;
}

//////////////////////////////////////Turn//////////////////////////////////////////
void turn(float _target)
{
    int __speed=0;
    int pre__speed;
    float pre_error;
    float error=1;
    float increment;
    float ki=1,kp=5,kd;
    float _angle=0.00;

    offset_angle+=_target;
    
    gyro.zeroCalibrate(100,10);
    TCCR2B = 0x07;    // 1024 prescaler
    TIMSK2 = 0x01;    // enable timer overflow interrupt

    kd=40/_target;
    if(kd>0.75)
    {
      kd=0.75;
    }
    if(kd<0.2)
    {
      kd=0;
    }

    while((_angle<_target-0.5)||(_angle>_target+0.5))
    {
        
      _angle=get_angle(_angle);
      
        
        pre__speed=__speed;
        pre_error=error;
        error=_target-_angle;
        __speed=kp*(error-pre_error)+ki*error+kd*pre__speed;
        update_speed=0;
       
        //__speed=3*(_target-_angle);

    
        if(__speed<=0)
        {
          Move('r',(unsigned int)__speed);
        }
        else
        {
          Move('l',__speed);
        }

    }
    TIMSK2 = 0;   // disable timer overflow interrupt
    Move('s',0);
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
void Map_at(int desired_x,int desired_y)
{
  
 while(Stepper_Counter<=Stepper_Final)
 {
  
  stepper();//Turn the stepper
 
  if(Stepper_Counter%(Stepper_Final/Capture_Points)==1)
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
      
      int x = R*cos(theta*3.14/180);
      int y = R*sin(theta*3.14/180);
      x+=desired_x;
      y+=desired_y;
      
      Serial.print(x);
      Serial.print(",");
      Serial.println(y);
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



