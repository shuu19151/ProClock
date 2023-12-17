# 1 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\ProClock.ino"
# 2 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\ProClock.ino" 2

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

void setup() {
  Serial.begin(115200);
  // PCF8563_init();

  if(multiProDisplay.begin() != 0) {
    Serial.println(((reinterpret_cast<const __FlashStringHelper *>(("SSD1306 allocation failed")))));
    for(;;);
  }
  multiProDisplay.clear();
  multiProDisplay.render_string("HELLO", true, false);
}

void loop() {
}
