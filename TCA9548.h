#ifndef TCA9548_H
#define TCA9548_H

#include <Arduino.h>
#include <Wire.h>

class TCA9548 {
    private:
    uint8_t m_address;
    public:
    TCA9548(uint8_t address);

    bool begin(void);
    void selectChannel(uint8_t bus);
};

#endif // TCA9548_H