#include "myPCF8563.h"
#include "Arduino.h"

#define PCF8563_DATA_SIZE     (7)

#define REG_CONTROL_STATUS_1  (0x00)
#define REG_CONTROL_STATUS_2  (0x01)
#define REG_VL_SECONDS        (0x02)
#define REG_MINUTES           (0x03)
#define REG_HOURS             (0x04)
#define REG_DAYS              (0x05)
#define REG_WEEKDAYS          (0x06)
#define REG_CENTURY_MONTHS    (0x07)
#define REG_YEARS             (0x08)
#define REG_MINUTE_ALARM      (0x09)
#define REG_HOUR_ALARM        (0x0A)
#define REG_DAY_ALARM         (0x0B)
#define REG_WEEKDAY_ALARM     (0x0C)
#define REG_CLKOUT_CONTROL    (0x0D)
#define REG_TIMER_CONTROL     (0x0E)
#define REG_TIMER             (0x0F)

#define CENTURY_FLAG          (7)
#define VL_FLAG               (7)

#define MASK_VL_SECONDS       (0x80) //0b10000000
#define MASK_MINUTE           (0x7f) //0b01111111
#define MASK_HOUR             (0x3f) //0b00111111
#define MASK_DAY              (0x3f) //0b00111111
#define MASK_WEEKDAY          (0x07) //0b00000111
#define MASK_MONTH            (0x1f) //0b00011111
#define MASK_CENTURY          (0x80) //0b10000000

/**
 * @brief Initialize the PCF8563
 * 
 * @param None
 * 
 * @return None
*/
void PCF8563::begin(TwoWire &port) {
  this->m_i2c = &port;
  this->m_i2c->begin();
  this->m_write_AND(REG_CONTROL_STATUS_1, ~(1<<3)); //clear TESTC bit
  this->m_write_AND(REG_CLKOUT_CONTROL, ~(1<<7)); //clear CLKOUT enable bit
  this->m_startClock(); //start the clock
}

/**
 * @brief Start the clock
 * 
 * @param None
 * 
 * @return None
*/
void PCF8563::m_startClock() {
  m_write_AND(REG_CONTROL_STATUS_1, ~(1<<5));
}

/**
 * @brief Stop the clock
 * 
 * @param None
 * 
 * @return None
*/
void PCF8563::m_stopClock() {
  m_write_OR(REG_CONTROL_STATUS_1, 1<<5);
}

/**
 * @brief Set the RTC date/time register with Time structure
 * 
 * @param [in] dt: Time structure
 * 
 * @return None
*/
void PCF8563::setDateTime(const Time* dt) {
  uint8_t data[7] = {0};
  data[0] = m_dec2bcd(dt->second) & ~MASK_VL_SECONDS;
  data[1] = m_dec2bcd(dt->minute);
  data[2] = m_dec2bcd(dt->hour);
  data[3] = m_dec2bcd(dt->day);
  data[4] = m_dec2bcd(dt->weekday);
  data[5] = m_dec2bcd(dt->month);
  data[6] = m_dec2bcd(dt->year % 100);
  (2000 % dt->year == 2000) ? data[5] &= ~MASK_CENTURY : data[5] |= MASK_CENTURY; // if year >= 2000, set century bit to 0 otherwise set to 1
  this->m_stopClock();
  this->m_write(REG_VL_SECONDS, data, 7);
  this->m_startClock();
}

/**
 * @brief Set the RTC date/time register with Time structure
 * 
 * @param [in] dt: Time structure
 * 
 * @return Time structure
*/
Time PCF8563::getDateTime(void) {
  Time datetime = {0};
  uint8_t rawData[7] = {0};
  this->m_read(REG_VL_SECONDS, rawData, PCF8563_DATA_SIZE);
  /*- 0..59 -*/
  datetime.second = this->m_bcd2dec(rawData[0] & ~MASK_VL_SECONDS);
  /*- 0..59 -*/
  datetime.minute = this->m_bcd2dec(rawData[1] & MASK_MINUTE);
  /*- 0..23 -*/
  datetime.hour = this->m_bcd2dec(rawData[2] & MASK_HOUR);
  /*- 1..31 -*/
  datetime.day = this->m_bcd2dec(rawData[3] & MASK_DAY);
  /*- 0..6 -*/
  datetime.weekday = this->m_bcd2dec(rawData[4] & MASK_WEEKDAY);
  /*- 1..12 -*/
  datetime.month = this->m_bcd2dec(rawData[5] & MASK_MONTH);
  /*- 0..99 -*/
  /* year value is 00 to 99, xx
  * with the highest bit of month = century
  * 0 = 20xx, 1 = 19xx
  */
  datetime.year = this->m_bcd2dec(rawData[6]) + (rawData[5] & MASK_CENTURY ? 1900 : 2000);

  this->m_i2c->endTransmission();
  return datetime;
}

