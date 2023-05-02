#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#include "utils.h"
#include "commands.h"

MFRC522::MIFARE_Key key;
MFRC522 mfrc522(SS_PIN, RST_PIN);
String cm;
bool readMode, isConnected;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
  SPI.begin();
  mfrc522.PCD_Init();
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  readMode=false;
}

void loop() {
  if(isConnected && readMode) {
    readDataFromCard(&mfrc522, &key);
  }

  if (Serial.available() > 0) {
    StaticJsonDocument<200> doc;
    DeserializationError err = deserializeJson(doc, Serial);

    if (err != DeserializationError::Ok) {
      sendMessage(err.c_str());
    }

    cm = doc["cmd"].as<String>();
    JsonVariantConst data = doc["data"];

    if (cm == "hShake") {
      isConnected=handShake(data);
    } else if (cm == "rData") {
      readDataFromCard(&mfrc522, &key, 10);
    } else if (cm == "wData") {  
      writeDataToCard(data, &mfrc522, &key, 10);
    } else if (cm == "reboot") {
      void (*reboot)(void) = 0;
      reboot();
    } else if (cm == "cMode") {
      readMode=data["readMode"].as<bool>();
      sendMessage(doc);
    }
  }
  delay(1000);
}


