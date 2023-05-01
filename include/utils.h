#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           
#define SS_PIN          10 
#define KEY "bdk93-sdf33-njdo2-aomd3"

void sendMessage(const String &s);
void sendMessage(JsonVariantConst data);
void getUuid(byte array[], unsigned int len, char buffer[]);
bool waitNewPresentCard(MFRC522 *mfrc522);
bool auth(byte b, MFRC522 *mfrc522, MFRC522::MIFARE_Key *key);