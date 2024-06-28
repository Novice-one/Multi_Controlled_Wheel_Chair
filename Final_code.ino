#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>      //SPI library for communicate with the   nRF24L01+
#include "RF24.h"     //The main library of the nRF24L01+
#include <SoftwareSerial.h>
SoftwareSerial bluetooth(7,6);// RX, TX
RF24 radio(1,2);
char t;
const int enbA = 3;
const int enbB = 5;

#define MLa 8     //left motor 1st pin
#define MLb 9     //left motor 2nd pin
#define MRa 10    //right motor 1st pin
#define MRb 11    //right motor 2nd pin
 
void setup()
{
Serial.begin(9600);
bluetooth.begin(9600);
radio.begin();                               
radio.openReadingPipe(1, pipe);
radio.startListening();  
pinMode(enbA, OUTPUT);
pinMode(enbB, OUTPUT);
pinMode(MLa,OUTPUT);   //left motors forward
pinMode(MLb,OUTPUT);   //left motors reverse
pinMode(MRa,OUTPUT);   //right motors forward
pinMode(MRb,OUTPUT);   //right motors reverse
}
 
void loop() 
{
if(bluetooth.available()>0)
{
  t = bluetooth.read();
  Serial.println(t);
}
 switch(t)
 {
  case 'F':
        {            //move forward(all motors rotate in forward direction)
          digitalWrite(MLa,LOW);
          digitalWrite(MLb,HIGH);
          digitalWrite(MRa,LOW);
          digitalWrite(MRb,HIGH);
        }
   break;
   
  case 'B':
        {      //move reverse (all motors rotate in reverse direction)
          digitalWrite(MLa,HIGH);
          digitalWrite(MLb,LOW);
          digitalWrite(MRa,HIGH);
          digitalWrite(MRb,LOW);
        }
   break;
   
  case 'L':
        {      //turn right (left side motors rotate in forward direction, right side motors doesn't rotate)
          digitalWrite(MLa,LOW); 
          digitalWrite(MLb,HIGH); 
          digitalWrite(MRa,LOW);
          digitalWrite(MRb,LOW);
        }
   break;
   
  case 'R':
        {      //turn left (right side motors rotate in forward direction, left side motors doesn't rotate)
          digitalWrite(MLa,LOW); 
          digitalWrite(MLb,LOW); 
          digitalWrite(MRa,LOW);
          digitalWrite(MRb,HIGH);
        }
   break;
   
  case 'S':
        {           //STOP (all motors stop)
          digitalWrite(MLa,LOW);
          digitalWrite(MLb,LOW); 
          digitalWrite(MRa,LOW);
          digitalWrite(MRb,LOW);
        }
 }
 if (radio.available()){
    radio.read(data, sizeof(data));

     if(data[0] > 380){
      //forward            
      analogWrite(enbA,   RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, HIGH);
       digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4,   LOW);
    }
    
    if(data[0] < 310){
      //backward              
       analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1,   LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4,   HIGH);
    }
     
    if(data[1] > 180){
      //left
      analogWrite(enbA,   RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, HIGH);
       digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4,   HIGH);
    }

    if(data[1] < 110){
      //right
      analogWrite(enbA,   RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, LOW);
       digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4,   LOW);
    }

    if(data[0] > 330 && data[0] < 360 && data[1] > 130 &&   data[1] < 160){
      //stop car
      analogWrite(enbA, 0);
      analogWrite(enbB,   0);
    }
  }
}