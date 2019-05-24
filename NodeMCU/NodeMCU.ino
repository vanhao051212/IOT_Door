//==============================================================================================
//                                    declare
//==============================================================================================

#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

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

//thời gian reset lại
unsigned long time_out; //time out reset

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
    
    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, pwdWifi);

    while (WiFi.status() != WL_CONNECTED)
    {
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
    time_join_room = millis();
    time_out = millis();
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Diem danh SS!");
}

//==============================================================================================
//                                    loop
//==============================================================================================

void loop() {
    if (millis() - time_out > 300000){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Reset tu dong");
      ESP.restart();
    }
    if (!joinedRoom && (millis()-time_join_room>5000)) { //neu chua join room in server
      time_join_room = millis();
      webSocket.emit("esp-send-join-room", "{\"RoomID\":\"R101\"}");
    }
    
    if (!printedReady && joinedRoom) {
      printedReady = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("San sang dung!");
    }
    if(Serial.available() > 0) {
      unsigned long timeCheckCard = millis();
      char val = Serial.read();
      if(val == '5') {
        webSocket.emit("esp-send-mess", "{\"RoomID\":\"R101\",\"MessID\":\"M05\"}");
        if(sendNotice("", "M05")) {
          lcd.setCursor(0, 0);
          lcd.print("                ");
          lcd.setCursor(0, 0);
          lcd.print("Thanh cong!");
        } else {
          lcd.setCursor(0, 0);
          lcd.print("                ");
          lcd.setCursor(0, 0);
          lcd.print("That bai thu lai");
          if(sendNotice("", "M05")) {
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("Thanh cong!");
          }
        }
      }
      if(val == '6') {
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("Quet the di!");
        timeCheckCard = millis();
        while (millis() - timeCheckCard < 4000) {
          if(readCardFunc()) {
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("Da nhan the!");
            if(sendNotice(strID, "M06")) {
              lcd.setCursor(0, 0);
              lcd.print("                ");
              lcd.setCursor(0, 0);
              lcd.print("Everything OK :)");
            } else {
              lcd.setCursor(0, 0);
              lcd.print("                ");
              lcd.setCursor(0, 0);
              lcd.print("Fail :(");
            }
            break;
          }
        }
      }
      if (joinedRoom) { // neu uart den + xac thuc + da join room
        switch(val) {
          case '1':
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("Quet the gui!");
            timeCheckCard = millis();
            while (millis() - timeCheckCard < 4000) {
              if(readCardFunc()) {
                lcd.setCursor(0, 0);
                lcd.print("                ");
                lcd.setCursor(0, 0);
                lcd.print("Dang gui...");
                processCharSendSocket("R101", "M01", strID);
                webSocket.emit("esp-send-mess", charSendSocket);
                break;
              }
            } 
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("Ket thuc!");
            break;
          case '2':
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("Quet the gui!");
            timeCheckCard = millis();
            while (millis() - timeCheckCard < 4000) {
              if(readCardFunc()) {
                lcd.setCursor(0, 0);
                lcd.print("                ");
                lcd.setCursor(0, 0);
                lcd.print("Dang gui...");
                processCharSendSocket("R101", "M02", strID);
                webSocket.emit("esp-send-mess", charSendSocket);
                break;
              }
            }
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("Ket thuc!");
            break;
          case '3':
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("Quet the gui!");
            timeCheckCard = millis();
            while (millis() - timeCheckCard < 4000) {
              if(readCardFunc()) {
                lcd.setCursor(0, 0);
                lcd.print("                ");
                lcd.setCursor(0, 0);
                lcd.print("Dang gui...");
                processCharSendSocket("R101", "M03", strID);
                webSocket.emit("esp-send-mess", charSendSocket);
                break;
              }
            }
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("Ket thuc!");
            break;
          case '4':
            timeCheckCard = millis();
            while (millis() - timeCheckCard < 4000) {
              if(readCardFunc()) {
                lcd.setCursor(0, 0);
                lcd.print("                ");
                lcd.setCursor(0, 0);
                lcd.print("Sending...");
                processCharSendSocket("R101", "M04", strID);
                webSocket.emit("esp-send-mess", charSendSocket);
                break;
              }
            }
            lcd.setCursor(0, 0);
            lcd.print("                ");
            lcd.setCursor(0, 0);
            lcd.print("Terminate!");
            break;
        }
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

  String stringJson = "{\"RoomID\":\"R101\",\"MessID\":\"M01\",\"CardID\":\"ABCDABCD\"}";
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
  int httpCode=http.POST(String("RoomID=R101&") + String("CardID=")+ CardID + '&' + String("CMD=") + command); 
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
  lcd.print("Gui thanh cong");
  delay(1500);
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("San sang dung!");
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
