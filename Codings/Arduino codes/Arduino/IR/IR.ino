#include "Grove_I2C_Motor_Driver.h"
// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f
#include "rgb_lcd.h"
#include "ITG3200.h"
#include "Ultrasonic.h"
rgb_lcd lcd;
ITG3200 gyro;
Ultrasonic ultrasonic;
int colorR = 0;
int colorG = 0;
int colorB = 0;

int ir1;
int ir1_low=0;
int ir1_high=0;
int wheel_counter=0;
int wheel_flag=0;
int speed1=0,speed2=0;
int sensorValue1;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);
  gyro.init();
  gyro.zeroCalibrate(200,10);
  lcd.setRGB(colorR, colorG, colorB);
  Motor.speed(MOTOR1,90);
  Motor.speed(MOTOR2,90);
  //Interrupt initialize
  cli();
  timer2_init();
  sei();
}

ISR(TIMER2_COMPA_vect)
{//timer2 interrupt 8kHz
  int speed_sensor;
  
  speed_sensor = analogRead(A0);
  sensorValue1=speed_sensor;
  
  if(speed_sensor>20)
  {
    ir1_high=0;
    ir1_low++;
    if(ir1_low==10)//test 10 times
    {
      ir1=0;
      ir1_low=0;
    }
  }
  else
  {
    ir1_low=0;
    ir1_high++;
    if(ir1_high==10)//test 10 times
    {
      ir1=1;
      ir1_high=0;
    }
  }
}

// the loop routine runs over and over again forever:
void loop() {

    //Motor.speed(MOTOR1,90);
    //Motor.speed(MOTOR2,90);
    
    straight(90,5);
    Motor.stop(MOTOR1);
    Motor.stop(MOTOR2);
    delay(500);
    //while(1);
    Serial.println(ir1);


}


void timer2_init()
{
  //set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
}


void straight(int __speed, unsigned int Step)
{
    float az;
    int Z;

    float pre_error=0;
    float error=0;
    float increment;
    float ki=1,kp=0.1;
  
    Step=wheel_counter+Step;
    
    speed1=__speed;
    speed2=__speed;

      while(wheel_counter<Step)
      {
        Serial.println(ir1);
        gyro.getAngularVelocity(&az);
        Z=az;
        /*pre_error=error;
       
        error=az;
        increment=kp*(error-pre_error)+ki*error;
        speed2+=increment;*/

        if(Z>2)
        {
          speed2++;
        }
        if(Z<-2)
        {
          speed2--;
        }

        Motor.speed(MOTOR1, speed1);
        Motor.speed(MOTOR2, speed2);
        
        delayMicroseconds(10);

        if((ir1==1)&&(wheel_flag==0))
        {
          wheel_counter++;
          wheel_flag=1;
        }
        if(ir1==0)
        {
          wheel_flag=0;
        }

        /*Serial.print(speed1);
        Serial.print("         ");
        Serial.print(speed2);
        Serial.print("         ");
        Serial.println(Z);*/
        
       }

       Motor.speed(MOTOR1, -43);
       Motor.speed(MOTOR2, -50);
       delay(100);
    
}


