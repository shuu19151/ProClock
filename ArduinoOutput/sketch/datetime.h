#line 1 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\datetime.h"
#include "RTClib.h"

class ProPCF8563 {
    private:
    RTC_PCF8563 m_rtc;
    public:
    ProPCF8563(void);
    void begin(void);
    DateTime now(void);
};