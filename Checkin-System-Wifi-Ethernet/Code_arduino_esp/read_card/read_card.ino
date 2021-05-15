#include <MFRC522.h>
#include "SPI.h"
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

#define BUZZER_PIN A0
#define SELECT_MODE 4
#define WIFI_MODE HIGH
#define RST_PIN_MFRC 9 // RST-PIN for RC522
#define SS_PIN_MFRC  10  // SDA-PIN for RC522

MFRC522 mfrc522(SS_PIN_MFRC, RST_PIN_MFRC);   // Create MFRC522 instance

SoftwareSerial software_serial(5, 6); // RX, TX
SoftwareSerial software_serial_esp(2, 3); // RX, TX

LiquidCrystal_I2C lcd(0x27, 16, 2);

void dump_byte_array(byte *buffer, byte bufferSize);
bool readCardFunc(void);
void(* resetFunc) (void) = 0; //cài đặt hàm reset
void Toggle_Buzzer();

String strID = "";
String httpRequestStatus = "";
bool requesting = false;
long double beginTimeRequest = 0;
int previousButtonState = 0;

void setup() {
  SPI.begin();           // Init SPI bus
  Serial.begin(9600);
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(SELECT_MODE, INPUT);
  previousButtonState = digitalRead(SELECT_MODE);
  if (digitalRead(SELECT_MODE) != WIFI_MODE){
    software_serial.begin(9600);
  } else {
    software_serial_esp.begin(9600);
  }
  mfrc522.PCD_Init();    // Init MFRC522
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Dang khoi dong");
  if (digitalRead(SELECT_MODE) != WIFI_MODE) {
    lcd.setCursor(3, 1);
    lcd.print("LAN mode");
  } else {
    lcd.setCursor(3, 1);
    lcd.print("Wifi mode");
  }
  Toggle_Buzzer();
  String start = "";
  if (digitalRead(SELECT_MODE) != WIFI_MODE) {
    while (!software_serial.available()) {
      delay(5);
    };
    while (software_serial.available()) {
      char c = software_serial.read();
      start = start + c;
      Serial.print(c);
      delay(10);
    }
  } else {
    while (!software_serial_esp.available()) {
      delay(5);
    };
    while (software_serial_esp.available()) {
      char c = software_serial_esp.read();
      start = start + c;
      Serial.print(c);
      delay(10);
    };
  }
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
  if (previousButtonState != digitalRead(SELECT_MODE)) {
    if (digitalRead(SELECT_MODE) != WIFI_MODE){
      software_serial.begin(9600);
      software_serial_esp.end();
    } else {
      software_serial_esp.begin(9600);
      software_serial.end();
    }
    previousButtonState = digitalRead(SELECT_MODE);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dang chon");
    if (digitalRead(SELECT_MODE) != WIFI_MODE) {
      lcd.setCursor(6, 1);
      lcd.print("che do LAN");
    } else {
      lcd.setCursor(5, 1);
      lcd.print("che do wifi");
    }
    delay(2000);
    Dislay_LCD(0);
  }
  
  // cai nay cho chuc năng reset
  if (!requesting && (software_serial.available() || software_serial_esp.available())) {
    String start = "";
    if (digitalRead(SELECT_MODE) != WIFI_MODE) {
      while (software_serial.available()) {
        char c = software_serial.read();
        start = start + c;
        Serial.print(c);
        delay(10);
      };
      if (start == "START") {
        software_serial_esp.print("START");
        resetFunc();
      }
    } else {
      while (software_serial_esp.available()) {
        char c = software_serial_esp.read();
        start = start + c;
        Serial.print(c);
        delay(10);
      }
      if (start == "START") {
        resetFunc();
      }
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

    if (software_serial.available() || software_serial_esp.available()) {
      requesting = false;
      String httpRequestStatus = "";
      if (digitalRead(SELECT_MODE) != WIFI_MODE) {
        while (software_serial.available()) {
          char c = software_serial.read();
          delay(10);
          httpRequestStatus += c;
        }
      } else {
        while (software_serial_esp.available()) {
          char c = software_serial_esp.read();
          delay(10);
          httpRequestStatus += c;
        }
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

  //cai nay doc the
  if (readCardFunc() && !requesting) {
    Serial.println(F("Ma the la: "));
    Serial.println(strID);
    String dataSend = strID;
    if (digitalRead(SELECT_MODE) != WIFI_MODE) {
      software_serial.print(dataSend);
    } else {
      software_serial_esp.print(dataSend);
    }
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

void Toggle_Buzzer() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
  delay(200);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
}
