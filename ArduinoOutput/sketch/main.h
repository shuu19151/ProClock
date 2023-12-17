#line 1 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\main.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"
#include "display.h"
#include "TCA9548.h"
// #include "fonts.h"

const char* ssid = "AP25-2_4GHz";
const char* password = "Lamquangtri105";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

MultiProDisplay multiProDisplay(0x70, 5, SCREEN_WIDTH, SCREEN_HEIGHT);
