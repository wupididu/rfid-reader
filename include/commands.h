#include <Arduino.h>
#include <ArduinoJson.h>

bool handShake(JsonVariantConst data);
void readDataFromCard(MFRC522 *mfrc522, MFRC522::MIFARE_Key *key);
void readDataFromCard(MFRC522 *mfrc522, MFRC522::MIFARE_Key *key, unsigned int tryCount);
void writeDataToCard(JsonVariantConst data, MFRC522 *mfrc522, MFRC522::MIFARE_Key *key);
void writeDataToCard(JsonVariantConst data, MFRC522 *mfrc522, MFRC522::MIFARE_Key *key, unsigned int tryCount);