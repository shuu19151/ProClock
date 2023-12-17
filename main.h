#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "conf.h"
#include "apps.h"

const char* ssid = "AP25-2_4GHz";
const char* password = "Lamquangtri105";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

MultiProDisplay multiProDisplay(0x70, 5, SCREEN_WIDTH, SCREEN_HEIGHT);
ClockApp clockApp(&multiProDisplay, CENTER, true, 30, 0);
