int Pin1 = 6; 
int Pin2 = 7; 
int Pin3 = 8; 
int Pin4 = 9; 
int _step = 0; 
int flag = 2100;
int points = 10;
int dir = 0;// false=clockwise, true=counter clockwise
unsigned long int count=1;

void setup() 
{ 
 pinMode(Pin1, OUTPUT);  
 pinMode(Pin2, OUTPUT);  
 pinMode(Pin3, OUTPUT);  
 pinMode(Pin4, OUTPUT);  
} 
 void loop() 
{ 
    stepper();

}

void stepper()
{

  
  switch(_step){ 
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


 if(dir){ 
   _step++; 
 }else{ 
   _step--; 
 } 
 if(_step>7){ 
   _step=0; 
 } 
 if(_step<0){ 
   _step=7; 
 } 
 //delay(1);
 delayMicroseconds(900); 
 count++;
 
 if(count%flag==1)
 {
    dir=~dir;
    count=1;
 }
 if(count%(flag/points)==1)
 {
    digitalWrite(Pin1, LOW);  
    digitalWrite(Pin2, LOW); 
    digitalWrite(Pin3, LOW); 
    digitalWrite(Pin4, LOW); 
    delay(10);
 }
 
}

