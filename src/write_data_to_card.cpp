#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#include "utils.h"

void writeDataToCard(JsonVariantConst data, MFRC522 *mfrc522, MFRC522::MIFARE_Key *key, unsigned int tryCount) {
    MFRC522::StatusCode status;
    byte block;

    sendMessage("Start write data");

    if (!waitNewPresentCard(mfrc522, tryCount)) {
    sendMessage("Hasnt new card");
    return;
    }

    // get data

    String name = data["name"].as<String>();
    String lastName = data["lastName"].as<String>();

    // write name

    byte bufferName[34]; 

    name.getBytes(bufferName, sizeof(bufferName));
    block = 1;
    //Serial.println(F("Authenticating using key A..."));
    if (!auth(block, mfrc522, key)) {
        return;
    }

    status = mfrc522->MIFARE_Write(block, bufferName, 16);
    if (status != MFRC522::STATUS_OK) {
        sendMessage(mfrc522->GetStatusCodeName(status));
        return;
    }

    // Write last name

    byte bufferLastName[34]; 

    lastName.getBytes(bufferLastName, sizeof(bufferLastName));
    block = 2;
    //Serial.println(F("Authenticating using key A..."));
    if (!auth(block, mfrc522, key)) {
        return;
    }

    status = mfrc522->MIFARE_Write(block, bufferLastName, 16);
    if (status != MFRC522::STATUS_OK) {
        sendMessage(mfrc522->GetStatusCodeName(status));
        return;
    }

    sendMessage("Success write data");

    StaticJsonDocument<200> doc;

    doc["cmd"] = "wData";
    doc["data"] = true;
    sendMessage(doc);

    mfrc522->PICC_HaltA();
    mfrc522->PCD_StopCrypto1();
}

void writeDataToCard(JsonVariantConst data, MFRC522 *mfrc522, MFRC522::MIFARE_Key *key) {
    writeDataToCard(data, mfrc522, key, 1);
}