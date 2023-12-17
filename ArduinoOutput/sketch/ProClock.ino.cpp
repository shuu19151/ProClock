#include <Arduino.h>
#line 1 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\ProClock.ino"
#include "main.h"

// const char* ntpServer1 = "pool.ntp.org";
// const char* ntpServer2 = "time.nist.gov";
// const long gmtOffset_sec = 3600;
// const int daylightOffset_sec = 3600;
// const char* timeZone = "CET-1CEST,M3.5.0,M10.5.0/3";
// RTC_PCF8563 rtc;
uint16_t second = 0, minute = 0;
// void PCF8563_init(void) {
//   if (! rtc.begin()) {
//     Serial.println("Couldn't find PCF8563");
//     Serial.flush();
//     while (1) delay(10);
//   }
// }
 
#line 18 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\ProClock.ino"
void setup();
#line 30 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\ProClock.ino"
void loop();
#line 18 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\ProClock.ino"
void setup() {
  Serial.begin(115200);
  // PCF8563_init();

  if(multiProDisplay.begin() != 0) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  multiProDisplay.clear();
  multiProDisplay.render_string("HELLO", true, false);
}

void loop() {
}
