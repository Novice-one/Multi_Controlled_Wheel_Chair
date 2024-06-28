#include <SPI.h>        //SPI library for communicate with the nRF24L01+
#include  "RF24.h"       //The main library of the nRF24L01+
#include "Wire.h"       //For  communicate
#include "I2Cdev.h"     //For communicate with MPU6050
#include  "MPU6050.h"    //The main library of the MPU6050

MPU6050 mpu;
int16_t  ax, ay, az;
int16_t gx, gy, gz;
int data[2];

RF24 radio(9,10);
                                    
const uint64_t pipe = 0xE8E8F0F0E1LL;

void  setup(void){
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();              //Initialize the MPU object
  radio.begin();                 //Start  the nRF24 communicate     
  radio.openWritingPipe(pipe);   //Sets the address  of the receiver to which the program will send data.
}

void loop(void){
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

 
  data[0] = map(ax,  -17000, 17000, 300, 400 ); //Send X axis data
  data[1] = map(ay, -17000, 17000,  100, 200);  //Send Y axis data
  radio.write(data, sizeof(data));
}