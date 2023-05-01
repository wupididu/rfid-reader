#include <Arduino.h>
#include <ArduinoJson.h>


#include "utils.h"

void handShake(JsonVariantConst data) {
  String key = data["key"].as<String>();
  if (key == KEY) {
    StaticJsonDocument<200> doc;
    doc["cmd"] = "hShake";
    doc["data"]["key"] = KEY;
    sendMessage(doc); 
  }
}