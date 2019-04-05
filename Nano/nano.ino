#include"function.h"


void setup() {
  // put your setup code here, to run once:
  INIT();
}

void loop() {
  // put your main code here, to run repeatedly:

  /** READ RFID ******************************************/
  if (rfid.isCard())
    {
      if (rfid.readCardSerial())
        {
          /* Reading card */
          Serial.println(" ");
          Serial.println("Card found");
          Serial.println("Cardnumber:");
          for (int i = 0; i < 5; i++)
          {     
            Serial.print(rfid.serNum[i]);
            Serial.print(" ");
            reading_card[i] = rfid.serNum[i];
          }
          Serial.println();
          Time_Out=millis();
       }

    }
    rfid.halt();
  /*******************************************************************/
  if(millis() - Time_Out <= 10000 ){
    Check_Button();
  }  
}
