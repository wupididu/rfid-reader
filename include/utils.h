#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           
#define SS_PIN          10 
#define KEY "bdk93-sdf33-njdo2-aomd3"

#define CM_HANDSHAKE "hShake"
#define CM_READ_DATA "rData"
#define CM_WRITE_DATA "wData"
#define CM_REBOOT "reboot"
#define CM_CHANGE_MODE "cMode"

void sendMessage(const String &s);
void sendMessage(JsonVariantConst data);
void getUuid(byte array[], unsigned int len, char buffer[]);
bool waitNewPresentCard(MFRC522 *mfrc522, unsigned int tryCount);
bool waitNewPresentCard(MFRC522 *mfrc522);
bool auth(byte b, MFRC522 *mfrc522, MFRC522::MIFARE_Key *key);