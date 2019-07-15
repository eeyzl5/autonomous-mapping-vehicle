#include "Grove_I2C_Motor_Driver.h"
#define I2C_ADDRESS 0x0f // default I2C address is 0x0f
//#include <Wire.h>
#include <math.h>
#include "rgb_lcd.h"
#include "ITG3200.h"
#include "Ultrasonic.h"
rgb_lcd lcd;
ITG3200 gyro;
Ultrasonic ultrasonic;
int colorR = 0;
int colorG = 0;
int colorB = 0;

////////////////////////////////Variables////////////////////////////////////////////
int Position=0;
int distance1 = 0;
int _speed = 0;
int num;
int no_points=8;
int largest_dis=0;
int speed1=0,speed2=0;
int data_counter;
int ir1;
int ir1_low=0;
int ir1_high=0;
int wheel_counter=0;
int wheel_flag=0;
int row_counter=0;
int data[80][4];
int route=1;

ISR(TIMER2_COMPA_vect)
{//timer2 interrupt 8kHz
  int speed_sensor;
  
  speed_sensor = analogRead(A0);
  
  if(speed_sensor>16)
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
////////////////////////////////Initilization///////////////////////////////////////
void setup() {
  Serial.begin(9600);
  Motor.begin(I2C_ADDRESS);
  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);
  gyro.init();
  gyro.zeroCalibrate(200,10);
  //lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  //check();
}

/////////////////////////////////////Main//////////////////////////////////////////
void loop() {
  

  //find route
  /*distance1 = ultrasonic.MeasureInCentimeters(5);
  delay(1);
  
  if(distance1>30)
  {
    straight(90,1);
  }
  else
  {
    check();
  }*/
 
  
  /*distance1 = ultrasonic.MeasureInCentimeters(5)+8;
  Serial.print(x);
  Serial.print("     ");
  Serial.println(y);*/



  //Mapping
  Map_at(0,0);

  filtering();
  print_data();

  /*for(int i=0;i<10;++i)
  {
    straight(90*(route),5);
    Map_at(20*(route),0);

    filtering();
    print_data();
  }*/
  
  straight(90,5);
  Map_at(20,0);

  filtering();
  print_data();

  straight(90,5);
  Map_at(40,0);

  filtering();
  print_data();

  straight(90,5);
  Map_at(60,0);
  
  filtering();
  print_data();
  
  //Filtering the data (x)
  for(int i=1;i<row_counter-1;++i)
  {
    int test1=data[i][1]-data[i-1][1];
    int test2=data[i][1]-data[i+1][1];
    
    if((test1>3)&&(test2>3))
    {
      for(int j=i;j<row_counter-1;++j)
      {
        data[j][0]=data[j+1][0];
        data[j][1]=data[j+1][1];
        data[j][2]=data[j+1][2];
        data[j][3]=data[j+1][3];
      }
      i--;
      row_counter--;
    }

    
  }

  print_data();

  //Filtering the data (y)
  for(int i=1;i<row_counter-1;++i)
  {
    int test1=data[i][2]-data[i-1][2];
    int test2=data[i][2]-data[i+1][2];
    
    if((test1>3)&&(test2>3))
    {
      for(int j=i;j<row_counter-1;++j)
      {
        data[j][0]=data[j+1][0];
        data[j][1]=data[j+1][1];
        data[j][2]=data[j+1][2];
        data[j][3]=data[j+1][3];
      }
      i--;
      row_counter--;
    }

    
  }

  filtering();

  print_data();

  while(1);
  
  //Motor.stop(MOTOR1);
  //Motor.stop(MOTOR2);



  //Serial.println("**************************");
  
}

//////////////////////////////////Timer2 Initialization//////////////////////////
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

///////////////////////////////////Straight/////////////////////////////////////////
void straight(int __speed, unsigned int Step)
{
    timer2_init();
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
        gyro.getAngularVelocity(&az);
        Z=az;

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
        
       }
       TCCR2B = 0;
       Motor.speed(MOTOR1, -43);
       Motor.speed(MOTOR2, -50);
       delay(100);
       Motor.stop(MOTOR1);
       Motor.stop(MOTOR2);
    
}

///////////////////////////////////////Degree////////////////////////////////////////
float Degree(float adder)
{
    float az;
    int Z;

    for(int i=0;i<10;++i)
    {
      gyro.getAngularVelocity(&az);             //d
      //Z=az;
      adder=adder+0.00292*az;
      delayMicroseconds(50);                                         //d
    }
    
    return adder;
}

