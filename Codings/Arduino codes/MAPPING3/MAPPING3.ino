#include "Ultrasonic.h"
////////////////////////////////Variables////////////////////////////////////////////
int timer2_counter=252;
int Pin1 = 3; 
int Pin2 = 4; 
int Pin3 = 7; 
int Pin4 = 8; 
int lf=6;
int lb=5;
int rf=9;
int rb=10;
int utr=12;
float yaw_x=0.0;
float yaw_y=0.0;
float yaw_angle=0.0;
int Left_motor_step=0;
int Right_motor_step=0;
int Stepper_step = 0; 
int counterL=0;
int counterH=0;
int state=1;
int IR_left_L=0;
int IR_left_H=0;
int IR_right_L=0;
int IR_right_H=0;
int IR_left_state=0;
int IR_right_state=0;
int Vehicle_direction=0;
int Stepper_Final = 4200;
float Capture_Points = 80;
float Range=0.0;
float Stepper_angle=0.0;
boolean Stepper_Zero = HIGH;
boolean Stepper_Dir = true;// false=clockwise, true=Stepper_Counterer clockwise
unsigned int Stepper_Counter=1;
char route[20]={'s','f','r','r','r','r','r','f','f','f','r','f','f','f','f','l','f','f','f','f'};

Ultrasonic ultrasonic;

void setup() 
{ 
 Serial.begin(9600);
 pinMode(Pin1, OUTPUT);  
 pinMode(Pin2, OUTPUT);  
 pinMode(Pin3, OUTPUT);  
 pinMode(Pin4, OUTPUT); 
 pinMode(lf,OUTPUT);
 pinMode(lb,OUTPUT);
 pinMode(rb,OUTPUT);
 pinMode(rf,OUTPUT);

 if(analogRead(A4)<=15)
 {
  IR_left_state=0;
 }
 else
 {
  IR_left_state=1;
 }

 if(analogRead(A1)>999)
 {
  IR_right_state=0;
 }
 else
 {
  IR_right_state=1;
 }

 findZero();

  noInterrupts();           // disable all interrupts
  TCCR2A = 0;
  TCCR2B = 0;  
  TCNT2 = timer2_counter;   // preload timer
  TCCR2B = 0x07;    // 1024 prescaler 
  TIMSK2 = 0x00;   // enable timer overflow interrupt
  interrupts();             // enable all interrupts

} 


ISR(TIMER2_OVF_vect)
{
  TCNT2 = timer2_counter;   // preload timer
  int IR_left_read=analogRead(A4);
  int IR_right_read=analogRead(A1);
      if(IR_left_read<=15)
      {
        IR_left_L++;
        IR_left_H=0;
        if((IR_left_L==5)&&(IR_left_state==1))
        {
          IR_left_state=0;
          ++Left_motor_step;      
          IR_left_L=0;
        }
      }
      else
      {
        IR_left_L=0;
        IR_left_H++;
        if((IR_left_H==5)&&(IR_left_state==0))
        {
          IR_left_state=1;
          ++Left_motor_step;
          IR_left_H=0;
        }
      }  


      if(IR_right_read>999)
      {
        IR_right_L++;
        IR_right_H=0;
        if((IR_right_L==5)&&(IR_right_state==1))
        {
          IR_right_state=0;
          ++Right_motor_step;      
          IR_right_L=0;
        }
      }
      else
      {
        IR_right_L=0;
        IR_right_H++;
        if((IR_right_H==5)&&(IR_right_state==0))
        {
          IR_right_state=1;
          ++Right_motor_step;
          IR_right_H=0;
        }
      }  
      
}


void loop() 
{ 

  for(int ii=0;ii<8;++ii)
  {
    Move(route[ii],1000);
    Map_at(yaw_x,yaw_y);
  }

  //Map_at(yaw_x,yaw_y);
  //Move('f',1);
  //Map_at(yaw_x,yaw_y);
  //Move('f',1);
  //Map_at(yaw_x,yaw_y);
  //Move('f',2);
  //Map_at(yaw_x,yaw_y);

  //Serial.println(Vehicle_direction);
  Serial.println("******");
  while(1);

}


