#include<SoftwareSerial.h>
#include<LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RFID.h>
#include<Arduino_JSON.h>
SoftwareSerial ss(9, 8);    // RX(TX-ESP)   TX(RX_ESP)
LiquidCrystal_I2C lcd(0x27,16,02);




#define STT1  2
#define STT2  3
#define STT3  4
#define STT4  5
#define RST   7
#define Led1  A2
#define Led2  A3

#define SS_PIN 10
#define RST_PIN 7
RFID rfid(SS_PIN, RST_PIN);

JSONVar myObject;

unsigned char reading_card[5]; //for reading card

int Time_Out;

void INIT(){
  Serial.begin(9600);
  ss.begin(9600);
  SPI.begin();
  rfid.init();
  lcd.init();
  lcd.backlight();
  pinMode(STT1,INPUT_PULLUP);
  pinMode(STT2,INPUT_PULLUP);
  pinMode(STT3,INPUT_PULLUP);
  pinMode(STT4,INPUT_PULLUP);
  
}




void Send_ID(unsigned char id[]){
  myObject["id0"]=id[0];
  myObject["id1"]=id[1];
  myObject["id2"]=id[2];
  myObject["id3"]=id[3];
  myObject["id4"]=id[4];

  String jsonString = JSON.stringify(myObject);
  Serial.println(jsonString);
  //ss.print(jsonString);
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
    ss.print(1);
  }
  if(!digitalRead(STT2)){
    while(!digitalRead(STT2));
    Serial.println("YEU CAU NUOC");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DANG YEU CAU:");
    lcd.setCursor(0,1);
    lcd.print("NUOC UONG");
    ss.print(2);
  }
  if(!digitalRead(STT3)){
    while(!digitalRead(STT3));
    Serial.println("YEU CAU MICRO");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DANG YEU CAU:");
    lcd.setCursor(0,1);
    lcd.print("MICRO");
    ss.print(3);
  }
  if(!digitalRead(STT4)){
    while(!digitalRead(STT4));
    Serial.println("BAO VAO LOP");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BAO DA VAO LOP");
    ss.print(4);
  }
}
