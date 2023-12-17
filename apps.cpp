#include "apps.h"

App::App(MultiProDisplay* multiProDisplay, align_t align, uint32_t duration, uint32_t updateFrequency)
: pt_multiProDisplay(multiProDisplay), pt_align(align), pt_duration(duration * 1000) {
    this->pt_updateFrequency = updateFrequency == -1 ? this->pt_duration : updateFrequency * 1000;
}

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

void App::update(bool first, uint32_t remaining_duration) {
    // Do nothing
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
    String timeString;
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
        timeString = str_rjust(String(now.hour()), 2, '0') + "  " + str_rjust(String(now.minute()), 2, '0') + "  " + str_rjust(String(now.second()), 2, '0');
    }
    else {
        timeString = str_rjust(String(now.hour()), 2, '0') + "  " + str_rjust(String(now.minute()), 2, '0');
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
