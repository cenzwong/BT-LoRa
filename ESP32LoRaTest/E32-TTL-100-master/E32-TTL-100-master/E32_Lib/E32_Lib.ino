//Arduino Nano

#include "E32.h"
//#include "config.h"

E32 myE32; // to be config in config.h
//target device address and Channel
E32Device deviceCCCC = {0xCCCC, 0x17};

uint8_t dataA[] = "Hello World2";

uint8_t dataB[] = "Fuck the Police!!!!!";

E32Data inFrame = {};

// HM10 is used
#define BT_RXD A4
#define BT_TXD A5
//The library is included in E32.h
SoftwareSerial SerialBT(BT_TXD, BT_RXD);  // RX, TX swap intensionally

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void setup() {
  // put your setup code here, to run once:s
  Serial.begin(115200);
  Serial.write(0xF0);
  myE32.begin();
  SerialBT.begin(9600); //must be after myE32.begin()
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
      char inChar = Serial.read();
      if(inChar == 'a'){
        SerialBT.end();
        myE32.sendMessage(deviceCCCC, dataA);
        SerialBT.begin(9600);
      }
      if(inChar == 'b'){
        //myE32.sendMessage(deviceCCCC, dataB);
        }
        if(inChar == 'r'){
          //do reset()
          resetFunc();
          }
        if(inChar == 'z'){
          //do reset()
          Serial.print('z');
          SerialBT.end();
          myE32.returnConfig();
          SerialBT.begin(9600);
          }
    }

  if(myE32.getAUX() == LOW){
    SerialBT.end();
    delay(3); //delay 3 ms
    myE32.waitReceiveComplete();
    inFrame = myE32.getMessage();      
    SerialBT.begin(9600);
    for(uint8_t i = 0 ; i < inFrame.dataLen; i++){
      SerialBT.write(inFrame.dataPtr[i]);     
      Serial.write(inFrame.dataPtr[i]);
      delay(5);
      }
  }
  

   static char BTinData[300] = {0};
   if(BT_receiveComplete(BTinData)){
      SerialBT.end();
      Serial.write(BTinData);
      delay(100);
      myE32.sendMessage(deviceCCCC, BTinData);
      SerialBT.begin(9600);
    }
    
}

bool BT_receiveComplete(char* inD){
    if(SerialBT.available()>0){
        char inChar = SerialBT.read();
        //Serial.write(inChar);
    
        static uint8_t i = 0;
        inD[i++] = inChar;
        inD[i] = '\0';
        if(inChar == '\n'){
          //End of Message  
          inD[i++] = '\0';
          i = 0;      
          return true;
        }      
      }
    return false;
}
