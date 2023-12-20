#line 1 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\main.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include "conf.h"
#include "apps.h"

#define NUM_WIFI_CREDENTIALS 2
const char* wifiCredentials[2][NUM_WIFI_CREDENTIALS] = {
    {"AP25-2_4GHz", "Lamquangtri105"},
    {"Croissant", "wifihuroi"}
};

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

MultiProDisplay multiProDisplay(0x70, 5, SCREEN_WIDTH, SCREEN_HEIGHT);
ClockApp clockApp(&multiProDisplay, CENTER, true, 30, 0);
CryptoApp cryptoApp(&multiProDisplay, CENTER, "bitcoin", "usd", 30);