#ifndef _APPS_H_
#define _APPS_H_
#pragma once

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "display.h"
#include "clientRequest.h"

class App {
    protected:
    MultiProDisplay* pt_multiProDisplay;
    align_t pt_align;
    uint32_t pt_duration;
    uint32_t pt_updateFrequency;
    wchar_t str2wchar(std::string unit);
    public:
    App(MultiProDisplay* multiProDisplay, align_t align = RIGHT, uint32_t duration = 30);
    App(MultiProDisplay* multiProDisplay, align_t align = RIGHT, uint32_t duration = 30, uint32_t updateFrequency = -1);
    void run(void);
    virtual void update(bool first, uint32_t remaining_duration) = 0;
};

class ClockApp : public App {
    private:
    bool m_showSecond;
    std::string m_align;
    uint32_t m_start;
    uint8_t m_lastHour;
    uint8_t m_lastMinute;
    uint8_t m_lastSecond;
    public:
    ClockApp(MultiProDisplay* multiProDisplay, align_t align = CENTER, bool showSecond = true, uint32_t duration = 30, uint32_t updateFrequency = -1);
    void update(bool first, uint32_t remaining_duration) override;
};

class CryptoApp : public App {
    public:
    CryptoApp(MultiProDisplay* multiProDisplay, align_t align = CENTER, std::string crypto="bitcoin", std::string currency="usd", uint8_t decimal=4, uint32_t duration = 30, uint32_t updateFrequency = -1);
    void update(bool first, uint32_t remaining_duration) override;
    void setArgs(std::string crypto, std::string currency, uint8_t decimal);
    private:
    std::string m_crypto;
    std::string m_currency;
    uint8_t m_decimal;
    Request m_request;
};

#endif // APPS_H