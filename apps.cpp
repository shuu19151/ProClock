#include "apps.h"
#include <string>
App::App(MultiProDisplay* multiProDisplay, align_t align, uint32_t duration, uint32_t updateFrequency)
: pt_multiProDisplay(multiProDisplay), pt_align(align), pt_duration(duration * 1000) {
    this->pt_updateFrequency = updateFrequency == -1 ? this->pt_duration : updateFrequency * 1000;
}

// Update value frequency is the same as duration
App::App(MultiProDisplay* multiProDisplay, align_t align, uint32_t duration)
: pt_multiProDisplay(multiProDisplay), pt_align(align), pt_duration(duration * 1000), pt_updateFrequency(duration * 1000) {}

void App::run(void) {
    uint32_t usedDuration = 0;
    while(usedDuration < pt_duration) {
        uint32_t start = millis();
        this->update(usedDuration == 0, this->pt_duration - usedDuration);
        uint32_t updateDuration = millis() - start;
        int32_t sleep = this->pt_updateFrequency - updateDuration;
        // Serial.printf("sleep run: %d\n", sleep);
        if(sleep > 0) {
            delay(sleep);
        }
        usedDuration += millis() - start;
        // Serial.printf("usedDuration run: %d\n", usedDuration);
    }
}

void App::update(bool first, uint32_t remaining_duration) {}

wchar_t App::str2wchar(std::string unit) {
    if(unit == "cardano")           { return CHAR_ADA;}
    else if(unit == "baked-token")  { return CHAR_BAKED;}
    else if(unit == "bitcoin")      { return CHAR_BTC;}
    else if(unit == "dogecoin")     { return CHAR_DOGE;}
    else if(unit == "ethereum")     { return CHAR_ETH;}
    else if(unit == "litecoin")     { return CHAR_LTC;}
    else if(unit == "polkadot")     { return CHAR_POLKADOT;}
    else if(unit == "kusama")       { return CHAR_KSM;}
    else if(unit == "usd")          { return '$';}
    else if(unit == "eur")          { return CHAR_EUR;}
    else if(unit == "gbp")          { return '£';}
    else if(unit == "sats")         { return CHAR_WIDESATOSHI;}
}

ClockApp::ClockApp(MultiProDisplay* multiProDisplay, align_t align, bool showSecond, uint32_t duration, uint32_t updateFrequency)
: App(multiProDisplay, align, duration, updateFrequency) {
    this->m_showSecond = showSecond;
    this->m_align = align;
    this->m_start = millis();
    this->m_lastHour = -1;
    this->m_lastMinute = -1;
    this->m_lastSecond = -1;
}

void ClockApp::update(bool first, uint32_t remaining_duration) {
    std::string timeString;
    uint32_t loopStart = millis();
    RTC_PCF8563 rtc;
    rtc.begin();
    rtc.start();
    DateTime now = rtc.now();
    if(first) {
        this->m_lastHour = -1;
        this->m_lastMinute = -1;
        this->m_lastSecond = -1;
    }
    // If show second is false, and the time is not changed, then sleep until the next minute
    if((!this->m_showSecond) && (now.hour() == this->m_lastHour) && (now.minute() == this->m_lastMinute)) {
        uint32_t sleep = 60 - now.second();
        sleep = sleep > remaining_duration ? remaining_duration : sleep;
        delay(sleep * 1000);
        return;
    }
    if(this->m_showSecond) {
        timeString = str_rjust(std::to_string(now.hour()), 2, '0') + "  " + str_rjust(std::to_string(now.minute()), 2, '0') + "  " + str_rjust(std::to_string(now.second()), 2, '0');
    }
    else {
        timeString = str_rjust(std::to_string(now.hour()), 2, '0') + "  " + str_rjust(std::to_string(now.minute()), 2, '0');
    }
    Serial.printf("Bay gio la: %d:%d:%d\n", now.hour(), now.minute(), now.second());
    if(first) {
        this->pt_multiProDisplay->clearAllDisplay();
        if(this->m_showSecond) {
            this->pt_multiProDisplay->getDisplay(1).render_character(CHAR_WIDECOLON, 0);
            this->pt_multiProDisplay->getDisplay(3).render_character(CHAR_WIDECOLON, 0);
        }
        else {
            this->pt_multiProDisplay->getDisplay(2).render_character(CHAR_WIDECOLON, 0);
        }
        this->pt_multiProDisplay->show();
    }
    this->pt_multiProDisplay->render_string(timeString, true, true);
    if(this->m_showSecond) {
        if(this->m_lastHour != now.hour())      {this->pt_multiProDisplay->getDisplay(0).show();}
        if(this->m_lastMinute != now.minute())  {this->pt_multiProDisplay->getDisplay(2).show();}
        if(this->m_lastSecond != now.second())  {this->pt_multiProDisplay->getDisplay(4).show();}
    }
    else {
        if(this->m_lastHour != now.hour())      {this->pt_multiProDisplay->getDisplay(1).show();}
        if(this->m_lastMinute != now.minute())  {this->pt_multiProDisplay->getDisplay(3).show();}
    }
    uint32_t sleep = 1000 - (millis() - loopStart);
    // Serial.printf("sleep update: %d\n", sleep);
    if(sleep > 0) {
        delay(sleep);
    }
    this->m_lastHour = now.hour();
    this->m_lastMinute = now.minute();
    this->m_lastSecond = now.second();
}

CryptoApp::CryptoApp(MultiProDisplay* multiProDisplay, align_t align, 
std::string crypto, std::string currency, uint8_t decimal, uint32_t duration, uint32_t updateFrequency)
: App(multiProDisplay, align, duration, updateFrequency) {
    this->m_crypto = crypto;
    this->m_currency = currency;
    this->m_decimal = decimal;
}

void CryptoApp::update(bool first, uint32_t remaining_duration) {
    // init URL
    const char* base_url = "https://api.coingecko.com/api/v3/coins/%s/market_chart?vs_currency=%s&days=0";
    char url[128];
    sprintf(url, base_url, this->m_crypto, this->m_currency);
    Serial.printf("url: %s\n", url);
    // get data from URL
    String prices = this->m_request.parseJson(url, "prices", 0, 1);
    Serial.printf("prices: %s\n", String(prices.toInt()));
    this->pt_multiProDisplay->clearAllDisplay();
    this->pt_multiProDisplay->render_string(("  " + std::to_string(prices.toInt()) + "  "), true, true);
    this->pt_multiProDisplay->getDisplay(0).render_character(str2wchar(this->m_currency), 0);
    this->pt_multiProDisplay->getDisplay(4).render_character(str2wchar(this->m_crypto), 0);
    this->pt_multiProDisplay->show();
}

void CryptoApp::setArgs(std::string crypto, std::string currency, uint8_t decimal) {
    this->m_crypto = crypto;
    this->m_currency = currency;
    this->m_decimal = decimal;
}