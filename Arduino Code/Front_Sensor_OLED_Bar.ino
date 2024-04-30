#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0,1); // RX, TX
unsigned char data[4]={};
float distance;
const int ledCount = 10;
int displaynum = 0;
int buzzPin =  3; 
int ledPins[] = {
  13, 12, 11, 10, 9, 8, 7 ,6, 5, 4
}; 

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  //M0/ESP32/ESP8266/mega2560/Uno/Leonardo

void setup() {
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(ledPins[thisLed], OUTPUT);
  } // put your setup code here, to run once:
  pinMode(buzzPin, OUTPUT);
  Serial.begin(57600);
  mySerial.begin(9600); 
  u8g2.begin();
}

void loop() {
 do{
     for(int i=0;i<4;i++)
     {
       data[i]=mySerial.read();
     }
  }while(mySerial.read()==0xff);

  mySerial.flush();
 
 if(data[0]==0xff)
    {
      int sum;
      sum=(data[0]+data[1]+data[2])&0x00FF;
      if(sum==data[3])
      {
        distance=(data[1]<<8)+data[2];
        if(distance>30)
          {          

if(distance < 1200){
int distancecm = distance/10;
  u8g2.clearBuffer();                    
  u8g2.setFont(u8g2_font_logisoso28_tn); 
  u8g2.setCursor(30,30);
  u8g2.print(distancecm); 
  u8g2.sendBuffer();                    
int ledLevel = map(distance, 0, 1450, 0, ledCount);

for (int thisLed = 0; thisLed < ledCount; thisLed++) {

    if (thisLed < ledLevel) {
      digitalWrite(ledPins[thisLed], HIGH);
    } else { // turn off all pins higher than the ledLevel:
      digitalWrite(ledPins[thisLed], LOW);
    }
}
} else {
  u8g2.clearBuffer();                    
  u8g2.setFont(u8g2_font_logisoso28_tr); 
  u8g2.setCursor(30,30);
  u8g2.print("OK");  
  u8g2.sendBuffer();
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    digitalWrite(ledPins[thisLed], LOW);
  }

}

            }else 
             {
               Serial.println("Below the lower limit");
             }
      }else Serial.println("ERROR");
     }
 if (distance < 400){
      digitalWrite(buzzPin, HIGH);
     } else {
      digitalWrite(buzzPin, LOW);
     }
delay(100);
}
