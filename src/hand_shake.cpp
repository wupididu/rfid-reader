#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <ArduinoJson.h>

#include "utils.h"

bool handShake(JsonObject data) {
  String key = data["key"].as<String>();
  if (key == KEY) {
    StaticJsonDocument<200> doc;
    doc["cmd"] = CM_HANDSHAKE;
    doc["data"]["key"] = KEY;
    sendMessage(doc);
    return true;
  }
  return false;
}