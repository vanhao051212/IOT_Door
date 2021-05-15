#include "EthernetPostGet.h"
#include <SoftwareSerial.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClientClass Eclient;
SoftwareSerial software_serial(5, 6); // RX, TX

//char server[] = "192.168.1.107";
char server[] = "192.168.20.135";
int port = 8000;

String dataPost = "";
long double beginTimeReset = 0;

void(* resetFunc) (void) = 0; //cài đặt hàm reset

void setup() {
  SPI.begin();
  Serial.begin(9600);
  software_serial.begin(9600);
  Serial.println(F("Connecting..."));
  Eclient.MacInitial(mac);
  Eclient.EthernetConnectServer(server, port);
  Serial.println(F("Setup is complete"));
  software_serial.print("START");
  beginTimeReset = millis();
}

void loop() {
  if (millis() - beginTimeReset > 1800000){     // 30p se reset 1 lan
    software_serial.print("START");
    delay(1000);
    resetFunc();
  }
  
  while (software_serial.available()) {
    dataPost = "";
    while (software_serial.available()) {
      char c = software_serial.read();
      delay(100);
      dataPost += c;
    }
    Serial.print(F("String reviced: "));
    Serial.println(dataPost);
  }

  if (Eclient.Connected()) {
    if (dataPost != "") {
      dataPost = "RoomID=R016&CardID=" + dataPost + "&CMD=M06";
      Serial.print(F("Du lieu dataPost: "));
      Serial.println(dataPost);
      String respone = Eclient.PostDataFromArduino(server, port, dataPost);
//      String respone = Eclient.GetDataFromServer(server, port);
      if (respone != "") {
        Serial.print(F("\nRespone: "));
        Serial.println(respone);
        software_serial.print(respone);
      } else {
        Serial.println(F("\nRespone rong"));
        software_serial.print("FAIL");
      }
      Serial.println(F("--------------------------------------"));
      dataPost = "";
      Eclient.Disconnect();
    }
  } else {
    Serial.println(F("Disconnect"));
    Eclient.EthernetConnectServer(server, port);
  }
}