//////////////////////////////////////Turn//////////////////////////////////////////
void turn(float _angle)
{
    int __speed;
    float pre_error;
    float error=1;
    float ki=5,kp=7;
    float degree=0.00;


    while((degree<_angle-0.1)||(degree>_angle+0.1))
    {
        degree=Degree(degree);                          //d
        pre_error=error;
        error=_angle-degree;
        __speed=kp*(error-pre_error)+ki*error;

        if((__speed>-30)&&(__speed<0))
        {
            __speed=-30;
        }
        if((__speed<30)&&(__speed>0))
        {
            __speed=30;
        }
        
    
        Motor.speed(MOTOR1,__speed);
        Motor.speed(MOTOR2,(-1)*__speed);


    }
    Motor.stop(MOTOR1);
    Motor.stop(MOTOR2);

}

///////////////////////////////////////Mapping//////////////////////////////////////
void Map_at(int desired_x,int desired_y)
{
  int theta;
  int front=0,back=0;
  float deg_=0;
  float x,y;
  double temp;
  int column_counter=0;
  int flag=1;

  
  for(int i=0;i<no_points;++i)
  {
    theta=i*(360/no_points);
    distance1 = ultrasonic.MeasureInCentimeters(5)+8;
    delay(1);

    x=distance1*cos(theta*PI/180);
    y=distance1*sin(theta*PI/180);
    x+=desired_x;
    y+=desired_y;

    temp=sqrt(x*x+y*y);
    deg_=(acos(x/temp))*180/PI;               //calculate degree corresponding to Position0

    if(y<0)
    {
      deg_=360-deg_;
    }


    if(theta==0)
    {
      front=distance1;
    }
    if(theta==90)
    {
      back=distance1;
    }

    if(flag==1)
    {
      if((front-back>0)&&(front>30))
      {
        route=1;
      }
      if((front-back<=0)&&(back>30))
      {
        route=-1;
      }
    }
    
    if(back<30)
    {
      route=1;
      flag=0;
    }
    if(front<30)
    {
      route=-1;
      flag=0;
    }
    
    
    turn(360/no_points);
    Motor.stop(MOTOR1);
    Motor.stop(MOTOR2);

    column_counter=0;
    data[row_counter][column_counter]=temp;
    column_counter++;
    data[row_counter][column_counter]=x;
    column_counter++;
    data[row_counter][column_counter]=y;
    column_counter++;
    data[row_counter][column_counter]=deg_;

    row_counter++;
    
    /*Serial.print(temp);
    Serial.print("     ");
    Serial.print(x);
    Serial.print("     ");
    Serial.print(y);
    Serial.print("     ");
    Serial.println(deg_);*/
  }

  Position++;
  Motor.stop(MOTOR1);
  Motor.stop(MOTOR2);

  
}

void print_data()
{
  //Print data
  for(int i=0;i<row_counter;++i)
  {
    for(int j=0;j<4;++j)
    {
      Serial.print(data[i][j]);
      Serial.print("        ");
    }
    Serial.println("");
  }

  Serial.println("**************************");
}


void filtering()
{
  
  //Bubble sort the data in terms of angle
  for(int i=0;i<(row_counter-1);++i)
  {
    for(int o=0;o<(row_counter-(i+1));++o)
    {
      if(data[o][3]>data[o+1][3])
      {
        int t[4];
        for(int j=0;j<4;++j)
        {
          t[j]=data[o][j];
          data[o][j]=data[o+1][j];
          data[o+1][j]=t[j];
        }
      }
    }
  }

  
  //Filtering the data (distance1)
  for(int i=1;i<row_counter-1;++i)
  {
    int test1=data[i][0]-data[i-1][0];
    int test2=data[i][0]-data[i+1][0];
    
    if(test1+test2>12)
    {
      for(int j=i;j<row_counter-1;++j)
      {
        data[j][0]=data[j+1][0];
        data[j][1]=data[j+1][1];
        data[j][2]=data[j+1][2];
        data[j][3]=data[j+1][3];
      }
      i--;
      row_counter--;
    }

    
  }

}

/////////////////////////////////////Check////////////////////////////////////////
void check()
{
  int counter=0;
  int distance=0;
  
  for(counter=0;counter<no_points;++counter)
  {
    distance = ultrasonic.MeasureInCentimeters(5);
    turn(360/no_points);
    if((largest_dis<distance)&&(distance<400))
    {
      largest_dis=distance;
      num=counter;
    }
  }

  //_speed=(distance-point)*10;
  distance = ultrasonic.MeasureInCentimeters(5);

  if((largest_dis>distance)||(distance>400))
   {
     for(counter=0;counter<(no_points-num);++counter)
     {
       turn(-360/no_points);
     }
   }
   
}

