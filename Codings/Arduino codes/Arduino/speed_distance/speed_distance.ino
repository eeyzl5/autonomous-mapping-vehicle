
#include <Wire.h>
#include <ADXL345.h>
#include "Grove_I2C_Motor_Driver.h"
// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f

ADXL345 adxl; //variable adxl is an instance of the ADXL345 library

int velocity=0;
int distance=0;
double xyz[3];
float levelx;
float levely;
float levelz;
float ax=0.00;
float ay=0.00;
float az=0.00;
float temp1=0.00;
float temp2=0.00;

void setup(){
  Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);
  adxlInitialize();
  for(int i=0;i<100;++i)
  {
    adxl.getAcceleration(xyz);
    ax = ax+xyz[0];
    ay = ay+xyz[1];
    az = az+xyz[2];
    delay(10);
  }
  levelx=ax/100;
  levely=ay/100;
  levelz=az/100;
  az=0.00;
}

void loop(){

  
  //Motor.speed(MOTOR1, 100);
  //Motor.speed(MOTOR2, 100);

  //for(int i=0;i<20;++i){
    adxl.getAcceleration(xyz);
    ax = xyz[0]-levelx;
    az = xyz[2]-levelz;
    /*
    if((ax<=0.02)&&(ax>=-0.02))
    {
      ax=0.00;
    }
    if((az<=0.04)&&(az>=-0.04))
    {
      ax=0.00;
    }*/


      temp1=temp1+0.0014*ax;
      velocity=1000*temp1;
      //temp2=temp2+0.001*velocity;
      //distance=10*temp2;

    
    
    Serial.println(velocity);

    delay(1);
    //}

  //Motor.stop(MOTOR1);
  //Motor.stop(MOTOR2);
  
  //while(1);

  

	
}

void adxlInitialize(void)
{
  adxl.powerOn();

  //set activity/ inactivity thresholds (0-255)
  adxl.setActivityThreshold(75); //62.5mg per increment
  adxl.setInactivityThreshold(75); //62.5mg per increment
  adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?
 
  //look of activity movement on this axes - 1 == on; 0 == off 
  adxl.setActivityX(1);
  adxl.setActivityY(1);
  adxl.setActivityZ(1);
 
  //look of inactivity movement on this axes - 1 == on; 0 == off
  adxl.setInactivityX(1);
  adxl.setInactivityY(1);
  adxl.setInactivityZ(1);
 
  //look of tap movement on this axes - 1 == on; 0 == off
  adxl.setTapDetectionOnX(0);
  adxl.setTapDetectionOnY(0);
  adxl.setTapDetectionOnZ(1);
 
  //set values for what is a tap, and what is a double tap (0-255)
  adxl.setTapThreshold(50); //62.5mg per increment
  adxl.setTapDuration(15); //625us per increment
  adxl.setDoubleTapLatency(80); //1.25ms per increment
  adxl.setDoubleTapWindow(200); //1.25ms per increment
 
  //set values for what is considered freefall (0-255)
  adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment
 
  //setting all interrupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );
 
  //register interrupt actions - 1 == on; 0 == off  
  adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  adxl.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  adxl.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);
}


