#include "ESP32_E32.h"
#include "config.h"

E32::E32(){
  pinMode(E32_M0, OUTPUT);
  pinMode(E32_M1, OUTPUT);
  pinMode(E32_AUX, INPUT);
  while(!digitalRead(E32_AUX)); //This wait until it become high
}

void E32::begin(void){
  E32Serial.begin(9600);
  delay(10);
  changeMode(Mode_Sleep); 
  delay(10);
  setConfig();
  delay(10);
  changeMode(Mode_Normal);
  }

void E32::changeMode(uint8_t mode){
  digitalWrite(E32_M0,(mode & 0b01));
  digitalWrite(E32_M1,(mode & 0b10));
  //while(!digitalRead(E32_AUX)); //This wait until it become high  
}

void E32::setConfig(void){
  E32Serial.write(Para_Setting_CMD);
  E32Serial.write((Para_Setting_ADDR & 0xFF00) >> 2*4);
  E32Serial.write(Para_Setting_ADDR & 0x00FF);

  E32Serial.write( (Para_Setting_UART_pbit << SPED_USART_pbit) |\
  (Para_Setting_UART_brate << SPED_USART_brate) |\
  (Para_Setting_UART_adrate << SPED_USART_adrate));

  E32Serial.write(Para_setting_Channel_reg);
  
  E32Serial.write(Para_Setting_TranEnbit << OPT_Mode |\
  Para_Setting_IODriverMode << OPT_IOMode |\
  Para_Setting_WirelessWakeUpTime << OPT_Waketime |\
  Para_Setting_FECSwitch << OPT_FEC_switch |\
  Para_Setting_TransPower << OPT_Transmission_power);
  
  while(!digitalRead(E32_AUX)); //This wait until it become high
  while(E32Serial.available()){
    Serial.write(E32Serial.read()); //clear the buffer
    }
}

void E32::returnConfig(void){
    changeMode(Mode_Sleep); 
    // C1 C1 C1
    Serial2.write(193);
    Serial2.write(193);
    Serial2.write(193);
    //while(!digitalRead(E32_AUX)); //This wait until it become high
    Serial.write(0x10);
    delay(1000);
    while(Serial2.available() > 0){
      Serial.write(Serial2.read());
      delay(100);
    }
    Serial.write(0x01);
    changeMode(Mode_Normal);
}

void E32::sendMessage(E32Device targetDevice, char* outData){
  uint8_t strlen_outData = strlen(outData);
  E32Serial.write(targetDevice.addr >> 8);
  E32Serial.write(targetDevice.addr);
  E32Serial.write(targetDevice.channel);
  //data
  //Serial.write(strlen(outData));
  E32Serial.write(strlen_outData);
  for(uint8_t i = 0; i < strlen_outData; i++){
    E32Serial.write(outData[i]);
    //Serial.write(outData[i]);
  }
  while(!digitalRead(E32_AUX));
  delay(30);
}

bool E32::receiveComplete(){
  //return true when receive complete
 if(E32Serial.available() > 0){
      char inChar = E32Serial.read();
      if(inDataTop <= 0){
        //get the data length, first byte
        inDataTop = inChar;
        inDataAry.dataLen = inChar;
      }else{
        //Storing data
        inDataAry.dataPtr[inDataAry.dataLen - inDataTop] = inChar;
        inDataTop--; 
        if(inDataTop == 0){
          //data storing complete
          inDataAry.dataPtr[inDataAry.dataLen+1] = 0;
          delay(10);
          return true;
        }
      }
      delay(15);
    }
  return false;
}

bool E32::getAUX(){
  return digitalRead(E32_AUX);  
}

bool E32::receiveTimeout(){
  
  }

E32Data E32::getMessage(){
  return inDataAry;
}
