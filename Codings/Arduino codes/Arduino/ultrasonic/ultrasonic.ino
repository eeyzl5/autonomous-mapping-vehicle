
#include "Ultrasonic.h"
#include "Grove_I2C_Motor_Driver.h"
// default I2C address is 0x0f
#define I2C_ADDRESS 0x0f

Ultrasonic ultrasonic;
void setup()
{
	Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);
}
void loop()
{

	long RangeInCentimeters1;	
  
  RangeInCentimeters1 = ultrasonic.MeasureInCentimeters(12);
  delay(100);

	
	Serial.println(RangeInCentimeters1);//0~400cm

}
