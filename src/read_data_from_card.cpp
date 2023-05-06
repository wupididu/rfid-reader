#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#include "utils.h"

bool readDataFromBlock(String *value, byte *block, byte *len);
bool readDataFromBlock(long long *value, byte *block, byte *len);

void readDataFromCard(byte tryCount) {
    sendMessage("Start read data");

    if (!waitNewPresentCard(tryCount)) {
        sendMessage("Hasn't new card");
        return;
    }
   
    // get uidCard

    char bufferUid[32] = "";
    getUuid(mfrc522.uid.uidByte, mfrc522.uid.size, bufferUid);
    String uidCard = String((char *)bufferUid);

    byte block, len;

    // get uuid

    String uuid;
    block = 1;
    len = 18;
    if (!readDataFromBlock(&uuid, &block, &len)) {
        return;
    }
     
    // get name

    String name;
    block = 2;
    len = 18;
    if (!readDataFromBlock(&name, &block, &len)) {
        return;
    }
     
    // get last name

    String lastName;
    block = 4;
    len = 18;
    if (!readDataFromBlock(&lastName, &block, &len)) {
        return;
    }

    // get user type

    String userType;
    block = 5;
    len = 18;
    if (!readDataFromBlock(&userType, &block, &len)) {
        return;
    }

    // get correctUntil

    long long correctUntil;
    block = 6;
    len = 18;
    if (!readDataFromBlock(&correctUntil, &block, &len)) {
        return;
    }

    // send data

    sendMessage("Success read data");

    StaticJsonDocument<200> doc;

    doc["cmd"] = CM_READ_DATA;
    doc["data"]["uuid"] = uuid;
    doc["data"]["uidCard"] = uidCard;
    doc["data"]["name"] = name;
    doc["data"]["lastName"] = lastName;
    doc["data"]["correctUntil"] = correctUntil;
    doc["data"]["userType"] = userType;
    sendMessage(doc);

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}

bool readDataFromBlock(String *value, byte *block, byte *len) {
    if (!auth(*block)) {
        return false;
    }

    sendMessage("Auth success");

    byte buffer[18];

    status = mfrc522.MIFARE_Read(*block, buffer, len);
    if (status != MFRC522::STATUS_OK) {
        sendMessage(mfrc522.GetStatusCodeName(status));
        return false;
    }

    *value = String((char *)buffer);
    sendMessage(*value);

    return true;
}

bool readDataFromBlock(long long *value, byte *block, byte *len) {
    if (!auth(*block)) {
        return false;
    }

    sendMessage("Auth success");

    byte buffer[18];

    status = mfrc522.MIFARE_Read(*block, buffer, len);
    if (status != MFRC522::STATUS_OK) {
        sendMessage(mfrc522.GetStatusCodeName(status));
        return false;
    }

    *value = *((long long *)buffer);
    return true;
}