#ifndef _MAIN_H_
#define _MAIN_H_

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include "myPCF8563.h"
#include "flags.h"
#include "utils.h"

typedef struct {
  Time dateTime;
  Time setDateTime;
} device_datetime_t;

typedef struct {
  std::string wifi_ssid;
  std::string wifi_pass;
} wifi_credential_t;

typedef struct {
  std::string currency;
  std::string crypto;
} crypto_settings_t;

typedef enum {
    APP_CLOCK = 0,
    APP_CRYPTO,
} app_selection_e;

typedef enum {
    USER_SET_CLOCK = 0,
    USER_SET_CRYPTO,
} user_settings_e;

extern PCF8563 rtc;
extern uint8_t app_flags;
extern uint8_t commit_flags;
extern uint8_t ble_flags;
extern uint8_t webserver_flags;
extern wifi_credential_t wifi_cre;
extern crypto_settings_t crypto_set;
extern device_datetime_t dvDateTime;
extern user_settings_e user_settings;
extern SemaphoreHandle_t display_mutex;
extern TaskHandle_t TaskAppHandle;
// extern AsyncEventSource events;
#endif