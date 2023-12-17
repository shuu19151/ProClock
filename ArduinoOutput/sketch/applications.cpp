#line 1 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\applications.cpp"
#include "applications.h"

App::App(MultiProDisplay multiProDisplay, bool align, uint32_t duration, uint32_t updateFrequency)
: m_multiProDisplay(multiProDisplay), m_align(align), m_duration(duration) {
    if(updateFrequency > 0) {
        m_updateFrequency = updateFrequency;
    } else {
        m_updateFrequency = this->m_duration;
    }
}

void App::run(void) {
    uint32_t usedDuration = 0;
    while(usedDuration < m_duration) {
        uint32_t start = millis();
        uint32_t updateDuration = millis() - start;
        uint32_t sleep = this->m_updateFrequency - updateDuration;
        if(sleep > 0) {
            delay(sleep);
        }
        usedDuration += millis() - start;
    }
}