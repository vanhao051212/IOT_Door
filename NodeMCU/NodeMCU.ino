//==============================================================================================
//                                    declare
//==============================================================================================

#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

#define AUTO_RESET

#include <MFRC522.h>
#define RST_PIN 15 // RST-PIN for RC522 - RFID - SPI - Modul GPIO15 
#define SS_PIN  2  // SDA-PIN for RC522 - RFID - SPI - Modul GPIO2
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

#include <SocketIoClient.h>

SocketIoClient webSocket;

//host để kiểm tra id của thẻ rf
const char* Send_Check_ID="http://192.168.20.135:8000/checkRFID";
const char* Send_Cmd="http://192.168.20.135:8000/receivedCmd";

//host và port để esp kết nối socket0
const char* Host_Socket = "192.168.20.135";
unsigned int Port_Socket = 8000;


const char* ssid = "UIT-hidden";
const char* pwdWifi = "Uit201913032019";

char charSendSocket[200];

void convertStringToChar(String str);
void processCharSendSocket(String RoomID, String Mess, String CardID);

bool sendNotice(String CardID, String command);

//biến kiểm tra xem node đã join room trên server hay chưa
bool joinedRoom = false;

//hàm handle sự kiện join room thành công do server gửi về
void join_success(const char * payload, size_t length);


//hàm handle tin nhắn từ server gửi về cho node mcu
void handleMess(const char * payload, size_t length);

//hàm handle tin nhắn đã được gửi tới server, do server gửi về
void ackMess(const char * payload, size_t length);

//hàm này cho phép gửi tin nhắn từ esp lên server
//đủ điều kiện esp đã join room và xác nhận thẻ thành công thì mới được gửi
bool ableSendMess = false;

String strID="";

// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize);

bool readCardFunc(void);

//tránh tình trạng print dòng ready to use quá nhiều lần trên lcd
bool printedReady = false;

//thời gian để gửi event yêu cầu join room lên server
unsigned long time_join_room;

#ifdef AUTO_RESET
//thời gian reset lại
unsigned long time_out; //time out reset
#endif

//turn led
void Turn_Led_1(uint8_t state);
void Turn_Led_2(uint8_t state);

//check request
void Execute_Request(char CMD);

//==============================================================================================
//                                    setup
//==============================================================================================

void setup() {
    //initialize LCD
    lcd.init();
    //turn on LCD backlight                
    lcd.backlight();
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Khoi dong....");
    
    Serial.begin(9600);

    SPI.begin();           // Init SPI bus
    mfrc522.PCD_Init();    // Init MFRC522 

    Serial.println();
    Serial.println();
    Serial.println();

    #ifdef AUTO_RESET
    time_out = millis();
    #endif
    
    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, pwdWifi);

    while (WiFi.status() != WL_CONNECTED)
    {
      #ifdef AUTO_RESET
      if (millis() - time_out > 30000){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Reset tu dong");
        ESP.restart();
      }
      #endif
      delay(500);
      Serial.print(".");
    }
  
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    webSocket.on("join-success", join_success);
    webSocket.on("server-send-mess-to-esp", handleMess);
    webSocket.on("server-ack", ackMess);
    webSocket.begin(Host_Socket, Port_Socket, "/socket.io/?transport=websocket");
    // use HTTP Basic Authorization this is optional remove if not needed
    // webSocket.setAuthorization("username", "password");
    webSocket.emit("esp-send-join-room", "{\"RoomID\":\"R004\"}");
    time_join_room = millis();
    #ifdef AUTO_RESET
    time_out = millis();
    #endif
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Diem danh SS!");
    Turn_Led_1(HIGH);
    Turn_Led_2(LOW);
}

//==============================================================================================
//                                    loop
//==============================================================================================

void loop() {
    #ifdef AUTO_RESET
    if (millis() - time_out > 600000){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Reset tu dong");
      ESP.restart();
    }
    #endif
    if (!joinedRoom && (millis()-time_join_room>5000)) { //neu chua join room in server
      time_join_room = millis();
      webSocket.emit("esp-send-join-room", "{\"RoomID\":\"R004\"}");
    }
    
    if (!printedReady && joinedRoom) {
      printedReady = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("DD + Menu SS!");
    }

    if (readCardFunc()) {
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("  Da nhan the");
      Turn_Led_1(LOW);
      Turn_Led_2(LOW);
      if(sendNotice(strID, "M06")) {
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("DD Thanh Cong :)");
        delay(500);
        Turn_Led_1(HIGH);
        Turn_Led_2(LOW);
        delay(500);
        Turn_Led_1(LOW);
        Turn_Led_2(LOW);
        delay(500);
        Turn_Led_1(HIGH);
        Turn_Led_2(LOW);
      } else {
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("DD Loi :<");
        delay(500);
        Turn_Led_1(LOW);
        Turn_Led_2(HIGH);
        delay(500);
        Turn_Led_1(LOW);
        Turn_Led_2(LOW);
        delay(500);
        Turn_Led_1(LOW);
        Turn_Led_2(HIGH);
        delay(500);
        Turn_Led_1(HIGH);
        Turn_Led_2(LOW);
      }
      printedReady=false;
    }
    if(Serial.available() > 0) {
      char val = Serial.read();
      if(val == '1' || val == '2' || val == '3' || val == '4' || val == '5') {
        Execute_Request(val);
        printedReady=false;
      }
    }
    if(!webSocket.StatusConnectSocket) {
      webSocket.disconnect();
      delay(500);
      joinedRoom=false;
      webSocket.begin(Host_Socket, Port_Socket, "/socket.io/?transport=websocket");
      webSocket.StatusConnectSocket=true;
      delay(500);
    }
    webSocket.loop();
}

