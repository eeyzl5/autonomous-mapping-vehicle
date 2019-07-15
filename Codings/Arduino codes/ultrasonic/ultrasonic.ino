
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
  long RangeInCentimeters2;
	
  
    RangeInCentimeters1 = ultrasonic.MeasureInCentimeters(6); // two measurements should keep an interval
    delay(10);

    RangeInCentimeters2 = ultrasonic.MeasureInCentimeters(5);
    delay(10);

  

	
	Serial.print(RangeInCentimeters1);//0~400cm
  Serial.print(",");//0~400cm
	Serial.println(RangeInCentimeters2);//0~400cm

}
