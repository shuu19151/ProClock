#line 1 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\applications.h"
#include "display.h"
#include "datetime.h"

class App {
    private:
    MultiProDisplay m_multiProDisplay;
    bool m_align;
    uint32_t m_duration;
    uint32_t m_updateFrequency;
    public:
    App(MultiProDisplay multiProDisplay, bool align, uint32_t duration, uint32_t updateFrequency);
    virtual void run(void) = 0;
    void update(bool first, uint32_t remaining_duration);
};

class ClockApp : public App {
    private:
    ProPCF8563 m_rtc;
    public:
    ClockApp(void);
    void run(void);
};

class CryptoApp : public App {
    private:
    ProPCF8563 m_rtc;
    public:
    CryptoApp(void);
    void run(void);
};