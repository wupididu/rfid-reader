#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#include "utils.h"

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;

void sendMessage(const String &s) {
  Serial.print(s);
  Serial.write('#');
}

void sendMessage(JsonVariantConst data) {
  serializeJson(data, Serial);
  Serial.write('>');
}

void getUuid(byte array[], byte len, char buffer[]) {
  for (unsigned int i = 0; i < len; i++)
   {
      byte nib1 = (array[i] >> 4) & 0x0F;
      byte nib2 = (array[i] >> 0) & 0x0F;
      buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
      buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
   }
   buffer[len*2] = '\0';
}

bool waitNewPresentCard(byte tryCount) {
  unsigned int count = 0;

  while(count < tryCount) {
    if (mfrc522.PICC_IsNewCardPresent()) {
      if (mfrc522.PICC_ReadCardSerial()) {
        return true;
      }
    }
    count++;
    delay(1000);
  }
  
  return false;
}

bool auth(byte b) {
  MFRC522::StatusCode status;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, b, &key, &(mfrc522.uid)); //line 834
  if (status != MFRC522::STATUS_OK) {
    sendMessage(mfrc522.GetStatusCodeName(status));
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return false;
  }
  sendMessage("Auth success");
  return true;
}

bool readDataFromBlock(byte blockAddr, byte *buffer) {
  byte len = 18;
  
  status = mfrc522.MIFARE_Read(blockAddr, buffer, &len);
  if (status != MFRC522::STATUS_OK) {
    sendMessage(mfrc522.GetStatusCodeName(status));
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return false;
  }
  
  sendMessage("Read data success");
  return true;
}

bool writeDataToBlock(byte blockAddr, const char *value) {
  byte buffer[16];
  for (byte i = 0; i < 16; i++) {
    if (i < strlen(value)) {
      buffer[i] = value[i];
    } else {
      buffer[i] = 0x00;
    }
  }
  
  status = mfrc522.MIFARE_Write(blockAddr, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    sendMessage(mfrc522.GetStatusCodeName(status));
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return false;
  }
  sendMessage("Read data success");
  return true;
}