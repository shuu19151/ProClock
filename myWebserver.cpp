#include "myWebserver.h"
#include "mySPIFFS.h"

static const char* PARAM_HOUR = "hour";
static const char* PARAM_MINUTE = "min";
static const char* PARAM_SECOND = "sec";
static const char* PARAM_DOW = "dayofweek";
static const char* PARAM_DATE = "date";
static const char* PARAM_MONTH = "month";
static const char* PARAM_YEAR = "year";

static const char* PARAM_SSID = "ssid";
static const char* PARAM_PASS = "pass";
static const char* PARAM_CURRENCY = "currency";
static const char* PARAM_CRYPTO = "crypto";
static const char* PARAM_APPSELECT = "app";

AsyncWebServer server(80);

AsyncEventSource events("/events");

static JSONVar SetWebClock;
static JSONVar JSONFirstLoad;

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void firstLoadHandler(AsyncWebServerRequest *request) {
    dvDateTime.dateTime = rtc.getTime();
    // JSONFirstLoad["hour"] = String(dvDateTime.dateTime.hour);
    // JSONFirstLoad["min"] = String(dvDateTime.dateTime.minute);
    // JSONFirstLoad["sec"] = String(dvDateTime.dateTime.second);
    // JSONFirstLoad["dow"] = String(dvDateTime.dateTime.weekday);
    // JSONFirstLoad["date"] = String(dvDateTime.dateTime.day);
    // JSONFirstLoad["mon"] = String(dvDateTime.dateTime.month);
    // JSONFirstLoad["year"] = String(dvDateTime.dateTime.year);

    JSONFirstLoad["ssid"] = String(wifi_cre.wifi_ssid.c_str());
    JSONFirstLoad["pass"] = String(wifi_cre.wifi_pass.c_str());
    JSONFirstLoad["currency"] = String(crypto_set.currency.c_str());
    JSONFirstLoad["crypto"] = String(crypto_set.crypto.c_str());
    
    String JSONStr = JSON.stringify(JSONFirstLoad);
    request->send(200, "application/json", JSONStr);
    DEBUG("First load");
}

void updateHandler(AsyncWebServerRequest *request) {
    String inputMessage;
    
    if(request->hasParam(PARAM_HOUR)) {
      inputMessage = request->getParam(PARAM_HOUR)->value();
      dvDateTime.setDateTime.hour= inputMessage.toInt();
      DEBUGF("Set hour: %s\n", inputMessage);
    }
    /*---*/
    if(request->hasParam(PARAM_MINUTE)) {
      inputMessage = request->getParam(PARAM_MINUTE)->value();
      dvDateTime.setDateTime.minute = inputMessage.toInt();
      DEBUGF("Set min: %s\n", inputMessage);
    }
    /*---*/
    if(request->hasParam(PARAM_SECOND)) {
      inputMessage = request->getParam(PARAM_SECOND)->value();
      dvDateTime.setDateTime.second = inputMessage.toInt();
      DEBUGF("Set min: %s\n", inputMessage);
    }
    /*---*/
    if(request->hasParam(PARAM_DOW)) {
      inputMessage = request->getParam(PARAM_DOW)->value();
      if(inputMessage == "Sun") {
        dvDateTime.setDateTime.weekday = 0;
      }
      else if(inputMessage == "Mon") {
        dvDateTime.setDateTime.weekday = 1;
      }
      else if(inputMessage == "Tue") {
        dvDateTime.setDateTime.weekday = 2;
      }
      else if(inputMessage == "Wed") {
        dvDateTime.setDateTime.weekday = 3;
      }
      else if(inputMessage == "Thur") {
        dvDateTime.setDateTime.weekday = 4;
      }
      else if(inputMessage == "Fri") {
        dvDateTime.setDateTime.weekday = 4;
      }
      else if(inputMessage == "Sat") {
        dvDateTime.setDateTime.weekday = 6;
      }
      else {
        dvDateTime.setDateTime.weekday = 2;
      }
      DEBUGF("Set day of week: %s\n", inputMessage);
    }
    /*---*/
    if(request->hasParam(PARAM_DATE)) {
      inputMessage = request->getParam(PARAM_DATE)->value();
      dvDateTime.setDateTime.day = inputMessage.toInt();
      DEBUGF("Set date: %s\n", inputMessage);
    }
    /*---*/
    if(request->hasParam(PARAM_MONTH)) {
      inputMessage = request->getParam(PARAM_MONTH)->value();
      dvDateTime.setDateTime.month = inputMessage.toInt();
      DEBUGF("Set month: %s\n", inputMessage);
    }
    /*---*/
    if(request->hasParam(PARAM_YEAR)) {
      inputMessage = request->getParam(PARAM_YEAR)->value();
      dvDateTime.setDateTime.year = inputMessage.toInt();
      SET_BIT(webserver_flags, WS_SET_DATETIME_FLAG);
      DEBUGF("Set year: %s\n", inputMessage);
    }
    /*---*/
    if(request->hasParam(PARAM_SSID)) {
      inputMessage = request->getParam(PARAM_SSID)->value();
      wifi_cre.wifi_ssid = inputMessage.c_str();
      DEBUGF("Set SSID: %s\n", inputMessage);
    }
    /*---*/
    if(request->hasParam(PARAM_PASS)) {
      inputMessage = request->getParam(PARAM_PASS)->value();
      wifi_cre.wifi_pass = inputMessage.c_str();
      DEBUGF("Set password: %s\n", inputMessage);
      SET_BIT(webserver_flags, WS_NEW_WIFI_FLAG);
    }
    /*---*/
    if(request->hasParam(PARAM_CURRENCY)) {
      inputMessage = request->getParam(PARAM_CURRENCY)->value();
      crypto_set.currency = inputMessage.c_str();
      DEBUGF("Set currency: %s\n", inputMessage);
    }
    /*---*/
    if(request->hasParam(PARAM_CRYPTO)) {
      inputMessage = request->getParam(PARAM_CRYPTO)->value();
      crypto_set.crypto = inputMessage.c_str();
      DEBUGF("Set crypto: %s\n", inputMessage);
      SET_BIT(webserver_flags, WS_NEW_CRYPTO_FLAG);
    }
    /*---*/
    if(request->hasParam(PARAM_APPSELECT)) {
      inputMessage = request->getParam(PARAM_APPSELECT)->value();
      user_settings = (user_settings_e)inputMessage.toInt();
      DEBUG("App changed");
    }
}

