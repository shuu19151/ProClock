#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "main.h"
#include "nvs_flash.h"
#include "apps.h"
#include "myEEPROM.h"
#include "wifiClientEvents.h"

// 1: BLE, 0: Webserver
#define BLE_ENABLE (0) 

#if BLE_ENABLE
  #include "ble.h"
#else
  #include "myWebserver.h"
#endif

app_selection_e app_selection = APP_CLOCK;
user_settings_e user_settings = USER_SET_CLOCK;
user_settings_e prev_user_settings = USER_SET_CLOCK;
uint8_t commit_flags = 0b00000000;
uint8_t ble_flags = 0b00000000;
uint8_t app_flags = 0b00000000;
uint8_t webserver_flags = 0b00000000;
wifi_credential_t wifi_cre;
crypto_settings_t crypto_set;
device_datetime_t dvDateTime;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

void TaskApp(void *pvParameters);
void TaskProcess(void *pvParameters);

void initWiFi(void);
void AppProcess(void);
void set_app_flags(void);

TaskHandle_t TaskAppHandle = NULL;
TaskHandle_t TaskProcessHandle = NULL;

SemaphoreHandle_t display_mutex = NULL;

PCF8563 rtc;
MultiProDisplay multiProDisplay(0x70, 5, SCREEN_WIDTH, SCREEN_HEIGHT);
// ClockApp clockApp(&multiProDisplay, CENTER, true, 100, 0);
// CryptoApp cryptoApp(&multiProDisplay, CENTER, crypto_set.crypto, crypto_set.currency, 30);
ClockApp* clockApp = NULL;
CryptoApp* cryptoApp = NULL;

void setup() {
    Serial.begin(115200);
    if(multiProDisplay.begin() != 0) {
        DEBUG(F("SSD1306 allocation failed"));
        for(;;);
    }
    rtc.begin();

    nvs_flash_init();

    display_mutex = xSemaphoreCreateMutex();
    initEEPROM();
    initWiFi();
    #if BLE_ENABLE
      initBLE();
    #else
      setupAccessPoint();
    #endif

    xTaskCreatePinnedToCore(TaskProcess, "TaskProcess", (1024 * 10), NULL, 1, &TaskProcessHandle, ARDUINO_RUNNING_CORE);
}

void loop() {}

