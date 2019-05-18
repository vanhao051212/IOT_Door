#include <SoftwareSerial.h>

SoftwareSerial ss(8,9);   // RX, TX

#define Status_1 2
#define Status_2 3
#define Status_3 4
#define Status_4 5

#define Led_1 6
#define Led_2 7

#define Pull_up 10
#define Pull_down 11

bool falt_projector = false;

void(* resetFunc) (void) = 0;// reset function

void INIT();
void Send_uart(int value);
void Check_button();
void Check_projector();

/*======================================================================================================*/
void INIT()
{
  Serial.begin(9600);
  ss.begin(115200);

  pinMode(Status_1, INPUT);
  pinMode(Status_2, INPUT);
  pinMode(Status_3, INPUT);
  pinMode(Status_4, INPUT);

  pinMode(Led_1, OUTPUT);
  pinMode(Led_2, OUTPUT);

  digitalWrite(Led_1, HIGH);
  digitalWrite(Led_2, HIGH);
  
  pinMode(Pull_up, INPUT_PULLUP);
  pinMode(Pull_down, INPUT);
  digitalWrite(Pull_down, LOW);
}

void Send_uart(int value)
{
  ss.write(value);
}

void Check_button()
{
  if (!digitalRead(Status_1))
  {
    while (!digitalRead(Status_1));
    Send_uart(char(1));
  } 

  if (!digitalRead(Status_2))
  {
    while (!digitalRead(Status_2));
    Send_uart(char(2));
  }

  if (!digitalRead(Status_3))
  {
    while (!digitalRead(Status_3));
    Send_uart(char(3));
  }

  if (!digitalRead(Status_4))
  {
    while (!digitalRead(Status_4));
    Send_uart(char(6));
  }
}

void Check_projector()
{
  if (digitalRead(Pull_up))
  {
    falt_projector = true;
  } else {
    falt_projector = false;
  }

  if (!digitalRead(Pull_down))
  {
    falt_projector = true;
  } else {
    falt_projector = false;
  }
}
