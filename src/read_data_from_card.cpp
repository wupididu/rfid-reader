#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#include "utils.h"

void readDataFromCard(byte tryCount) {
    sendMessage("Start read data");

    if (!waitNewPresentCard(tryCount)) {
        sendMessage("Hasn't new card");
        return;
    }

    StaticJsonDocument<200> doc;
    doc["cmd"] = CM_READ_DATA;
   
    // get uidCard

    char bufferUid[7] = "";
    getUuid(mfrc522.uid.uidByte, mfrc522.uid.size, bufferUid);
    doc["data"]["uidCard"] = String((char *)bufferUid);

    //

    byte block, len;
    byte buffer[18];

    // get uuid

    block = 1;
    len = 18;

    if (!readDataFromBlock(block, buffer, &len)) {
        return;
    }

    doc["data"]["uuid"] = String((char *)buffer);
     
    // get name

    block = 2;
    len = 18;

    if (!readDataFromBlock(block, buffer, &len)) {
        return;
    }

    doc["data"]["name"] = String((char *)buffer);
     
    // get last name

    block = 4;
    len = 18;

    if (!readDataFromBlock(block, buffer, &len)) {
        return;
    }

    doc["data"]["lastName"] = String((char *)buffer);

    // get user type

    block = 5;
    len = 18;

    if (!readDataFromBlock(block, buffer, &len)) {
        return;
    }

    doc["data"]["userType"] = String((char *)buffer);

    // get correctUntil

    block = 6;
    len = 18;

    if (!readDataFromBlock(block, buffer, &len)) {
        return;
    }

    doc["data"]["correctUntil"] = String((char *)buffer);

    // send data

    sendMessage("Success read data");
    sendMessage(doc);

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}