void setupAccessPoint(void) {
  initSPIFFS();
  // Connect to Wi-Fi network with SSID and password
  DEBUG("Setting AP (Access Point)");
  // NULL sets an open Access Point
  WiFi.softAP("ProClock", NULL);
 
  IPAddress ip = WiFi.softAPIP();
  DEBUG("AP IP address: ");
  DEBUG(ip);

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");
  
  server.on("/ReadingsFirstConfig", HTTP_GET, firstLoadHandler);
  
  server.on("/update", HTTP_GET, updateHandler);

  events.onConnect([](AsyncEventSourceClient *client) {
    Serial.println("Client connected");
  });

  server.addHandler(&events); 
  server.begin();
}

void webserverProcess(void) {
  if(IS_BIT_SET(webserver_flags, WS_SET_DATETIME_FLAG)) {
    memcpy(&dvDateTime.dateTime, &dvDateTime.setDateTime, sizeof(dvDateTime.setDateTime));
    rtc.stopClock();
    rtc.setYear(dvDateTime.setDateTime.year);
    rtc.setMonth(dvDateTime.setDateTime.month);
    rtc.setDay(dvDateTime.setDateTime.day);
    rtc.setWeekday(dvDateTime.setDateTime.weekday);
    rtc.setHour(dvDateTime.setDateTime.hour);
    rtc.setMinute(dvDateTime.setDateTime.minute);
    rtc.setSecond(dvDateTime.setDateTime.second);
    rtc.startClock();
    CLR_BIT(webserver_flags, WS_SET_DATETIME_FLAG);
  }
  if(IS_BIT_SET(webserver_flags, WS_NEW_WIFI_FLAG)) {
    DEBUG("New WiFi credentials received");
    DEBUGF("WiFi SSID: %s\n", wifi_cre.wifi_ssid.c_str());
    DEBUGF("WiFi password: %s\n", wifi_cre.wifi_pass.c_str());
    CLR_BIT(app_flags, APP_RUN_NORMAL_FLAG);
    SET_BIT(commit_flags, COMMIT_WIFI_FLAG);
  }
  if(IS_BIT_SET(webserver_flags, WS_NEW_CRYPTO_FLAG)) {
    DEBUG("New crypto settings received");
    DEBUGF("Base currency: %s\n", crypto_set.currency.c_str());
    DEBUGF("Compare crypto: %s\n", crypto_set.crypto.c_str());
    CLR_BIT(app_flags, APP_RUN_NORMAL_FLAG);
    CLR_BIT(webserver_flags, WS_NEW_CRYPTO_FLAG);
    SET_BIT(commit_flags, COMMIT_CRYPTO_FLAG);
  }
}