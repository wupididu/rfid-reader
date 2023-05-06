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

    byte block, len;
    byte buffer[18];

    // write data

    sendMessage("Write uuid");
    block = 1;
    len = 16;
    data["uuid"].as<String>().getBytes(buffer, 18);

    if (!writeDataToBlock(block, buffer, len)) {
        return;
    }

    sendMessage("Write name");
    block = 2;
    len = 16;
    data["name"].as<String>().getBytes(buffer, 18);

    if (!writeDataToBlock(block, buffer, len)) {
        return;
    }

    sendMessage("Write last name");
    block = 4;
    len = 16;
    data["lastName"].as<String>().getBytes(buffer, 18);

    if (!writeDataToBlock(block, buffer, len)) {
        return;
    }

    sendMessage("Write last name");
    block = 5;
    len = 16;
    data["middleName"].as<String>().getBytes(buffer, 18);

    if (!writeDataToBlock(block, buffer, len)) {
        return;
    }

    sendMessage("Write user type");
    block = 6;
    len = 16;
    data["userType"].as<String>().getBytes(buffer, 18);
    if (!writeDataToBlock(block, buffer, len)) {
        return;
    }

    sendMessage("Write sportType");
    block = 8;
    len = 16;
    data["sportType"].as<String>().getBytes(buffer, 18);
    if (!writeDataToBlock(block, buffer, len)) {
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