#include "function.h"


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
          
       }
      Send_ID(reading_card);      
    }
    rfid.halt();
  /*******************************************************************/
  /* Check respone */
  if(ss.available()>0){

    char character = char(ss.read());
    
    // if receive id
    if(character != '.' ){
      Check_Result += character;   
    }
    if(character == '.'){
      Check_Result.trim();  
      if(Check_Result=="success"){
        
        digitalWrite(Led1,0);
        Time_Out=millis(); 
            
      }
      else{
         digitalWrite(Led1,1);
      }
      Check_Result="";
    }
  }
  if(millis() - Time_Out <= 10000 ){
    Check_Button();
  }
  else{
    digitalWrite(Led1,1);
  }


}