/**
 * @brief Convert from decimal to BCD number
 * 
 * @param [in] dec: 8-bit decimal
 * 
 * @return 8-bit BCD value
*/
const uint8_t PCF8563::m_dec2bcd(uint8_t dec) {
  return ((dec / 10) << 4) | (dec % 10);
}

/**
 * @brief Convert from BCD number to decimal value
 * 
 * @param [in] bcd: 8-bit BCD number
 * 
 * @return 8-bit decimal value
*/
uint8_t PCF8563::m_bcd2dec(uint8_t bcd) {
  return ((bcd >> 4) * 10) + (bcd & 0x0f);
}

/**
 * @brief Check clock integrity
 * 
 * @param None
 * 
 * @return 1 if clock integrity is guaranteed, 0 if not
*/
bool PCF8563::checkClockIntegrity() {
    uint8_t data;
    this->m_read(REG_VL_SECONDS, &data, 1); //read the data

    if(data & (1<<7)) {
      return 0; //if clock integrity is not guaranteed return 0
    }

    else {
      return 1; //otherwise return 1
    }
}

/**
 * @brief Enable CLK OUTPUT
 * 
 * @param None
 * 
 * @return None
*/
void PCF8563::enableClkOutput() {
  this->m_write_OR(REG_CLKOUT_CONTROL, 1<<7); //set FE bit in REG_CLKOUT_CONTROL register
}

/**
 * @brief Disable CLK OUTPUT
 * 
 * @param None
 * 
 * @return None
*/
void PCF8563::disableClkOutput() {
  this->m_write_AND(REG_CLKOUT_CONTROL, ~(1<<7)); //clear FE bit in REG_CLKOUT_CONTROL register
}

/**
 * @brief Set CLK OUTPUT frequency
 * 
 * @param [in] frequency: Selected frequency
 * 
 * @return None
*/
void PCF8563::setClkOutputFrequency(output_frequency frequency) {
  switch(frequency) {
    case CLKOUT_32768_Hz:
      this->m_write_AND(REG_CLKOUT_CONTROL,~((1<<0) | (1<<1)));
      break;

    case CLKOUT_1024_Hz:
      this->m_write_AND(REG_CLKOUT_CONTROL,~(1<<1));
      this->m_write_OR(REG_CLKOUT_CONTROL,1<<0);
      break;

    case CLKOUT_32_Hz:
      this->m_write_AND(REG_CLKOUT_CONTROL,~(1<<0));
      this->m_write_OR(REG_CLKOUT_CONTROL,1<<1);
      break;

    case CLKOUT_1_Hz:
      this->m_write_OR(REG_CLKOUT_CONTROL,(1<<0) | (1<<1));
      break;
  }
}

/**
 * @brief Read one byte from selected register
 * 
 * @param [in] addr: Register address
 * 
 * @return None
*/
void PCF8563::m_read(uint8_t addr, uint8_t* data, uint8_t size) {
  this->m_i2c->beginTransmission(PCF8563_ADDR);
  this->m_i2c->write(addr);
  this->m_i2c->endTransmission();
  this->m_i2c->requestFrom((int)PCF8563_ADDR, (int)size); //request one byte from the chip
  for(uint8_t i = 0; i < size; i++) {
    *(data++) = this->m_i2c->read();
  }
}

/**
 * @brief Write data to the register
 * 
 * @param [in] addr: Register address
 * @param [in] data: Data to send
 * @param [in] size: Size of data
 * 
 * @return None
*/
void PCF8563::m_write(uint8_t addr, uint8_t *data, uint8_t size) {
  this->m_i2c->beginTransmission(PCF8563_ADDR);
  this->m_i2c->write(addr);
  for(uint8_t i = 0; i < size; i++) {
    this->m_i2c->write(data[i]);
  }
  this->m_i2c->endTransmission();
}

/**
 * @brief Write data to the register using OR operations
 * 
 * @param [in] address: Register address
 * @param [in] data: One byte of data that we want to put in the register
 * 
 * @return None
*/
void PCF8563::m_write_OR(uint8_t address, uint8_t data) {
  uint8_t c;
  this->m_read(address, &c, 1);
  c = c | data;
  this->m_write(address, &c, 1);
}

/**
 * @brief Write data to the register using AND operation
 * 
 * @param [in] address: Register address
 * @param [in] data: One byte of data that we want to put in the register
 * 
 * @return None
*/
void PCF8563::m_write_AND(uint8_t address, uint8_t data) {
  uint8_t c;
  this->m_read(address, &c, 1);
  c = c & data;
  this->m_write(address, &c, 1);
}
