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
#include "ble.h"
#include "myEEPROM.h"
#include "wifiClientEvents.h"

app_selection_e app_selection = APP_CLOCK;
user_settings_e user_settings = USER_SET_CLOCK;
user_settings_e prev_user_settings = USER_SET_CLOCK;
uint8_t commit_flags = 0b00000000;
uint8_t ble_flags = 0b00000000;
uint8_t app_flags = 0b00000000;
wifi_credential_t wifi_cre;
crypto_settings_t crypto_set;

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

MultiProDisplay multiProDisplay(0x70, 5, SCREEN_WIDTH, SCREEN_HEIGHT);
ClockApp clockApp(&multiProDisplay, CENTER, true, 100, 0);
CryptoApp cryptoApp(&multiProDisplay, CENTER, crypto_set.compare_crypto, crypto_set.base_currency, 30);

void setup() {
  Serial.begin(115200);
  if(multiProDisplay.begin() != 0) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  nvs_flash_init();

  display_mutex = xSemaphoreCreateMutex();
  
  initEEPROM();
  initWiFi();
  initBLE();

  xTaskCreatePinnedToCore(TaskProcess, "TaskProcess", (1024 * 10), NULL, 1, &TaskProcessHandle, ARDUINO_RUNNING_CORE);
}

void loop() {}

void initWiFi(void) {
  WiFi.onEvent(WiFiEvent);
  if(wifi_cre.wifi_ssid != "" && wifi_cre.wifi_pass != "") { // Check if WiFi credentials found
    WiFi.begin(wifi_cre.wifi_ssid.c_str(), wifi_cre.wifi_pass.c_str());
    multiProDisplay.clearAllDisplay();
    multiProDisplay.render_string(("  " + wifi_cre.wifi_ssid), false, true);
    multiProDisplay.getDisplay(0).render_character(CHAR_WIFI, 0);
    multiProDisplay.show();
    Serial.println("Display wifi ssid");
    uint8_t s_retry_num = 0;
    while(!IS_BIT_SET(app_flags, APP_RUN_ONLINE_FLAG)) { // Check if connected to WiFi
      if (s_retry_num < 10) { // try 10 times
        s_retry_num++;
        Serial.println("retry to connect to the AP");
        if(WiFi.status() == WL_CONNECTED) { // display check mark if connected
          multiProDisplay.clearAllDisplay();
          multiProDisplay.getDisplay(2).render_character(CHAR_CHECK, 0);
          multiProDisplay.show();
          Serial.println("Display check mark");
          vTaskDelay(2000 / portTICK_PERIOD_MS);
          break;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
      }
      else { // display cross mark if not connected after 10 times
        Serial.printf("Failed to connect to %s\n", wifi_cre.wifi_ssid.c_str());
        multiProDisplay.clearAllDisplay();
        multiProDisplay.getDisplay(2).render_character(CHAR_CROSS, 0);
        multiProDisplay.show();
        Serial.println("Display cross mark");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        break;
      }
    }
    if(WiFi.status() == WL_CONNECTED) { // display check mark if connected
      multiProDisplay.clearAllDisplay();
      multiProDisplay.getDisplay(2).render_character(CHAR_CHECK, 0);
      multiProDisplay.show();
      Serial.println("Display check mark");
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
  }
  else { // display no wifi if no WiFi credentials found
    Serial.println("No WiFi credentials found");
    multiProDisplay.clearAllDisplay();
    multiProDisplay.render_string("NO WIFI", true, false);
    multiProDisplay.show();
    Serial.println("Display no wifi");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    return;
  }
}

void TaskProcess(void *pvParameters) {
  for(;;) {
    BLEProcess();
    EEPROMProcess();
    AppProcess();
  }
}

void AppProcess(void) {
  set_app_flags();
  if(!IS_BIT_SET(app_flags, APP_IS_RUNNING_FLAG)) {
    xTaskCreatePinnedToCore(TaskApp, "TaskApp", (1024 * 10), NULL, 2, &TaskAppHandle, ARDUINO_RUNNING_CORE);
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
    if(user_settings == USER_SET_CLOCK && app_selection != APP_CLOCK) {
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
  for(;;) {
    switch (app_selection) {
    case APP_CLOCK:
      clockApp.run();
      break;
    case APP_CRYPTO:
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      cryptoApp.setArgs(crypto_set.compare_crypto, crypto_set.base_currency, 4);
      cryptoApp.run();
      break;
    default:
      break;
    }
  }
}