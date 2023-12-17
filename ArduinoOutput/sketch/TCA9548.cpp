#line 1 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\TCA9548.cpp"
#include "TCA9548.h"

TCA9548::TCA9548(uint8_t address)
: m_address(address) {}

bool TCA9548::begin(void) {
    return Wire.begin();
}

void TCA9548::selectChannel(uint8_t bus) {
    Wire.beginTransmission(m_address);
    Wire.write(1 << bus);
    Wire.endTransmission();
}