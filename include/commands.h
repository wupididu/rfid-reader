#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <ArduinoJson.h>

bool handShake(JsonObject* data);
void readDataFromCard(byte tryCount);
void writeDataToCard(JsonObject* data, byte tryCount);