///////////////////////////////////Move////////////////////////////////
void Move(char _direction, unsigned int _time)
{
  int on1,on2,off1,off2;
  TIMSK2 = 0x01;   // enable timer overflow interrupt
  
  if(_direction=='s')
  {

  }
  else if(_direction=='f')
  {
    analogWrite(lf, 255);
    analogWrite(rf, 255);
    digitalWrite(lb, 0);
    digitalWrite(rb, 0);
  }
  else if(_direction=='b')
  {
    analogWrite(lb, 255);
    analogWrite(rb, 255);
    digitalWrite(lf, 0);
    digitalWrite(rf, 0);
  }
  else if(_direction=='r')
  {
    analogWrite(lf, 255);
    analogWrite(rf, 180);
    digitalWrite(lb, 0);
    digitalWrite(rb, 0);
  }
  else if(_direction=='l')
  {
    analogWrite(lf, 180);
    analogWrite(rf, 255);
    digitalWrite(lb, 0);
    digitalWrite(rb, 0);
  }

    delay(_time);
    
  //Left_motor_step=0; 
  //Right_motor_step=0; 
  digitalWrite(lf, 0);
  digitalWrite(rf, 0);
  digitalWrite(lb, 0);
  digitalWrite(rb, 0);
  delay(1000);
  float yaw_rad=(Right_motor_step-Left_motor_step)*1.31/17;
    if(yaw_rad==0)
    {
      yaw_angle=0;
      yaw_x=Left_motor_step*1.3;
      yaw_y=0;
    }
    else
    {
      yaw_angle=yaw_rad*180/3.14;
      yaw_x=(Left_motor_step*1.31/yaw_rad+8.5)*sin(yaw_rad);
      yaw_y=(Left_motor_step*1.31/yaw_rad+8.5)*(1-cos(yaw_rad));
    }

    Serial.print(yaw_x);
    Serial.print(",");
    Serial.println(yaw_y);

  TIMSK2 = 0x00;   // disable timer overflow interrupt
}

/////////////////////////////////stepper////////////////////////////////////////
void stepper()
{
  
  switch(Stepper_step){ 
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
   Stepper_step++; 
 }else{ 
   Stepper_step--; 
 } 
 if(Stepper_step>7){ 
   Stepper_step=0; 
 } 
 if(Stepper_step<0){ 
   Stepper_step=7; 
 }
 
 delayMicroseconds(800);   
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
 
 Stepper_Counter=1;
 Stepper_Dir = false;

 do
 {
  //Filter and Read A1
  if(analogRead(A2)<=18)
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
  if(analogRead(A2)<=18)
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
void Map_at(float X_global,float Y_global)
{

 float region[8];
 float Total=0.00;
 int region_counter=0;
  
 while(Stepper_Counter<=Stepper_Final)
 {
  
  stepper();
 
  if(Stepper_Counter%(int)(Stepper_Final/Capture_Points)==1)
  {
      digitalWrite(Pin1, LOW);  
      digitalWrite(Pin2, LOW); 
      digitalWrite(Pin3, LOW); 
      digitalWrite(Pin4, LOW); 
      
      Range = ultrasonic.MeasureInCentimeters(utr);
      delay(1);
      
      if(Stepper_Dir)
      {
        Stepper_angle += 360/Capture_Points;
      }
      else
      {
        Stepper_angle -= 360/Capture_Points;
      }

      Total += Range;

      if(Stepper_angle==45*(region_counter+1))
      {
        region[region_counter]=Total;
        region_counter++;
        Total=0.00;
      }

      //Stepper_angle+=yaw_angle;
      
      if(Range<150)
      {
        float X_local = cos((Stepper_angle+yaw_angle)*3.14/180)*Range;
        float Y_local = sin((Stepper_angle+yaw_angle)*3.14/180)*Range;
        //Convert the local map to global map
        X_local += X_global;
        Y_local += Y_global;
        Serial.print(X_local);
        Serial.print(",");
        Serial.println(Y_local);
      }
      
  }//end if
  
 }//end while
 
 digitalWrite(Pin1, LOW);  
 digitalWrite(Pin2, LOW); 
 digitalWrite(Pin3, LOW); 
 digitalWrite(Pin4, LOW);

 float temp[4];
 
 temp[0]=region[0]+region[7];
 temp[1]=region[1]+region[2];
 temp[2]=region[3]+region[4];
 temp[3]=region[5]+region[6];

 Vehicle_direction=0;

 for(int i=1;i<4;++i)
 {
  if(temp[i]>temp[i-1])
  {
    Vehicle_direction=i;
  }
 }
 
 if(Stepper_Dir)
 {
   Stepper_Dir=false;
 }
 else
 {
   Stepper_Dir=true;
 }
 
 Stepper_Counter=1;
 //Stepper_angle=0; 
  
}

