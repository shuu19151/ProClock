#ifndef _MY_EEPROM_H_
#define _MY_EEPROM_H_

#include <iostream>
#include <string>
#include "EEPROM.h"
#include "main.h"
#include "utils.h"

#define ADDRESS_WIFI_SSID (0)
#define ADDRESS_WIFI_PASSWORD (40)
#define ADDRESS_CRYPTO_BASE_CURRENCY (128)
#define ADDRESS_CRYPTO_COMPARE_CRYPTO (136)



void initEEPROM(void);
void EEPROMProcess(void);
void writeStringToEEPROM(int addrOffset, const std::string &strToWrite);
void writeIntToEEPROM(int addrOffset, uint8_t intToWrite);
std::string readStringFromEEPROM(int addrOffset);
std::string readBytesFromEEPROM(int addrOffset, int len);

#endif