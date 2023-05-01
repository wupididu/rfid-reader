#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>

#include "utils.h"

void readDataFromCard(MFRC522 *mfrc522, MFRC522::MIFARE_Key *key) {
    MFRC522::StatusCode status;
    byte block, len;

    sendMessage("Start read data");

    if (!waitNewPresentCard(mfrc522)) {
        sendMessage("Hasnt new card");
        return;
    }
    // get uuid

    char bufferUid[32] = "";
    getUuid(mfrc522->uid.uidByte, mfrc522->uid.size, bufferUid);
    String uuid = String((char *)bufferUid);

    // get name

    byte bufferName[18];
    len = 18;
    block = 1;
    if (!auth(block, mfrc522, key)) {
        return;
    }

    status = mfrc522->MIFARE_Read(block, bufferName, &len);
    if (status != MFRC522::STATUS_OK) {
        sendMessage(mfrc522->GetStatusCodeName(status));
        return;
    }

    String name = String((char *)bufferName);

    // get last name

    byte bufferLastName[18];
    block = 2;
    if (!auth(block, mfrc522, key)) {
        return;
    }

    status = mfrc522->MIFARE_Read(block, bufferLastName, &len);
    if (status != MFRC522::STATUS_OK) {
        sendMessage(mfrc522->GetStatusCodeName(status));
        return;
    }

    String lastName = String((char *)bufferLastName);

    // send data

    sendMessage("Success read data");

    StaticJsonDocument<200> doc;

    doc["cmd"] = "rData";
    doc["data"]["uuid"] = uuid;
    doc["data"]["name"] = name;
    doc["data"]["lastName"] = lastName;
    sendMessage(doc);

    mfrc522->PICC_HaltA();
    mfrc522->PCD_StopCrypto1();
}