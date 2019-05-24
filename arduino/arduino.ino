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

void(* resetFunc) (void) = 0;// reset function

void INIT();
void Send_uart(char value);
void Check_button();
void Check_projector();

bool firstSend = true;

void setup()
{
  INIT();
  timeSendInterval=millis();
}
void loop()
{
  Check_button();
  
  Check_projector();
  
  if(falt_projector) {
    if(millis() - timeSendInterval > 5000) {
      timeSendInterval=millis();
      Send_uart('5');
      Serial.println("Alert");
    }
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
      Send_uart('6');
      delay(2000);
    }
  }
}
void Check_projector() {
  if (digitalRead(G1) || !digitalRead(G2)) {
    delay(1000);
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
