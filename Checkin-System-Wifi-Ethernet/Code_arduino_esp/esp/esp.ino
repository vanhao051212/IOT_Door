//==============================================================================================
//                                    declare
//==============================================================================================

#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
SoftwareSerial ss(14, 5);

#define AUTO_RESET
//host để kiểm tra id của thẻ rf
const char* Send_Check_ID = "http://192.168.20.135:8000/checkRFID";
const char* Send_Cmd = "http://192.168.20.135:8000/receivedCmd";

const char* ssid = "UIT-hidden";
const char* pwdWifi = "Uit201913032019";

bool sendNotice(String CardID, String command);

String strID = "";
char c;

//thời gian để gửi event yêu cầu join room lên server
unsigned long time_join_room;

#ifdef AUTO_RESET
//thời gian reset lại
unsigned long time_out; //time out reset
#endif


//check request
void Execute_Request(char CMD);

//==============================================================================================
//                                    setup
//==============================================================================================

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  #ifdef AUTO_RESET
    time_out = millis();
  #endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pwdWifi);

  while (WiFi.status() != WL_CONNECTED)
  {
    #ifdef AUTO_RESET
        if (millis() - time_out > 30000) {
          ESP.restart();
        }
    #endif
      delay(500);
  }
  delay(5000);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.print("START");
  ss.print("START");
  #ifdef AUTO_RESET
    time_out = millis();
  #endif
}

//==============================================================================================
//                                    loop
//==============================================================================================

void loop() {
  if (millis() - time_out > 1800000) {
    Serial.print("START");
    ss.print("START");
    delay(500);
    ESP.restart();
  }

  if (ss.available()) {
    while (ss.available()) {
      c = ss.read();
      strID = strID + c;
      delay(10);
    }

    if (strID != "START") {
      if (sendNotice(strID, "M06")) {
        Serial.print("OK");
        ss.print("OK");
      } else {
        Serial.print("FAIL");
        ss.print("FAIL");
      }
      strID = "";
    } else {
      strID = "";
      ESP.restart();
    }
  }
}

//==============================================================================================
//                                    define function
//==============================================================================================

bool sendNotice(String CardID, String command) {
  HTTPClient http;
  http.begin(Send_Cmd);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(String("RoomID=R016&") + String("CardID=") + CardID + '&' + String("CMD=") + command);
  String payload = http.getString();
  http.end();
  // if successful
  if (httpCode == 200) {
    if (payload == "OK") {
      return true;
    }
  } else {
    return false;
  }
}
