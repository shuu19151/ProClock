#ifndef _BLE_H_
#define _BLE_H_

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "main.h"
#include "utils.h"

#define BLE_NAME "ESP32-ProClock"
#define SERVICE_UUID "fc96f65e-318a-4001-84bd-77e9d12af44b"
#define CHARACTERISTIC_UUID_TX "94b43599-5ea2-41e7-9d99-6ff9b904ae3a"
#define CHARACTERISTIC_UUID_RX "04d3552e-b9b3-4be6-a8b4-aa43c4507c4d"
#define BLE_KEYWORD 1

typedef enum {
    BLE_WIFI_SSID = 35,
    BLE_WIFI_PASSWORD = 36,
    BLE_CRYPTO_CURRENCY = 37,
    BLE_CRYPTO_CRYPTO = 38,
    BLE_USER_SET_APP = 39,
} ble_command_t;

static BLEServer* pServer = NULL;
static BLEService* pService = NULL;
static BLECharacteristic* pCharacteristicTX = NULL;
static BLECharacteristic* pCharacteristicRX = NULL;


void initBLE(void);

class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pSv);
    void onDisconnect(BLEServer* pSv);
};

class RXCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCh);
};

void BLEProcess(void);
#endif // MY_BLE_H