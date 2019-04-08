#include <SoftwareSerial.h>

SoftwareSerial mySerial(9,8);   // RX, TX

#define Status_1 2
#define Status_2 3
#define Status_3 4
#define Status_4 5
#define Status_5 6
#define Status_6 7
#define Status_7 10
#define Status_8 11

#define Led_1 A0
#define Led_2 A1
#define Led_3 A2
#define Led_4 A3
#define Led_5 A4
#define Led_6 A5
#define Led_7 A6
#define Led_8 A7

#define Time_delay 1000

void setup() {
  Serial.begin(9600);
  mySerial.begin(96000);
  
  pinMode(Status_1, INPUT);
  pinMode(Status_2, INPUT);
  pinMode(Status_3, INPUT);
  pinMode(Status_4, INPUT);
  pinMode(Status_5, INPUT);
  pinMode(Status_6, INPUT);
  pinMode(Status_7, INPUT);
  pinMode(Status_8, INPUT);

  pinMode(Led_1, OUTPUT);
  pinMode(Led_2, OUTPUT);
  pinMode(Led_3, OUTPUT);
  pinMode(Led_4, OUTPUT);
  pinMode(Led_5, OUTPUT);
  pinMode(Led_6, OUTPUT);
  pinMode(Led_7, OUTPUT);
  pinMode(Led_8, OUTPUT);

  digitalWrite(Led_1, HIGH);
  digitalWrite(Led_2, HIGH);
  digitalWrite(Led_3, HIGH);
  digitalWrite(Led_4, HIGH);
  digitalWrite(Led_5, HIGH);
  digitalWrite(Led_6, HIGH);
  digitalWrite(Led_7, HIGH);
  digitalWrite(Led_8, HIGH);
}

void loop() {
  if (!digitalRead(Status_1))
  {
    while (!digitalRead(Status_1));
    Serial.println("Status 1");
    digitalWrite(Led_1, LOW);
    delay(Time_delay);
    digitalWrite(Led_1, HIGH);
    mySerial.write(1);
  } 
  if (!digitalRead(Status_2))
  {
    while (!digitalRead(Status_2));
    Serial.println("Status 2");
    digitalWrite(Led_2, LOW);
    delay(Time_delay);
    digitalWrite(Led_2, HIGH);
    mySerial.write(2);
  }
  if (!digitalRead(Status_3))
  {
    while (!digitalRead(Status_3));
    Serial.println("Status 3");
    digitalWrite(Led_3, LOW);
    delay(Time_delay);
    digitalWrite(Led_3, LOW);
    mySerial.write(3);
  }
  if (!digitalRead(Status_4))
  {
    while (!digitalRead(Status_4));
    Serial.println("Status 4");
    digitalWrite(Led_4, LOW);
    delay(Time_delay);
    digitalWrite(Led_4, LOW);
    mySerial.write(4);
  }
    if (!digitalRead(Status_5))
  {
    while (!digitalRead(Status_5));
    Serial.println("Status 5");
    digitalWrite(Led_5, LOW);
    delay(Time_delay);
    digitalWrite(Led_5, LOW);
    mySerial.write(5);
  }
    if (!digitalRead(Status_6))
  {
    while (!digitalRead(Status_6));
    Serial.println("Status 6");
    digitalWrite(Led_6, LOW);
    delay(Time_delay);
    digitalWrite(Led_6, LOW);
    mySerial.write(6);
  }
  if (!digitalRead(Status_7))
  {
    while (!digitalRead(Status_7));
    Serial.println("Status 7");
    digitalWrite(Led_7, LOW);
    delay(Time_delay);
    digitalWrite(Led_7, LOW);
    mySerial.write(7);
  }
  if (!digitalRead(Status_8))
  {
    while (!digitalRead(Status_8));
    Serial.println("Status 8");
    digitalWrite(Led_8, LOW);
    delay(Time_delay);
    digitalWrite(Led_8, LOW);
    mySerial.write(8);
  }
}
