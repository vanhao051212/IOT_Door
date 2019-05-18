#include "function.h"
#include <TimerOne.h>

float beginTime = 0;
float endTime = 0;

void resetTimer();

void setup()
{
  INIT();

  Timer1.initialize(4000000); // set a timer of length 4000000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt(ISR_Timer); // attach the service routine here
}
void loop()
{
  Check_button();
  Check_projector();
  Serial.println("LOOP");
}
void ISR_Timer()
{
  Serial.println("Timer");
  if (falt_projector)
  {
    Send_uart(char(5));
    Serial.println("Mat may chieu");
  }
}
