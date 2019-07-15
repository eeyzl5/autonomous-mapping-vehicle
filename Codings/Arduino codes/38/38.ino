
#include "Grove_I2C_Motor_Driver.h"

// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f

void setup() {
  Motor.begin(I2C_ADDRESS);
}

void loop() {
   
  Motor.stop(MOTOR2);
  Motor.speed(MOTOR1, 80);
  delay(2000);
  Motor.speed(MOTOR2, -80);
  Motor.stop(MOTOR1);
  delay(2000);

  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);
  delay(2000);
//////////////////////////////////////
  Motor.stop(MOTOR2);
  Motor.speed(MOTOR1, 80);
  delay(2000);
  Motor.stop(MOTOR1);
  Motor.speed(MOTOR2, 80);
  delay(4000);
  Motor.stop(MOTOR2);
  Motor.speed(MOTOR1, 80);
  delay(2000);
  
  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);
  delay(2000);
}

// End of file
