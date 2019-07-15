#include "Grove_I2C_Motor_Driver.h"
// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f
#include <Wire.h>
#include "ITG3200.h"
#include "Ultrasonic.h"
ITG3200 gyro;
Ultrasonic ultrasonic;

int distance = 0;
int point = 14;
int _speed = 0;
int counter=0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  //pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);

}

// the loop function runs over and over again forever
void loop() {

  
  distance = ultrasonic.MeasureInCentimeters(5);

  if(counter<44)
  {
    if(counter>0)
    {
      Serial.println(distance);
    }
  
    ++counter;
  
    Motor.speed(MOTOR1, 100);
    Motor.speed(MOTOR2, -100);
    delay(27);
  }

  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);


}



