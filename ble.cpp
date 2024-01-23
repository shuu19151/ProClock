#include "ble.h"

void initBLE(void) {
  BLEDevice::init(BLE_NAME);
  Serial.printf("BLE Server Mac Address: %s\n", BLEDevice::getAddress().toString().c_str());

  /* Create BLE Server */
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  /* Create BLE Service */
  pService = pServer->createService(SERVICE_UUID);

  /* Create a BLE Characteristic for notifying */
  pCharacteristicTX = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
  // Create a BLE Descriptor
  pCharacteristicTX->addDescriptor(new BLE2902());

  /* Create a BLE Characteristic for reading */
  pCharacteristicRX = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
  pCharacteristicRX->setCallbacks(new RXCallback());

  pService->start();

  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void ServerCallbacks::onConnect(BLEServer* pSv) {
    Serial.println("Connected");
}

void ServerCallbacks::onDisconnect(BLEServer* pSv) {
    Serial.println("Disconnected");
}

void RXCallback:: onWrite(BLECharacteristic* pCh) {
  std::string data = pCh->getValue();
  if (data.length() < 1) return;
  Serial.printf("Data received: %s\n", data.c_str());
  Serial.printf("Data length: %d\n", data.length());

  switch (data[0]) {
    case BLE_WIFI_PASSWORD: {
      wifi_cre.wifi_pass = data.substr(1);
      Serial.printf("WiFi password: %s\n", wifi_cre.wifi_pass.c_str());
      SET_BIT(ble_flags, BLE_NEW_WIFI_FLAG);
      break;
    }
    case BLE_WIFI_SSID: {
      wifi_cre.wifi_ssid = data.substr(1);
      Serial.printf("WiFi SSID: %s\n", wifi_cre.wifi_ssid.c_str());
      SET_BIT(ble_flags, BLE_NEW_WIFI_FLAG);
      break;
    }
    case BLE_CRYPTO_CURRENCY: {
      crypto_set.base_currency = data.substr(1);
      Serial.printf("Crypto currency: %s\n", data.substr(1).c_str());
      SET_BIT(ble_flags, BLE_NEW_CRYPTO_FLAG);
      break;
    }
    case BLE_CRYPTO_CRYPTO: {
      crypto_set.compare_crypto = data.substr(1);
      Serial.printf("Crypto: %s\n", data.substr(1).c_str());
      SET_BIT(ble_flags, BLE_NEW_CRYPTO_FLAG);
      break;
    }
    case BLE_USER_SET_APP: {
      user_settings = (user_settings_e)std::stoi(data.substr(1)); // Convert string to int
      Serial.printf("User set app: %d\n", user_settings);
      break;
    }
    default:
      break;
  }
}

void BLEProcess(void) {
  if(IS_BIT_SET(ble_flags, BLE_NEW_WIFI_FLAG)) { // Check if new WiFi credentials received
    Serial.println("New WiFi credentials received");
    Serial.printf("WiFi SSID: %s\n", wifi_cre.wifi_ssid.c_str());
    Serial.printf("WiFi password: %s\n", wifi_cre.wifi_pass.c_str());
    CLR_BIT(ble_flags, BLE_NEW_WIFI_FLAG);
    SET_BIT(commit_flags, COMMIT_WIFI_FLAG);
  }
  if(IS_BIT_SET(ble_flags, BLE_NEW_CRYPTO_FLAG)) { // Check if new crypto settings received
    Serial.println("New crypto settings received");
    Serial.printf("Base currency: %s\n", crypto_set.base_currency.c_str());
    Serial.printf("Compare crypto: %s\n", crypto_set.compare_crypto.c_str());
    CLR_BIT(ble_flags, BLE_NEW_CRYPTO_FLAG);
    SET_BIT(commit_flags, COMMIT_CRYPTO_FLAG);
  }
}