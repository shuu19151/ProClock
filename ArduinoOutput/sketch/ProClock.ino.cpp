#include <Arduino.h>
#line 1 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\ProClock.ino"
#include "main.h"

#line 3 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\ProClock.ino"
void initWiFi(void);
#line 41 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\ProClock.ino"
void setup();
#line 50 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\ProClock.ino"
void loop();
#line 3 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\ProClock.ino"
void initWiFi(void) {
  for(uint i = 0; i < NUM_WIFI_CREDENTIALS; i++) {
    multiProDisplay.clearAllDisplay();
    multiProDisplay.render_string(String("  " + String(wifiCredentials[i][0])), false, true);
    multiProDisplay.getDisplay(0).render_character(CHAR_WIFI, 0);
    multiProDisplay.show();
    Serial.printf("Connecting to %s\n", wifiCredentials[i][0]);
    WiFi.begin(wifiCredentials[i][0], wifiCredentials[i][1]);
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
      break;
    }
    else {
      Serial.printf("Failed to connect to %s\n", wifiCredentials[i][0]);
      multiProDisplay.clearAllDisplay();
      multiProDisplay.getDisplay(2).render_character(CHAR_CROSS, 0);
      multiProDisplay.show();
    }
  }
  delay(2000);
  if(WiFi.status() != WL_CONNECTED) {
    multiProDisplay.clearAllDisplay();
    multiProDisplay.render_string("NO WIFI", true, false);
    multiProDisplay.show();
    delay(2000);
  }
}

void setup() {
  Serial.begin(115200);
  if(multiProDisplay.begin() != 0) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  initWiFi();
}

void loop() {
  clockApp.run();
  cryptoApp.run();
  // Serial.println("********************LOOP HERE!********************");
}
