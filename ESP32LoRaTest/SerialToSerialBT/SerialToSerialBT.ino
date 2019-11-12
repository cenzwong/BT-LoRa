// my board pinout
// https://www.flickr.com/photos/jgustavoam/40089095211

//  note
// the pin 16 and 17 is the UART2, Serial2
// pinMode(5,OUTPUT);
// digitalWrite(5,LOW);
// digitalWrite(5,HIGH);
//
// Test OK


//#define E32_M0  18
//#define E32_M1  5
//#define E32_RXD 17
//#define E32_TXD 16
//#define E32_AUX 4
//
// ======= ESP32 BTSerial =======
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

#include "ESP32_E32.h"
E32 myE32; // to be config in config.h
//target device address and Channel
E32Device deviceABCD = {0xABCD, 0x17};

char dataA[] = "abcde";

E32Data inFrame = {};

void(* resetFunc) (void) = 0; //declare reset function @ address 0



void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("Program started");
  
  myE32.begin();
}

void loop() {
  if(Serial.available()>0){
      char inChar = Serial.read();
      if(inChar == 'a'){
        myE32.sendMessage(deviceABCD, dataA);
      }
     if(inChar == 'b'){
        myE32.sendMessage(deviceABCD, "Hello my name is Cenz!");
        }
     if(inChar == 'r'){
          //do reset()
          resetFunc();
          }
      if(inChar == 'z'){
          //do reset()
          Serial.print('z');
          myE32.returnConfig();
          }
    }


    if(myE32.receiveComplete()){    
      inFrame = myE32.getMessage();      
      for(uint8_t i = 0 ; i < strlen(inFrame.dataPtr); i++){
        SerialBT.write(inFrame.dataPtr[i]);     
        Serial.write(inFrame.dataPtr[i]);
        delay(10);
      }

  }
  
   static char BTinData[300] = {0};
   if(BT_receiveComplete(BTinData)){
      Serial.write(BTinData);
      myE32.sendMessage(deviceABCD, BTinData);
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
