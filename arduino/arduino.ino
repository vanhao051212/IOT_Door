#include <SoftwareSerial.h>

SoftwareSerial mySerial(8,9);   // RX, TX

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

void(* resetFunc) (void) = 0;// reset function

unsigned long time_out; //time out reset

void setup() {
  Serial.begin(9600);
  mySerial.begin(115200);
  
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

  time_out = millis();
}

void loop() {
  if (millis() - time_out > 300000){
      delay(1000);
      resetFunc();
  }
  if (!digitalRead(Status_1))
  {
    while (!digitalRead(Status_1));
    //Serial.println("Status 1");
    mySerial.write('1');
    digitalWrite(Led_6, LOW);
    delay(Time_delay);
    digitalWrite(Led_6, HIGH);
  } 
  if (!digitalRead(Status_2))
  {
    while (!digitalRead(Status_2));
    mySerial.write('2');
    //Serial.println("Status 2");
    digitalWrite(Led_6, LOW);
    delay(Time_delay);
    digitalWrite(Led_6, HIGH);
  }
  if (!digitalRead(Status_3))
  {
    while (!digitalRead(Status_3));
    mySerial.write('3');
    //Serial.println("Status 3");
    digitalWrite(Led_6, LOW);
    delay(Time_delay);
    digitalWrite(Led_6, HIGH);
  }
  if (!digitalRead(Status_4))
  {
    while (!digitalRead(Status_4));
    mySerial.write('4');
    //Serial.println("Status 4");
    digitalWrite(Led_6, LOW);
    delay(Time_delay);
    digitalWrite(Led_6, HIGH);
  }
    if (!digitalRead(Status_5))
  {
    while (!digitalRead(Status_5));
    mySerial.write('5');
    //Serial.println("Status 5");
    digitalWrite(Led_6, LOW);
    delay(Time_delay);
    digitalWrite(Led_6, HIGH);
  }
    if (!digitalRead(Status_6))
  {
    while (!digitalRead(Status_6));
    mySerial.write('6');
    //Serial.println("Status 6");
    digitalWrite(Led_6, LOW);
    delay(Time_delay);
    digitalWrite(Led_6, HIGH);
  }
  if (!digitalRead(Status_7))
  {
    while (!digitalRead(Status_7));
    mySerial.write('7');
    //Serial.println("Status 7");
    digitalWrite(Led_6, LOW);
    delay(Time_delay);
    digitalWrite(Led_6, HIGH);
  }
  if (!digitalRead(Status_8))
  {
    while (!digitalRead(Status_8));
    mySerial.write('8');
    //Serial.println("Status 8");
    digitalWrite(Led_6, LOW);
    delay(Time_delay);
    digitalWrite(Led_6, HIGH);
  }
}
