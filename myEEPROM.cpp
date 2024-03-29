#include "myEEPROM.h"

void initEEPROM(void) {
  if (!EEPROM.begin(256)) {
    DEBUG("Failed to initialise EEPROM");
    DEBUG("Restarting...");
    delay(1000);
    ESP.restart();
  }

  wifi_cre.wifi_ssid = readStringFromEEPROM(ADDRESS_WIFI_SSID);
  DEBUGF("WiFi SSID: %s\n", wifi_cre.wifi_ssid.c_str());
  wifi_cre.wifi_pass = readStringFromEEPROM(ADDRESS_WIFI_PASSWORD);
  DEBUGF("WiFi password: %s\n", wifi_cre.wifi_pass.c_str());
  crypto_set.currency = readStringFromEEPROM(ADDRESS_CRYPTO_BASE_CURRENCY);
  DEBUGF("Base currency: %s\n", crypto_set.currency.c_str());
  crypto_set.crypto = readStringFromEEPROM(ADDRESS_CRYPTO_COMPARE_CRYPTO);
  DEBUGF("Compare crypto: %s\n", crypto_set.crypto.c_str());
}

void writeStringToEEPROM(int addrOffset, const std::string &strToWrite) {
  uint8_t len = strToWrite.length();
  EEPROM.write(addrOffset, len);

  for (int i = 0; i < len; i++) {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}

void writeIntToEEPROM(int addrOffset, uint8_t intToWrite) {
  EEPROM.write(addrOffset, 1);
  EEPROM.write(addrOffset + 1, intToWrite);
}

std::string readStringFromEEPROM(int addrOffset) {
  int newStrLen = EEPROM.read(addrOffset);
  DEBUG(newStrLen);  // debug

  char data[newStrLen + 1];

  for (int i = 0; i < newStrLen; i++) {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0';

  return data;
}

std::string readBytesFromEEPROM(int addrOffset, int len) {
  char data[len + 1];

  for (int i = 0; i < len; i++) {
    data[i] = EEPROM.read(addrOffset + i);
  }
  data[len] = '\0';

  return data;
}

void EEPROMProcess(void) {
  for (uint8_t i = 1; i < SIZE_OF_8BIT_FLAG; i++) {
    if(IS_BIT_SET(commit_flags, i)) {
      switch (i) {
        case COMMIT_WIFI_FLAG:
          writeStringToEEPROM(ADDRESS_WIFI_SSID, wifi_cre.wifi_ssid);
          writeStringToEEPROM(ADDRESS_WIFI_PASSWORD, wifi_cre.wifi_pass);
          DEBUG("Wrote wifi credentials to EEPROM");
          break;
        case COMMIT_CRYPTO_FLAG:
          writeStringToEEPROM(ADDRESS_CRYPTO_BASE_CURRENCY, crypto_set.currency);
          writeStringToEEPROM(ADDRESS_CRYPTO_COMPARE_CRYPTO, crypto_set.crypto);
          DEBUG("Wrote crypto settings to EEPROM");
          break;
      }
      CLR_BIT(commit_flags, i);
      SET_BIT(commit_flags, COMMIT_EEPROM_FLAG);
    }
  }
  // Commit EEPROM if COMMIT_EEPROM_FLAG is SET
  if (IS_BIT_SET(commit_flags, COMMIT_EEPROM_FLAG)) {
    CLR_BIT(commit_flags, COMMIT_EEPROM_FLAG);
    EEPROM.commit();
  }
}

// void EEPROMProcess(void) {
//   for (int i = 0; i < sizeof(epArr); i++) {
//     if (epArr[i] == true) {
//       switch (i) {
//         case 0:
//           writeStringToEEPROM(EP_FIRMWARE_VERSION, FirmwareVersion);
//           DEBUG("Wrote firmware version to EEPROM");
//           break;
//         case 1:
//           writeStringToEEPROM(EP_TCP_IP_ADDR, TCPServerIP);
//           DEBUG("Wrote TCP server IP to EEPROM");
//           break;
//         case 2:
//           writeStringToEEPROM(EP_LORA_MODE, String(LoRaMode));
//           DEBUG("Wrote LoRa transmission mode to EEPROM");
//           break;
//       }
//       epArr[i] = false;
//       ep_commit = true;
//     }
//     // Commit EEPROM if ep_commit is true
//     if (ep_commit) {
//       ep_commit = false;
//       EEPROM.commit();
//     }
//   }
// }
