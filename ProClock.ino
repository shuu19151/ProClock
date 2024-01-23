#include "main.h"
#include "apps.h"
#include "ble.h"
#include "myEEPROM.h"

app_selection_e app_selection = APP_CLOCK;
user_settings_e user_settings = USER_SET_CLOCK;
uint8_t commit_flags = 0b00000000;
uint8_t ble_flags = 0b00000000;
uint8_t app_flags = 0b00000000;
wifi_credential_t wifi_cre;
crypto_settings_t crypto_set;

// #define NUM_WIFI_CREDENTIALS 2
// const char* wifiCredentials[2][NUM_WIFI_CREDENTIALS] = {
//     {"AP25-2_4GHz", "Lamquangtri105"},
//     {"Croissant", "wifihuroi"}
// };

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


void TaskApp(void *pvParameters);
void TaskProcess(void *pvParameters);

void initWiFi(void);

TaskHandle_t TaskAppHandle = NULL;
TaskHandle_t TaskProcessHandle = NULL;

MultiProDisplay multiProDisplay(0x70, 5, SCREEN_WIDTH, SCREEN_HEIGHT);
ClockApp clockApp(&multiProDisplay, CENTER, true, 30, 0);
CryptoApp cryptoApp(&multiProDisplay, CENTER, "bitcoin", "usd", 30);


void setup() {
  Serial.begin(115200);
  if(multiProDisplay.begin() != 0) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  initEEPROM();
  initWiFi();
  initBLE();

  xTaskCreatePinnedToCore(TaskProcess, "TaskProcess", (1024 * 10), NULL, 1, &TaskProcessHandle, ARDUINO_RUNNING_CORE);
}

void loop() {}

void initWiFi(void) {
    multiProDisplay.clearAllDisplay();
    multiProDisplay.render_string(("  " + wifi_cre.wifi_ssid), false, true);
    multiProDisplay.getDisplay(0).render_character(CHAR_WIFI, 0);
    multiProDisplay.show();
    
    WiFi.begin(wifi_cre.wifi_ssid.c_str(), wifi_cre.wifi_pass.c_str());
    uint8_t count = 0;
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if(count++ > 10) {
        break;
      }
    }
    if(WiFi.status() == WL_CONNECTED) {
      multiProDisplay.clearAllDisplay();
      multiProDisplay.getDisplay(2).render_character(CHAR_CHECK, 0);
      multiProDisplay.show();
    }
    else {
      Serial.printf("Failed to connect to %s\n", wifi_cre.wifi_ssid.c_str());
      multiProDisplay.clearAllDisplay();
      multiProDisplay.getDisplay(2).render_character(CHAR_CROSS, 0);
      multiProDisplay.show();
    }
  delay(2000);
  if(WiFi.status() != WL_CONNECTED) {
    multiProDisplay.clearAllDisplay();
    multiProDisplay.render_string("NO WIFI", true, false);
    multiProDisplay.show();
    delay(2000);
  }
  // for(uint i = 0; i < NUM_WIFI_CREDENTIALS; i++) {
  //   multiProDisplay.clearAllDisplay();
  //   multiProDisplay.render_string(String("  " + String(wifiCredentials[i][0])), false, true);
  //   multiProDisplay.getDisplay(0).render_character(CHAR_WIFI, 0);
  //   multiProDisplay.show();
  //   Serial.printf("Connecting to %s\n", wifiCredentials[i][0]);
  //   WiFi.begin(wifiCredentials[i][0], wifiCredentials[i][1]);
  //   uint8_t count = 0;
  //   while(WiFi.status() != WL_CONNECTED) {
  //     delay(500);
  //     Serial.print(".");
  //     if(count++ > 10) {
  //       break;
  //     }
  //   }
  //   if(WiFi.status() == WL_CONNECTED) {
  //     multiProDisplay.clearAllDisplay();
  //     multiProDisplay.getDisplay(2).render_character(CHAR_CHECK, 0);
  //     multiProDisplay.show();
  //     break;
  //   }
  //   else {
  //     Serial.printf("Failed to connect to %s\n", wifiCredentials[i][0]);
  //     multiProDisplay.clearAllDisplay();
  //     multiProDisplay.getDisplay(2).render_character(CHAR_CROSS, 0);
  //     multiProDisplay.show();
  //   }
  // }
  // delay(2000);
  // if(WiFi.status() != WL_CONNECTED) {
  //   multiProDisplay.clearAllDisplay();
  //   multiProDisplay.render_string("NO WIFI", true, false);
  //   multiProDisplay.show();
  //   delay(2000);
  // }
}

void TaskProcess(void *pvParameters) {
  for(;;) {
    BLEProcess();
    EEPROMProcess();
    AppProcess();
  }
}

void AppProcess(void) {
  if(WiFi.status() == WL_CONNECTED && !IS_BIT_SET(app_flags, APP_RUN_NORMAL_FLAG)) {
    if(user_settings == USER_SET_CLOCK) {
      app_selection = APP_CLOCK;
    }
    else if(user_settings == USER_SET_CRYPTO) {
      app_selection = APP_CRYPTO;
    }
  }
  else {
    app_selection = APP_CLOCK;
  }
  if(!IS_BIT_SET(app_flags, APP_IS_RUNNING_FLAG)) {
    xTaskCreatePinnedToCore(TaskApp, "TaskApp", (1024 * 10), NULL, 2, &TaskAppHandle, ARDUINO_RUNNING_CORE);
    SET_BIT(app_flags, APP_IS_RUNNING_FLAG);
  }
}

void TaskApp(void *pvParameters) {
  for(;;) {
    switch (app_selection) {
    case APP_CLOCK:
      clockApp.run();
      break;
    
    case APP_CRYPTO:
      cryptoApp.setArgs(crypto_set.compare_crypto, crypto_set.base_currency, 4);
      cryptoApp.run();
      break;
    default:
      break;
    }
  }
}