void initWiFi(void) {
  #if BLE_ENABLE
  WiFi.mode(WIFI_STA);
  #else
  WiFi.mode(WIFI_AP_STA);
  #endif
  WiFi.onEvent(WiFiEvent);
  if(wifi_cre.wifi_ssid != "" && wifi_cre.wifi_pass != "") { // Check if WiFi credentials found
    WiFi.begin(wifi_cre.wifi_ssid.c_str(), wifi_cre.wifi_pass.c_str());
    if(xSemaphoreTake(display_mutex, portMAX_DELAY) == pdTRUE) {
      multiProDisplay.clearAllDisplay();
      multiProDisplay.render_string(("  " + wifi_cre.wifi_ssid), false, true);
      multiProDisplay.getDisplay(0).render_character(CHAR_WIFI, 0);
      multiProDisplay.show();
      DEBUG("Display wifi ssid");
      xSemaphoreGive(display_mutex);
    }
    uint8_t s_retry_num = 0;
    while(!IS_BIT_SET(app_flags, APP_RUN_ONLINE_FLAG)) { // Check if connected to WiFi
      if (s_retry_num < 10) { // try 10 times
        s_retry_num++;
        DEBUG("retry to connect to the AP");
        if(WiFi.status() == WL_CONNECTED) { // display check mark if connected
          if(xSemaphoreTake(display_mutex, portMAX_DELAY) == pdTRUE) {
            multiProDisplay.clearAllDisplay();
            multiProDisplay.getDisplay(2).render_character(CHAR_CHECK, 0);
            multiProDisplay.show();
            DEBUG("Display check mark");
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            xSemaphoreGive(display_mutex);
          }
          break;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
      }
      else { // display cross mark if not connected after 10 times
        DEBUGF("Failed to connect to %s\n", wifi_cre.wifi_ssid.c_str());
        if(xSemaphoreTake(display_mutex, portMAX_DELAY) == pdTRUE) {
          multiProDisplay.clearAllDisplay();
          multiProDisplay.getDisplay(2).render_character(CHAR_CROSS, 0);
          multiProDisplay.show();
          vTaskDelay(2000 / portTICK_PERIOD_MS);
          DEBUG("Display cross mark");
          xSemaphoreGive(display_mutex);
        }
        break;
      }
    }
    if(xSemaphoreTake(display_mutex, portMAX_DELAY) == pdTRUE) {
      multiProDisplay.clearAllDisplay();
      multiProDisplay.getDisplay(2).render_character(CHAR_CHECK, 0);
      multiProDisplay.show();
      DEBUG("Display check mark");
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      xSemaphoreGive(display_mutex);
    }
  }
  else { // display no wifi if no WiFi credentials found
    DEBUG("No WiFi credentials found");
    if(xSemaphoreTake(display_mutex, portMAX_DELAY) == pdTRUE) {
      multiProDisplay.clearAllDisplay();
      multiProDisplay.render_string("NO WIFI", true, false);
      multiProDisplay.show();
      DEBUG("Display no wifi");
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      xSemaphoreGive(display_mutex);
    }
  }
  if(TaskAppHandle != NULL) {
    xTaskNotifyGive(TaskAppHandle);
  }
}

void TaskProcess(void *pvParameters) {
    for(;;) {
      #if BLE_ENABLE
        BLEProcess();
      #else
        webserverProcess();
      #endif
        EEPROMProcess();
        AppProcess();
    }
}

void AppProcess(void) {
  set_app_flags();
  if(!IS_BIT_SET(app_flags, APP_IS_RUNNING_FLAG)) {
    xTaskCreatePinnedToCore(TaskApp, "TaskApp", (1024 * 16), NULL, 2, &TaskAppHandle, ARDUINO_RUNNING_CORE);
    SET_BIT(app_flags, APP_IS_RUNNING_FLAG);
  }
}

void set_app_flags(void) {
  if(user_settings != prev_user_settings) {
    CLR_BIT(app_flags, APP_RUN_NORMAL_FLAG);
    prev_user_settings = user_settings;
  }
  if (IS_BIT_SET(ble_flags, BLE_DISPLAY_SYMBOL_FLAG)) {
    if (xSemaphoreTake(display_mutex, portMAX_DELAY) == pdTRUE) {
        multiProDisplay.clearAllDisplay();
        if (IS_BIT_SET(ble_flags, BLE_CLIENT_CONNECT_FLAG)) {
          multiProDisplay.getDisplay(2).render_character(CHAR_BLUETOOTH, 0);
          CLR_BIT(ble_flags, BLE_CLIENT_CONNECT_FLAG);
        } else {
          multiProDisplay.getDisplay(2).render_character(CHAR_NOBLUETOOTH, 0);
        }
        multiProDisplay.show();
        vTaskDelay(1500 / portTICK_PERIOD_MS);
        xSemaphoreGive(display_mutex);
        xTaskNotifyGive(TaskAppHandle);
        CLR_BIT(ble_flags, BLE_DISPLAY_SYMBOL_FLAG);
    }
  }
  // If the app is running in config mode, set the flag then restart application
  if(IS_BIT_SET(app_flags, APP_RUN_ONLINE_FLAG) && !IS_BIT_SET(app_flags, APP_RUN_NORMAL_FLAG)) {
    if(IS_BIT_SET(webserver_flags, WS_NEW_WIFI_FLAG)) {
      if(app_selection == APP_CRYPTO) {
        vTaskDelete(TaskAppHandle);
      }
      CLR_BIT(webserver_flags, WS_NEW_WIFI_FLAG);
      WiFi.disconnect(true);
      initWiFi();
    }
    else if(user_settings == USER_SET_CLOCK && app_selection != APP_CLOCK) {
      app_selection = APP_CLOCK;
      vTaskDelete(TaskAppHandle);
      CLR_BIT(app_flags, APP_IS_RUNNING_FLAG);
    }
    else if(user_settings == USER_SET_CRYPTO && app_selection != APP_CRYPTO) {
      app_selection = APP_CRYPTO;
      vTaskDelete(TaskAppHandle);
      CLR_BIT(app_flags, APP_IS_RUNNING_FLAG);
    }
    SET_BIT(app_flags, APP_RUN_NORMAL_FLAG);
  }
  // If the app is running in offline mode, start clock app
  else if(!IS_BIT_SET(app_flags, APP_RUN_ONLINE_FLAG)) {
    if(app_selection != APP_CLOCK) {
      app_selection = APP_CLOCK;
      vTaskDelete(TaskAppHandle);
      CLR_BIT(app_flags, APP_IS_RUNNING_FLAG);
    }
  }
}

void TaskApp(void *pvParameters) {
  if(clockApp != NULL) {
    delete clockApp;
  }
  if(cryptoApp != NULL) {
    delete cryptoApp;
  }
  for(;;) {
    switch (app_selection) {
    case APP_CLOCK:
      clockApp = new ClockApp(&multiProDisplay, CENTER, true, 100, 0);
      clockApp->run();
      delete clockApp;
      break;
    case APP_CRYPTO:
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      cryptoApp = new CryptoApp(&multiProDisplay, CENTER, crypto_set.crypto, crypto_set.currency, 30);
      cryptoApp->run();
      delete cryptoApp;
      break;
    default:
      break;
    }
  }
}