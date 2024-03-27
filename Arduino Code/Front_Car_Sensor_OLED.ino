#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0,1); // RX, TX
unsigned char data[4]={};
float distance;
int buzzPin =  10; 
bool showme = true;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(/* rotation=*/U8G2_R0, /* reset=*/ U8X8_PIN_NONE);    //  M0/ESP32/ESP8266/mega2560/Uno/Leonardo

void setup() {
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
            if (distance > 1200) {
              u8g2.clearBuffer(); 
              u8g2.setFont(u8g2_font_logisoso32_tr);
              u8g2.setCursor(50,50);
              u8g2.print("OK");   
              u8g2.sendBuffer();  
              digitalWrite(buzzPin, LOW);   
            } else {
              u8g2.clearBuffer();                    // clear the internal memory
              u8g2.setFont(u8g2_font_logisoso32_tr);
              if (distance > 10){
                u8g2.setCursor(50,50);}
              if (distance > 100){
                u8g2.setCursor(40,50);}
              if (distance > 1000){
                u8g2.setCursor(27,50);
           }
           
           int distancecm = distance/10;
           // flash display if less than 40cm 
           if (distancecm > 40) {
            u8g2.print(distancecm);   
            u8g2.setCursor(85,50);
            u8g2.setFont(u8g2_font_logisoso22_tr);
            u8g2.print("cm");
            u8g2.sendBuffer(); 
           }
           else {
           if (showme) {
              u8g2.print(distancecm);   
              u8g2.setCursor(85,50);
              u8g2.setFont(u8g2_font_logisoso22_tr);
              u8g2.print("cm");
              u8g2.sendBuffer();
              showme = false;
           } else {
              u8g2.clear();
              u8g2.sendBuffer();
              showme = true;
             }}}                 
          }else 
             {
               Serial.println("Below the lower limit");
             }
      }else Serial.println("ERROR");
     }
     // Sound buzzer if less than 40cm 
     if (distance < 400){
      digitalWrite(buzzPin, HIGH);
     } else {
      digitalWrite(buzzPin, LOW);
     }
     delay(100);

}
