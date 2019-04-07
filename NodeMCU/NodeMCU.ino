#include"function.h"


void setup() {
  // put your setup code here, to run once:
  INIT();
  Time_out = millis();
}

void loop() {
  // reset program every 5' (300000ms)
  if (millis() - Time_out > 300000){
    Serial.print("Reset program!!!");
    delay(1000);
    resetFunc();
  }

  if(ss.available()>0){
    char character = char(ss.read());
    
    // if receive id
    if(character != '.' && character != ','){
      IDString += character;   
    }
    if(character == '.'){
      Serial.println("Checking id");
      IDString.trim();  
      Check_ID(IDString);       
      IDString="";
    }
    if(character == ','){
      IDString.trim();  
      Send_Request(IDString);       
      IDString="";
    }
  }
      

}
