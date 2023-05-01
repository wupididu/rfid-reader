#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#include "utils.h"
#include "commands.h"

MFRC522::MIFARE_Key key;
MFRC522 mfrc522(SS_PIN, RST_PIN);
String cm;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
  SPI.begin();
  mfrc522.PCD_Init();
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
}

void loop() {
  if (Serial.available() > 0) {
    StaticJsonDocument<200> doc;
    DeserializationError err = deserializeJson(doc, Serial);

    if (err != DeserializationError::Ok) {
      sendMessage(err.c_str());
    }

    cm = doc["cmd"].as<String>();

    if (cm == "hShake") {
      JsonVariantConst data = doc["data"];
      handShake(data);
    } else if (cm == "rData") {
      readDataFromCard(&mfrc522, &key);
    } else if (cm == "wData") {  
      JsonVariantConst data = doc["data"];
      writeDataToCard(data, &mfrc522, &key);
    } else if (cm == "reboot") {
      void (*reboot)(void) = 0;
      reboot();
    }
  }
  delay(3000);
}


