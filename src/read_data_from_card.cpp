#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#include "utils.h"

void readData(MFRC522 *mfrc522, MFRC522::MIFARE_Key *key);
bool readDataFromBlock(MFRC522 *mfrc522, MFRC522::MIFARE_Key *key, String *value, byte *block, byte *len);
bool readDataFromBlock(MFRC522 *mfrc522, MFRC522::MIFARE_Key *key, long long *value, byte *block, byte *len);

void readDataFromCard(MFRC522 *mfrc522, MFRC522::MIFARE_Key *key, unsigned int tryCount) {
    sendMessage("Start read data");

    if (!waitNewPresentCard(mfrc522, tryCount)) {
        sendMessage("Hasn't new card");
        return;
    }
   
    readData(mfrc522, key);
}

void readDataFromCard(MFRC522 *mfrc522, MFRC522::MIFARE_Key *key) {
    if (!waitNewPresentCard(mfrc522, 5)) {
        sendMessage("Hasnt new card");
        return;
    }

    sendMessage("Read data");
    readData(mfrc522, key);
}

void readData(MFRC522 *mfrc522, MFRC522::MIFARE_Key *key) {
    // get uidCard

    char bufferUid[32] = "";
    getUuid(mfrc522->uid.uidByte, mfrc522->uid.size, bufferUid);
    String uidCard = String((char *)bufferUid);

    byte block, len;

    // get uuid

    String uuid;
    block = 1;
    len = 18;
    if (!readDataFromBlock(mfrc522, key, &uuid, &block, &len)) {
        return;
    }
     
    // get name

    String name;
    block = 2;
    len = 18;
    if (!readDataFromBlock(mfrc522, key, &name, &block, &len)) {
        return;
    }
     
    // get last name

    String lastName;
    block = 4;
    len = 18;
    if (!readDataFromBlock(mfrc522, key, &lastName, &block, &len)) {
        return;
    }

    // get user type

    String userType;
    block = 5;
    len = 18;
    if (!readDataFromBlock(mfrc522, key, &userType, &block, &len)) {
        return;
    }

    // get correctUntil

    long long correctUntil;
    block = 6;
    len = 18;
    if (!readDataFromBlock(mfrc522, key, &correctUntil, &block, &len)) {
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

    mfrc522->PICC_HaltA();
    mfrc522->PCD_StopCrypto1();
}

bool readDataFromBlock(MFRC522 *mfrc522, MFRC522::MIFARE_Key *key, String *value, byte *block, byte *len) {
    if (!auth(*block, mfrc522, key)) {
        return false;
    }

    sendMessage("Auth success");

    byte buffer[18];

    MFRC522::StatusCode status;

    status = mfrc522->MIFARE_Read(*block, buffer, len);
    if (status != MFRC522::STATUS_OK) {
        sendMessage(mfrc522->GetStatusCodeName(status));
        return false;
    }

    *value = String((char *)buffer);
    sendMessage(*value);
    return true;
}

bool readDataFromBlock(MFRC522 *mfrc522, MFRC522::MIFARE_Key *key,long long *value, byte *block, byte *len) {
    if (!auth(*block, mfrc522, key)) {
        return false;
    }

    sendMessage("Auth success");

    byte buffer[18];

    MFRC522::StatusCode status;

    status = mfrc522->MIFARE_Read(*block, buffer, len);
    if (status != MFRC522::STATUS_OK) {
        sendMessage(mfrc522->GetStatusCodeName(status));
        return false;
    }

    *value = *((long long *)buffer);
    return true;
}