#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#include "utils.h"
 
void writeDataToCard(JsonObject data, byte tryCount) {
    sendMessage("Start write data");

    if (!waitNewPresentCard(tryCount)) {
    sendMessage("Hasnt new card");
    return;
    }

    byte block;

    if (!auth(3)) {
        return;
    }

    // write data

    sendMessage("Write uuid");
    block = 1;
    if (!writeDataToBlock(block, data["leastUuid"].as<const char*>())) {
        return;
    }

    block = 2;

    if (!writeDataToBlock(block, data["mostUuid"].as<const char*>())) {
        return;
    }

    if (!auth(7)) {
        return;
    }

    sendMessage("Write name");
    block = 4;

    if (!writeDataToBlock(block, data["name"].as<const char*>())) {
        return;
    }

    sendMessage("Write last name");
    block = 5;
    
    if (!writeDataToBlock(block, data["lastName"].as<const char*>())) {
        return;
    }

    sendMessage("Write middle name");
    block = 6;
    
    if (!writeDataToBlock(block, data["middleName"].as<const char*>())) {
        return;
    }

    if (!auth(11)) {
        return;
    }

    sendMessage("Write user type");
    block = 8;
    
    if (!writeDataToBlock(block, data["userType"].as<const char*>())) {
        return;
    }

    sendMessage("Success write data");

    StaticJsonDocument<200> doc;

    doc["cmd"] = CM_WRITE_DATA;
    doc["data"] = true;
    sendMessage(doc);

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}