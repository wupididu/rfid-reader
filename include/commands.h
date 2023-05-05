#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <ArduinoJson.h>

bool handShake(JsonObject* data);
void readDataFromCard(MFRC522 *mfrc522, MFRC522::MIFARE_Key *key);
void readDataFromCard(MFRC522 *mfrc522, MFRC522::MIFARE_Key *key, unsigned int tryCount);
void writeDataToCard(JsonObject* data, MFRC522 *mfrc522, MFRC522::MIFARE_Key *key, unsigned int tryCount);