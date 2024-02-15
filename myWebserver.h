#ifndef _MY_WEBSERVER_H_
#define _MY_WEBSERVER_H_
#include "main.h"
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <Arduino_JSON.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

void firstLoadHandler(AsyncWebServerRequest *request);
void updateHandler(AsyncWebServerRequest *request);
void setupAccessPoint(void);
void webserverProcess(void);
#endif // _MY_WEBSERVER_H_