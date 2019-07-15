#include "Grove_I2C_Motor_Driver.h"
#define I2C_ADDRESS 0x0f // default I2C address is 0x0f

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
unsigned long int counter=0;


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
  
  noInterrupts();           // disable all interrupts
  TCCR2A = 0;
  TCCR2B = 0;  
  TCNT2 = timer2_counter;   // preload timer
  TCCR2B = 0x07;    // 1024 prescaler 
  TIMSK2 = 0x01;   // enable timer overflow interrupt
  interrupts();             // enable all interrupts

  analogWrite(lf, 130);
  
}

ISR(TIMER2_OVF_vect)
{
  TCNT2 = timer2_counter;   // preload timer
  //digitalWrite(13, digitalRead(13) ^ 1);
}

void loop() {
  int state;
  if(analogRead(A3)<=14)
  {
    state=0;
  }
  else
  {
    state=1;
  }

  Serial.print(counter);
  Serial.print("       ");
  Serial.println(state);
    ++counter;

    if(counter==100)
    {
      Serial.println("***********");
      analogWrite(lf, 0);
      while(1);
    }
  //while(1);
}
