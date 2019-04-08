#ifndef FUNCTION_H
#define FUNCTION_H

#include<SoftwareSerial.h>
#include<LiquidCrystal_I2C.h>
#include <SPI.h>
#include "RFID.h"
SoftwareSerial ss(9, 8);    // RX(TX-ESP)   TX(RX_ESP)
LiquidCrystal_I2C lcd(0x27,16,02);




#define STT1  2
#define STT2  3
#define STT3  4
#define STT4  5
#define RST   7
#define Led1  A2
#define Led2  A1

#define MIC 1
#define REMOTE  2
#define WATER 3
#define TIME_UP 4

#define SS_PIN 10
#define RST_PIN 7
RFID rfid(SS_PIN, RST_PIN);



unsigned char reading_card[5]; //for reading card
String Check_Result="";

int Stt=1;

unsigned long Time_Out;

void INIT(){
  Serial.begin(9600);
  ss.begin(9600);
  SPI.begin();
  rfid.init();
  lcd.init();
  lcd.backlight();
  pinMode(Led1,OUTPUT);
  pinMode(Led2,OUTPUT);
  digitalWrite(Led1,1);
  digitalWrite(Led2,1);

  pinMode(STT1,INPUT_PULLUP);
  pinMode(STT2,INPUT_PULLUP);
  pinMode(STT3,INPUT_PULLUP);
  pinMode(STT4,INPUT_PULLUP);
  
}




void Send_ID(unsigned char id[]){
  String temp="";
  for(int i=0; i<4;i++){
    temp+= id[i];
    temp += " ";
  }
  temp =temp + id[4] + ".";


  Serial.println("String id: " + temp);
  ss.print(temp);
}
void Check_Button(){
  if(!digitalRead(STT1)){
    while(!digitalRead(STT1));
    Serial.println("YEU CAU MAY CHIEU");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DANG YEU CAU:");
    lcd.setCursor(0,1);
    lcd.print("MAY CHIEU");
    ss.print("REMOTE,");
    Stt=!Stt;
  }
  if(!digitalRead(STT2)){
    while(!digitalRead(STT2));
    Serial.println("YEU CAU NUOC");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DANG YEU CAU:");
    lcd.setCursor(0,1);
    lcd.print("NUOC UONG");
    ss.print("WATER,");
    Stt=!Stt;
  }
  if(!digitalRead(STT3)){
    while(!digitalRead(STT3));
    Serial.println("YEU CAU MICRO");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DANG YEU CAU:");
    lcd.setCursor(0,1);
    lcd.print("MICRO");
    ss.print("MIC,");
    Stt=!Stt;
  }
  if(!digitalRead(STT4)){
    while(!digitalRead(STT4));
    Serial.println("BAO HET GIO");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BAO HET GIO");
    ss.print("TIME_UP,");
    Stt=!Stt;
  }
  digitalWrite(Led2,Stt);
}

#endif
