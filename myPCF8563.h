#ifndef _MY_PCF8563_H_
#define _MY_PCF8563_H_

/**
 * @brief PCF8563 RTC library
 * 
 * origin from https://github.com/Bill2462/PCF8563-Arduino-Library
 * 
 * Add private I2C object
 * Add dec2bcd, bcd2dec function
 * Modify getDateTime, write, read function
 * Replace setHour, setMinute, setSecond, setDay, setMonth, setYear function with setDateTime functions
 * Macro masks for controlling registers
 * 
*/

#include <Arduino.h>
#include <Wire.h>

#define PCF8563_ADDR (0x51)

//now time structure
struct Time {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t weekday;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};

//output frequency
enum output_frequency {

  CLKOUT_32768_Hz,
  CLKOUT_1024_Hz,
  CLKOUT_32_Hz,
  CLKOUT_1_Hz,
};

class PCF8563 {
public:
  void begin(TwoWire &port = Wire); //initialize the chip
  void setDateTime(const Time* dt);

  void enableClkOutput();//enable CLK output
  void disableClkOutput();//disable CLK output
  void setClkOutputFrequency(output_frequency frequency);//set CLK output frequency

  Time getDateTime(void);
  bool checkClockIntegrity();//check clock integrity

private:
  void m_stopClock();//stop the clock
  void m_startClock();//start the clock
  void m_read(uint8_t addr, uint8_t* data, uint8_t size);
  void m_write(uint8_t addr, uint8_t* data, uint8_t size);
  void m_write_OR(uint8_t address, uint8_t data); //write data to the register using OR operations
  void m_write_AND(uint8_t address, uint8_t data); //write data to the register using AND operation
  const uint8_t m_dec2bcd(uint8_t dec);
  uint8_t m_bcd2dec(uint8_t bcd);
  TwoWire *m_i2c;
};
#endif
