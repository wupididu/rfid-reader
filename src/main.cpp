#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#include "utils.h"
#include "commands.h"

bool readMode, isConnected;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10000);

  SPI.begin();
  mfrc522.PCD_Init();
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  readMode=false;
  isConnected=false;
}

void loop() {
  if(isConnected && readMode) {
    readDataFromCard(1);
  }
  
  if (Serial.available() > 0) {
    StaticJsonDocument<400> doc;
    DeserializationError err = deserializeJson(doc, Serial);
    if (err != DeserializationError::Ok) {
      sendMessage(err.c_str());
    }

    String cm = doc["cmd"].as<String>();
    JsonObject data = doc["data"].as<JsonObject>();

    if (cm == CM_HANDSHAKE) {
      isConnected=handShake(data);
    } else if (cm == CM_READ_DATA) {
      readDataFromCard(10);
    } else if (cm == CM_WRITE_DATA) {  
      writeDataToCard(data, 10);
    } else if (cm == CM_REBOOT) {
      void (*reboot)(void) = 0;
      reboot();
    } else if (cm == CM_CHANGE_MODE) {
      readMode=data["readMode"].as<bool>();
      sendMessage(doc);
    }
  }

  delay(2000);
}