#include <Arduino.h>
#include <ArduinoJson.h>

void handShake(JsonVariantConst data);
void readDataFromCard(MFRC522 *mfrc522, MFRC522::MIFARE_Key *key);
void writeDataToCard(JsonVariantConst data, MFRC522 *mfrc522, MFRC522::MIFARE_Key *key);