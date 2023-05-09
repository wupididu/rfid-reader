#define ARDUINOJSON_USE_LONG_LONG 1

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#include "utils.h"

void readData();

void readDataFromCard() {
    if (!waitNewPresentCard(1)) {
        return;
    }

    readData();
}

void readDataFromCard(byte tryCount) {
    sendMessage("Start read data");

    if (!waitNewPresentCard(tryCount)) {
        sendMessage("Cant find card for read data");
        return;
    }

    readData();
}

void readData() {
    sendMessage("Find card for read");

    StaticJsonDocument<200> doc;
    doc["cmd"] = CM_READ_DATA;

    byte block;
    byte buffer[16];

    if (!auth(3)) {
        return;
    }

    // get uuid
    
    block = 1;
    if (!readDataFromBlock(block, buffer)) {
        return;
    }

    doc["data"]["leastUuid"] = String((char *)buffer);

    block = 2;
    if (!readDataFromBlock(block, buffer)) {
        return;
    }

    doc["data"]["mostUuid"] = String((char *)buffer);
     

    if (!auth(7)) {
        return;
    }

    // get name

    block = 4;

    if (!readDataFromBlock(block, buffer)) {
        return;
    }

    doc["data"]["name"] = String((char *)buffer);
     
    // get last name

    block = 5;

    if (!readDataFromBlock(block, buffer)) {
        return;
    }

    doc["data"]["lastName"] = String((char *)buffer);

    // get middleName

    block = 6;

    if (!readDataFromBlock(block, buffer)) {
        return;
    }

    doc["data"]["middleName"] = String((char *)buffer);

    if (!auth(11)) {
        return;
    }

    // get user type

    block = 8;

    if (!readDataFromBlock(block, buffer)) {
        return;
    }

    doc["data"]["userType"] = String((char *)buffer);

    // send data

    sendMessage("Success read data");
    sendMessage(doc);

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}