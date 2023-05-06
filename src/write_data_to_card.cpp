#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#include "utils.h"

bool writeDataToBlock(String *value, byte *block, byte len);
bool writeDataToBlock(long long *value, byte *block, byte len);
 
void writeDataToCard(JsonObject* data, byte tryCount) {
    sendMessage("Start write data");

    if (!waitNewPresentCard(tryCount)) {
    sendMessage("Hasnt new card");
    return;
    }

    byte block;

    // get data
    (*data)["uuid"];
    String uuid = (*data)["uuid"].as<String>();
    String name = (*data)["name"].as<String>();
    String lastName = (*data)["lastName"].as<String>();
    String userType = (*data)["userType"].as<String>();
    long long correctUntil = (*data)["correctUntil"].as<long long>();

    // write data

    sendMessage("Write data");

    block = 1;
    if (!writeDataToBlock(&uuid, &block, 16)) {
        return;
    }

    sendMessage("Write name");
    block = 2;
    if (!writeDataToBlock(&name, &block, 16)) {
        return;
    }

    sendMessage("Write last name");
    block = 4;
    if (!writeDataToBlock(&lastName, &block, 16)) {
        return;
    }

    sendMessage("Write user type");
    block = 5;
    if (!writeDataToBlock(&userType, &block, 16)) {
        return;
    }

    sendMessage("Write correct until");
    block = 6;
    if (!writeDataToBlock(&correctUntil, &block, 16)) {
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

bool writeDataToBlock(String *value, byte *block, byte len) {
    if (!auth(*block)) {
        return false;
    }

    sendMessage("Write data: Auth success");

    MFRC522::StatusCode status;
    byte buffer[34];

    sendMessage("get bytes");
    sendMessage(*value);

    value->getBytes(buffer, sizeof(buffer));

    sendMessage("mifare write");

    status = mfrc522.MIFARE_Write(*block, buffer, len);
    if (status != MFRC522::STATUS_OK) {
        sendMessage(mfrc522.GetStatusCodeName(status));
        return false;
    }

    return true;
}

bool writeDataToBlock(long long *value, byte *block, byte len) {
    if (!auth(*block)) {
        return false;
    }

    sendMessage("Write data: Auth success");

    MFRC522::StatusCode status;

    status = mfrc522.MIFARE_Write(*block, (unsigned char *)value, len);
    if (status != MFRC522::STATUS_OK) {
        sendMessage(mfrc522.GetStatusCodeName(status));
        return false;
    }

    return true;
}