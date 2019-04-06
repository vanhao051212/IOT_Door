#include"function.h"


void setup() {
  // put your setup code here, to run once:
  INIT();

}

void loop() {
  // put your main code here, to run repeatedly:
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