//==============================================================================================
//                                    define function
//==============================================================================================
void convertStringToChar(String str) {
  for(int i=0; i<200; i++) {
    charSendSocket[i] = '\0';
  }
  for(int i=0; i<str.length(); i++) {
    charSendSocket[i] = str[i];
  }
}

void processCharSendSocket(String RoomID, String Mess, String CardID) {
  DynamicJsonBuffer jsonBuffer;

  String stringJson = "{\"RoomID\":\"R004\",\"MessID\":\"M01\",\"CardID\":\"ABCDABCD\"}";
  JsonObject& root = jsonBuffer.parseObject(stringJson);

  root["RoomID"] = RoomID;
  root["MessID"] = Mess;
  root["CardID"] = CardID;
  stringJson = "";
  root.printTo(stringJson);
  convertStringToChar(stringJson);
}

bool sendNotice(String CardID, String command){
  HTTPClient http;
  http.begin(Send_Cmd);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode=http.POST(String("RoomID=R004&") + String("CardID=")+ CardID + '&' + String("CMD=") + command); 
  String payload=http.getString();
  http.end();   
  // if successful
  if(httpCode==200){
    if (payload == "OK") {
      return true;
    }
  }
  return false;
}

void join_success(const char * payload, size_t length) {
  joinedRoom = true;
  Serial.printf("join success");
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

void ackMess(const char * payload, size_t length) {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("Yc thanh cong");
  delay(1500);
  printedReady=false;
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  strID="";
  for (byte i = 0; i < bufferSize; i++) {
    String valHex = String(buffer[i],HEX);
    if(valHex.length() == 1) {
      valHex = String(0)+valHex;
    }
    strID+=valHex;
  }
  strID.toUpperCase();
  Serial.println(strID);
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
void Turn_Led_1(uint8_t state) {
  if(state == HIGH) {
    Serial.write("^1");
  } else {
    Serial.write("^0");
  }
}
void Turn_Led_2(uint8_t state) {
  if(state == HIGH) {
    Serial.write("^3");
  } else {
    Serial.write("^2");
  }
}
void Execute_Request(char CMD) {
  unsigned long timeCheckCard = millis();
  if(CMD == '5') {
    webSocket.emit("esp-send-mess", "{\"RoomID\":\"R004\",\"MessID\":\"M05\"}");
    if(!sendNotice("", "M05")) {
      sendNotice("", "M05");
    }
  }
  if (joinedRoom) { // neu uart den + xac thuc + da join room
    switch(CMD) {
      case '1':
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("Remote");
        timeCheckCard = millis();
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print("Quet the gui");
        while (millis() - timeCheckCard < 4000) {
          if(Serial.available()>0) {
            char receiveChar = Serial.read();
            if(receiveChar == '1' || receiveChar == '2' || receiveChar == '3' || receiveChar == '4' || receiveChar == '5') {
              Execute_Request(receiveChar);
              return;
            }
          }
          if(readCardFunc()) {
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("Dang gui...");
            processCharSendSocket("R004", "M01", strID);
            webSocket.emit("esp-send-mess", charSendSocket);
            break;
          }
        }
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("Ket thuc!");
        delay(1000);
        break;
      case '2':
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("Micro");
        timeCheckCard = millis();
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print("Quet the gui");
        while (millis() - timeCheckCard < 4000) {
          if(Serial.available()>0) {
            char receiveChar = Serial.read();
            if(receiveChar == '1' || receiveChar == '2' || receiveChar == '3' || receiveChar == '4' || receiveChar == '5') {
              Execute_Request(receiveChar);
              return;
            }
          }
          if(readCardFunc()) {
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("Dang gui...");
            processCharSendSocket("R004", "M02", strID);
            webSocket.emit("esp-send-mess", charSendSocket);
            break;
          }
        }
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("Ket thuc!");
        delay(1000);
        break;
      case '3':
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("HDMI");
        timeCheckCard = millis();
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print("Quet the gui");
        while (millis() - timeCheckCard < 4000) {
          if(Serial.available()>0) {
            char receiveChar = Serial.read();
            if(receiveChar == '1' || receiveChar == '2' || receiveChar == '3' || receiveChar == '4' || receiveChar == '5') {
              Execute_Request(receiveChar);
              return;
            }
          }
          if(readCardFunc()) {
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("Dang gui...");
            processCharSendSocket("R004", "M03", strID);
            webSocket.emit("esp-send-mess", charSendSocket);
            break;
          }
        }
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("Ket thuc!");
        delay(1000);
        break;
      case '4':
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("Truc trac KT");
        timeCheckCard = millis();
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print("Quet the gui");
        while (millis() - timeCheckCard < 4000) {
          if(Serial.available()>0) {
            char receiveChar = Serial.read();
            if(receiveChar == '1' || receiveChar == '2' || receiveChar == '3' || receiveChar == '4' || receiveChar == '5') {
              Execute_Request(receiveChar);
              return;
            }
          }
          if(readCardFunc()) {
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("Dang gui...");
            processCharSendSocket("R004", "M04", strID);
            webSocket.emit("esp-send-mess", charSendSocket);
            break;
          }
        }
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("Ket thuc!");
        delay(1000);
        break;
    }
  }
}
