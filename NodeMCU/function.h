#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino_JSON.h>
#include <SoftwareSerial.h>

SoftwareSerial ss(D5,D1);
WiFiClient client;

#define Led1  6
#define Led2  7



const char* ssid     = "Van Hao";
const char* password = "17520451";
const char* host = "192.168.137.1";
//WiFiClient client;

String IDString="";
/* change your link here **********************************/
const char* Send_Check_ID="http://192.168.137.1:80/IOT_Door/CheckID.php";
const char* Send_Request_url= "http://192.168.137.1:80/IOT_Door/Request.php";



void INIT(){
  Serial.begin(9600);
  ss.begin(9600);
  delay(100);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  Serial.println("connected server");


  
}

void Check_ID(String ID){
  HTTPClient http;
  http.begin(Send_Check_ID);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode=http.POST(String ("ID=")+ ID); 
  String payload=http.getString();
  http.end();   
  // if successful
  if(httpCode==200){
    Serial.println(payload);
  } 
  DynamicJsonBuffer jsonBuffer;
  JsonArray &root = jsonBuffer.parseArray(payload);
  if (!root.success()) {
    Serial.println("parseArray() failed");
  } else {
    int A=root.size(); //root[index]["Obj"]
    //Serial.println(A);
    
  }
  //int check= root[0]["Result"];
  //Serial.print(check);
  if(root[0]["Result"]==1){
    ss.print("success.");
    Serial.println("Success");
  }
  else{
    ss.print("fail.");
    Serial.println("Fail");
  }
}

void Send_Request(String Request){
  String data = "Request=" + Request + "&IDPhong=" +"A3.1";
  Serial.println(data);
  HTTPClient http;
  http.begin(Send_Request_url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode=http.POST(data); 
  String payload=http.getString();
  http.end();
  if(httpCode==200){
    Serial.println("Request success");
  }   
}
