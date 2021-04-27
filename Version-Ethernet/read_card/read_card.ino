#include <MFRC522.h>
#include "SPI.h"
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

#define BUZZER_PIN A0

#define RST_PIN_MFRC 9 // RST-PIN for RC522
#define SS_PIN_MFRC  10  // SDA-PIN for RC522
MFRC522 mfrc522(SS_PIN_MFRC, RST_PIN_MFRC);   // Create MFRC522 instance

SoftwareSerial software_serial(5, 6); // RX, TX
LiquidCrystal_I2C lcd(0x27, 16, 2);

void dump_byte_array(byte *buffer, byte bufferSize);
bool readCardFunc(void);
void(* resetFunc) (void) = 0; //cài đặt hàm reset
void Toggle_Buzzer();

String strID = "";
String httpRequestStatus = "";
bool requesting = false;
long double beginTimeRequest = 0;

void setup() {
  SPI.begin();           // Init SPI bus
  Serial.begin(9600);
  software_serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  mfrc522.PCD_Init();    // Init MFRC522

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Dang khoi dong");
  Toggle_Buzzer();
  while (!software_serial.available()) {
    delay(1);
  };
  String start = "";
  while (software_serial.available()) {
    char c = software_serial.read();
    start = start + c;
    Serial.print(c);
  };
  if (start == "START") {
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("DD san sang");
    lcd.setCursor(2, 1);
    lcd.print("Moi quet the");
  }
  Toggle_Buzzer();
}

void loop() {
  if (!requesting && software_serial.available()) {
    String start = "";
    while (software_serial.available()) {
      char c = software_serial.read();
      start = start + c;
      Serial.print(c);
    };
    if (start == "START") { 
      resetFunc();
    }
  }

  while (requesting && millis() - beginTimeRequest < 5000) {
    if (millis() - beginTimeRequest > 4500) {
      Dislay_LCD(404);
      requesting = false;
    } else {
      lcd.setCursor(1, 0);
      lcd.print("Dang diem danh");
      lcd.setCursor(0, 1);
      lcd.print("Vui long cho doi");
    }

    if (software_serial.available()) {
      requesting = false;
      String httpRequestStatus = "";
      while (software_serial.available()) {
        char c = software_serial.read();
        delay(10);
        httpRequestStatus += c;
      }
      Serial.print("HTTP Request Status: ");
      Serial.println(httpRequestStatus);

      if (httpRequestStatus == "OK") {
        Dislay_LCD(200);
        Serial.println(F("Request Okay"));
        httpRequestStatus = "";
      } else if (httpRequestStatus == "FAIL") {
        Dislay_LCD(502);
        Serial.println(F("Request Fail"));
        httpRequestStatus = "";
      } else {
        Dislay_LCD(404);
        Serial.println(F("Page Not Found"));
        httpRequestStatus = "";
      }
    }
  }

  if (readCardFunc() && !requesting) {
    Serial.println(F("Ma the la: "));
    Serial.println(strID);
    String dataSend = strID;
    software_serial.print(dataSend);
    Serial.println(F("Du lieu da gui: "));
    Serial.println(dataSend);
    requesting = true;
    beginTimeRequest = millis();
    lcd.clear();
  }
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  strID = "";
  for (byte i = 0; i < bufferSize; i++) {
    String valHex = String(buffer[i], HEX);
    if (valHex.length() == 1) {
      valHex = String(0) + valHex;
    }
    strID += valHex;
  }
  strID.toUpperCase();
}

bool readCardFunc(void) {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return false;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return false;
  }
  // Show some details of the PICC (that is: the tag/card)
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  return true;
}

void Dislay_LCD(int CMD) {
  lcd.clear();

  if (CMD == 200) {
    lcd.setCursor(1, 0);
    lcd.print("DD thanh cong");
    Toggle_Buzzer();
    delay(1000);
  } else if (CMD == 502) {
    lcd.setCursor(2, 0);
    lcd.print("DD that bai");
    lcd.setCursor(0, 1);
    lcd.print("Vui long thu lai");
    digitalWrite(BUZZER_PIN, HIGH);   
    delay(2000);
    digitalWrite(BUZZER_PIN, LOW);
  } else if (CMD == 404) {
    lcd.setCursor(2, 0);
    lcd.print("DD that bai");
    lcd.setCursor(0, 1);
    lcd.print("Vui long thu lai");
    digitalWrite(BUZZER_PIN, HIGH);   
    delay(2000);
    digitalWrite(BUZZER_PIN, LOW);
  }
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("DD san sang");
  lcd.setCursor(2, 1);
  lcd.print("Moi quet the");
}

void Toggle_Buzzer(){
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
  delay(200);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
}
