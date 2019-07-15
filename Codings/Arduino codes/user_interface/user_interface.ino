
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;
int a=125;
int m=1;
int s=0;

void setup() 
{
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    lcd.setRGB(colorR, colorG, colorB);
    
    // Print a message to the LCD.
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  lcd.print("a=");
  delay(1000);
}

void loop() 
{
 int down = digitalRead(8);
  int left = digitalRead(9);
  int up = digitalRead(10);
  int right = digitalRead(11);
  

  if(left==LOW)
  {

  }
  if(right==LOW)
  {

  }
  if(up==LOW)
  {

    ++a;
    delay(400);
  }
  if(down==LOW)
  {

    --a;
    delay(400);
  }



    lcd.setCursor(2, 0);
    lcd.print(a);
    lcd.setCursor(0, 1);
    lcd.print("         confirm");

  
}


