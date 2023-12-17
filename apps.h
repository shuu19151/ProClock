#include "display.h"
#include "datetime.h"

class App {
    protected:
    MultiProDisplay* pt_multiProDisplay;
    align_t pt_align;
    uint32_t pt_duration;
    uint32_t pt_updateFrequency;
    public:
    App(MultiProDisplay* multiProDisplay, align_t align = RIGHT, uint32_t duration = 30, uint32_t updateFrequency = -1);
    void run(void);
    virtual void update(bool first, uint32_t remaining_duration) = 0;
};

class ClockApp : public App {
    private:
    bool m_showSecond;
    String m_align;
    uint32_t m_start;
    uint8_t m_lastHour;
    uint8_t m_lastMinute;
    uint8_t m_lastSecond;
    public:
    ClockApp(MultiProDisplay* multiProDisplay, align_t align = CENTER, bool showSecond = true, uint32_t duration = 30, uint32_t updateFrequency = -1);
    void update(bool first, uint32_t remaining_duration) override;
};

class CryptoApp : public App {
};