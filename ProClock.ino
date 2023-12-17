#include "main.h"

void main_app(void) {
  // for(uint8_t i  = 0; i < sizeof(apps); i++) {


  // }
}

void setup() {
  Serial.begin(115200);
  // PCF8563_init();
  

  if(multiProDisplay.begin() != 0) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  multiProDisplay.clearAllDisplay();
}

void loop() {
  clockApp.run();
  Serial.println("********************LOOP HERE!********************");
}