#include <SoftwareSerial.h>

SoftwareSerial ss(8,9);   // RX, TX

#define Status_1 2
#define Status_2 3
#define Status_3 4
#define Status_4 5

#define Led_1 6
#define Led_2 7

#define G1 10
#define G2 11

bool falt_projector = false;

unsigned long timeSendInterval;

unsigned long timeOut;

void INIT();
void Send_uart(char value);
void Check_button();
void Check_projector();
void Turn_Led(uint8_t led, uint8_t state);
void Check_Led();

bool firstSend = true;

void(* resetFunc) (void) = 0;

void setup()
{
  INIT();
  timeSendInterval=millis();
  timeOut=millis();
}
void loop()
{
  Check_button();
  
  Check_projector();
  
  if(falt_projector) {
    timeOut=millis();
    if(millis() - timeSendInterval > 60000) {
      timeSendInterval=millis();
      Send_uart('5');
      Serial.println("Alert");
    }
  }
  Check_Led();
  if(millis()-timeOut > 300000) {
    resetFunc();
  }
}

/*======================================================================================================*/
void INIT()
{
  Serial.begin(9600);
  ss.begin(9600);

  pinMode(Status_1, INPUT_PULLUP);
  pinMode(Status_2, INPUT_PULLUP);
  pinMode(Status_3, INPUT_PULLUP);
  pinMode(Status_4, INPUT_PULLUP);

  pinMode(Led_1, OUTPUT);
  pinMode(Led_2, OUTPUT);

  digitalWrite(Led_1, HIGH);
  digitalWrite(Led_2, HIGH);
  
  pinMode(G1, INPUT_PULLUP);
  pinMode(G2, INPUT_PULLUP);
  delay(500);
}

void Send_uart(char value)
{
  ss.write(value);
}

void Check_button()
{
  if (!digitalRead(Status_1))
  {
    delay(50);
    if(!digitalRead(Status_1)) {
      Send_uart('1');
      delay(2000);
    }
  } 

  if (!digitalRead(Status_2))
  {
    delay(50);
    if(!digitalRead(Status_2)) {
      Send_uart('2');
      delay(2000);
    }
  }

  if (!digitalRead(Status_3))
  {
    delay(50);
    if(!digitalRead(Status_3)) {
      Send_uart('3');
      delay(2000);
    }
  }

  if (!digitalRead(Status_4))
  {
    delay(50);
    if(!digitalRead(Status_4)) {
      Send_uart('4');
      delay(2000);
    }
  }
}
void Check_projector() {
  if (digitalRead(G1) || !digitalRead(G2)) {
    delay(5);
    if (digitalRead(G1) || !digitalRead(G2)) {
      falt_projector = true;
    }
  }
  if (!digitalRead(G1) && digitalRead(G2)) {
    falt_projector = false;
    firstSend=true;
  }
  if(firstSend && falt_projector) {
    firstSend = false;
    Serial.println("Alert");
    Send_uart('5');
    timeSendInterval=millis();
  }
}
void Turn_Led(uint8_t led, uint8_t state) {
  digitalWrite(led, !state);
}
void Check_Led() {
  if(ss.available()>0) {
    char val = ss.read();
    if(val=='^') {
      while(ss.available()<=0) {};
      val = ss.read();
      switch(val) {
        case '0':
          Turn_Led(Led_1, LOW);
          break;
        case '1':
          Turn_Led(Led_1, HIGH);
          break;
        case '2':
          Turn_Led(Led_2, LOW);
          break;
        case '3':
          Turn_Led(Led_2, HIGH);
          break;
      }
    }
  }
}
