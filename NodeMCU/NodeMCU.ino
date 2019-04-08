#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <MFRC522.h>
#define RST_PIN 15 // RST-PIN for RC522 - RFID - SPI - Modul GPIO15 
#define SS_PIN  2  // SDA-PIN for RC522 - RFID - SPI - Modul GPIO2
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <SocketIoClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;

const char* Send_Check_ID="http://192.168.0.112:3000/checkRFID";
const char* Host_Socket = "192.168.0.112";
unsigned int Port_Socket = 3000;
const char* ssid = "Baby I'm unreal";
const char* pwmWifi = "417417417";

bool Check_ID(String ID){
  HTTPClient http;
  http.begin(Send_Check_ID);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode=http.POST(String ("ID=")+ ID); 
  String payload=http.getString();
  http.end();   
  // if successful
  if(httpCode==200){
    if (payload == "ACCEPT") {
      return true;
    }
  }
  return false;
}

bool joinedRoom = false;

void join_success(const char * payload, size_t length) {
  joinedRoom = true;
  USE_SERIAL.printf("join success");
}

void handleMess(const char * payload, size_t length) {
  Serial.println(payload);
  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(payload);

  String Mess = root["Mess"];

  lcd.clear();

  lcd.setCursor(0, 0);
  // print message
  lcd.print("Message: ");
  lcd.setCursor(0,1);
  lcd.print(Mess);
}

bool ableSendMess = false;

// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
  String strSend="";
  for (byte i = 0; i < bufferSize; i++) {
    if(i!=3)
      strSend+=String(buffer[i]) + " ";
    else
      strSend+=String(buffer[i]);
  }
  if (Check_ID(strSend)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card accept!");
    ableSendMess=true;
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wrong card ID");
  }
  Serial.println(strSend);
}

unsigned long time_join_room;

void setup() {
    // initialize LCD
    lcd.init();
    // turn on LCD backlight                      
    lcd.backlight();
    
    USE_SERIAL.begin(115200);

    SPI.begin();           // Init SPI bus
    mfrc522.PCD_Init();    // Init MFRC522 

    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }

    WiFiMulti.addAP(ssid, pwmWifi);

    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    webSocket.on("join-success", join_success);
    webSocket.on("server-send-mess-to-esp", handleMess);
    webSocket.begin(Host_Socket, Port_Socket, "/socket.io/?transport=websocket");
    // use HTTP Basic Authorization this is optional remove if not needed
    //webSocket.setAuthorization("username", "password");
    time_join_room = millis();
}

bool printedReady = false;

void loop() {
    if (!joinedRoom && (millis()-time_join_room>5000)) {
      time_join_room = millis();
      webSocket.emit("esp-send-join-room", "{\"RoomID\":\"R201\"}");
    }
    if (Serial.available() && ableSendMess) {
      char val = Serial.read();
      switch(val) {
        case '1':
          webSocket.emit("esp-send-mess", "{\"RoomID\":\"R201\",\"Mess\":\"M01\"}");
          break;
        case '2':
          webSocket.emit("esp-send-mess", "{\"RoomID\":\"R201\",\"Mess\":\"M02\"}");
          break;
        case '3':
          webSocket.emit("esp-send-mess", "{\"RoomID\":\"R201\",\"Mess\":\"M03\"}");
          break;
        case '4':
          webSocket.emit("esp-send-mess", "{\"RoomID\":\"R201\",\"Mess\":\"M04\"}");
          break;
        case '5':
          webSocket.emit("esp-send-mess", "{\"RoomID\":\"R201\",\"Mess\":\"M05\"}");
          break;
        case '6':
          webSocket.emit("esp-send-mess", "{\"RoomID\":\"R201\",\"Mess\":\"M06\"}");
          break;
        case '7':
          webSocket.emit("esp-send-mess", "{\"RoomID\":\"R201\",\"Mess\":\"M07\"}");
          break;
        case '8':
          webSocket.emit("esp-send-mess", "{\"RoomID\":\"R201\",\"Mess\":\"M08\"}");
          break;
      }
    } else if (Serial.available()) {
      while (Serial.available()) Serial.read();
      lcd.clear();

      lcd.setCursor(0, 0);
      // print message
      lcd.print("Load card!");
    }
    webSocket.loop();
    // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Detected card:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
}
