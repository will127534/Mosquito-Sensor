#include "TM1637.h"
#define CLK 5//pins definitions for TM1637 and can be changed to other ports
#define DIO 6
TM1637 tm1637(CLK,DIO);

volatile uint32_t lastInterrupt = 0;
volatile uint32_t length = 0;
volatile bool interrupted = 0;
volatile bool eventStart = 0;

void pinChanged(){
  //Serial.println("INT");
  if(!digitalRead(7)){
    //Serial.println("Low");
    if(millis()-lastInterrupt>500){
      lastInterrupt = millis();
      eventStart = 1;
    }
    
  }
  else{
    //Serial.println("High");
    if(!interrupted && eventStart==1){
      length = millis()-lastInterrupt;
      interrupted = 1;
      eventStart = 0;
    }
  }
}

void setup(){
  Serial.begin(115200);
  Serial1.begin(115200);
  //while(!Serial);
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);
  
  Serial.println("Started");

  pinMode(7, INPUT);
  attachInterrupt(4,pinChanged, CHANGE);
  pinMode(A0, INPUT);

  tm1637.display(0,0);
  tm1637.display(1,0);
  tm1637.display(2,0);
  tm1637.display(3,0);
}

uint32_t count = 0;
uint32_t pulseStart = 0;
bool pulseEnter = 0;

void loop(){
  /*
  if(analogRead(A0)<100){
    pulseStart = millis();
    pulseEnter = 1;
  }
  if(analogRead(A0)>=100 && pulseEnter == 1){
    pulseEnter = 0;
    Serial.print(count);Serial.print(",");Serial.println(millis()-pulseStart);
    count ++;
  }*/

  if(interrupted){
      count++;
      Serial.print(count);Serial.print(",");Serial.println(length);
      Serial1.print(count);Serial1.print(",");Serial1.println(length);

      uint8_t disp = count%10000;
      tm1637.display(0,disp/1000);
      disp = disp%1000;
      tm1637.display(1,disp/100);
      disp = disp%100;
      tm1637.display(2,disp/10);
      disp = disp%10;
      tm1637.display(3,disp);
      
      interrupted = 0;
  }
}
