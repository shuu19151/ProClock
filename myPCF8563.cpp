#include "myPCF8563.h"
#include "Arduino.h"

/**
 * @brief Initialize the PCF8563
 * 
 * @param None
 * 
 * @return None
*/
void PCF8563::init() {
  Wire.begin(); //initialize the I2C interface
  write_AND(Control_status_1,~(1<<3)); //clear TESTC bit
  write_AND(CLKOUT_control,~(1<<7)); //clear CLKOUT enable bit
}

/**
 * @brief Start the clock
 * 
 * @param None
 * 
 * @return None
*/
void PCF8563::startClock() {
  write_AND(Control_status_1, ~(1<<5));
}

/**
 * @brief Stop the clock
 * 
 * @param None
 * 
 * @return None
*/
void PCF8563::stopClock() {
  write_OR(Control_status_1, 1<<5);
}

/**
 * @brief Set the year
 * 
 * @param [in] year: Selected year (you can set values 0-99)
 * 
 * @return None
*/
void PCF8563::setYear(uint8_t year) {
  const uint8_t data = ((get_second_number(year))<<4) | (get_first_number(year));
  write(Years, data);
}

/**
 * @brief Set the month
 * 
 * @param [in] month: Selected month (you can set values 1-12)
 * 
 * @return None
*/
void PCF8563::setMonth(uint8_t month) {
  const uint8_t data = ((get_second_number(month))<<4) | (get_first_number(month));
  write(Century_months,data);
}

/**
 * @brief Set the day
 * 
 * @param [in] day: Selected day (you can set values 1-31)
 * 
 * @return None
*/
void PCF8563::setDay(uint8_t day) {
  const uint8_t data = ((get_second_number(day))<<4) | (get_first_number(day));
  write(Days,data);
}

/**
 * @brief Set weekday
 * 
 * @param [in] weekday: Selected day (you can set values 0-6)
 * 
 * @return None
*/
void PCF8563::setWeekday(uint8_t weekday) {
  const uint8_t data = weekday;
  write(Weekdays,weekday);
}

/**
 * @brief Set the hour
 * 
 * @param [in] hour: Selected day (you can set values 0-23)
 * 
 * @return None
*/
void PCF8563::setHour(uint8_t hour) {
  const uint8_t data = ((get_second_number(hour))<<4) | (get_first_number(hour));
  write(Hours,data);
}

/**
 * @brief Set the minute
 * 
 * @param [in] minute: Selected day (you can set values 0-59)
 * 
 * @return None
*/
void PCF8563::setMinute(uint8_t minut) {
  const uint8_t data = ((get_second_number(minut))<<4) | (get_first_number(minut));
  write(Minutes, data);
}

/**
 * @brief Set the second
 * 
 * @param [in] second: Selected day (you can set values 0-59)
 * 
 * @return None
*/
void PCF8563::setSecond(uint8_t second) {
  const uint8_t data = ((get_second_number(second))<<4) | (get_first_number(second));
  write(VL_seconds,data);
}

/**
 * @brief Get the time
 * 
 * @param None
 * 
 * @return Time structure
*/
Time PCF8563::getTime() {
  Time output;

  //read data registers contents
  const uint8_t YEAR    = read(Years);
  const uint8_t MONTH   = read(Century_months);
  const uint8_t DAY     = read(Days);
  const uint8_t WEEKDAY = read(Weekdays);
  const uint8_t HOUR    = read(Hours);
  const uint8_t MINUTE  = read(Minutes);
  const uint8_t SECONDS = read(VL_seconds);

  //convert readed data to numbers using bcd_to_number function).
  output.year    = bcd_to_number((YEAR&0b11110000)>>4,YEAR&0b00001111);
  output.month   = bcd_to_number((MONTH&0b00010000)>>4,MONTH&0b00001111);
  output.day     = bcd_to_number((DAY&0b00110000)>>4,DAY&0b00001111);
  output.weekday = bcd_to_number(0,WEEKDAY&0b00000111);
  output.hour    = bcd_to_number((HOUR&0b00110000)>>4,HOUR&0b00001111);
  output.minute  = bcd_to_number((MINUTE&0b01110000)>>4,MINUTE&0b00001111);
  output.second  = bcd_to_number((SECONDS&0b01110000)>>4,SECONDS&0b00001111);

  return output;
}

/**
 * @brief Check clock integrity
 * 
 * @param None
 * 
 * @return 1 if clock integrity is guaranteed, 0 if not
*/
bool PCF8563::checkClockIntegrity() {
    const uint8_t data = read(VL_seconds); //read the data

    if(data & (1<<7))
    {
      return 0; //if clock integrity is not guaranteed return 0
    }

    else
    {
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
void PCF8563::enableClkOutput()
{
  write_OR(CLKOUT_control,1<<7); //set FE bit in CLKOUT_control register
}

/**
 * @brief Disable CLK OUTPUT
 * 
 * @param None
 * 
 * @return None
*/
void PCF8563::disableClkOutput()
{
  write_AND(CLKOUT_control,~(1<<7)); //clear FE bit in CLKOUT_control register
}

/**
 * @brief Set CLK OUTPUT frequency
 * 
 * @param [in] frequency: Selected frequency
 * 
 * @return None
*/
void PCF8563::setClkOutputFrequency(output_frequency frequency)
{
  switch(frequency)
  {
    case CLKOUT_32768_Hz:
      write_AND(CLKOUT_control,~((1<<0) | (1<<1)));
      break;

    case CLKOUT_1024_Hz:
      write_AND(CLKOUT_control,~(1<<1));
      write_OR(CLKOUT_control,1<<0);
      break;

    case CLKOUT_32_Hz:
      write_AND(CLKOUT_control,~(1<<0));
      write_OR(CLKOUT_control,1<<1);
      break;

    case CLKOUT_1_Hz:
      write_OR(CLKOUT_control,(1<<0) | (1<<1));
      break;
  }
}

/**
 * @brief Read one byte from selected register
 * 
 * @param [in] address: Register address
 * 
 * @return One byte of data
*/
uint8_t PCF8563::read(uint8_t address)
{
  Wire.beginTransmission(PCF8563_address); //begin transmission
  Wire.write(address); //inform chip what register we want to read
  Wire.endTransmission();
  Wire.requestFrom(PCF8563_address,1); //request one byte from the chip
  uint8_t data = Wire.read(); //read the data
  return data;
}

/**
 * @brief Convert two digits to one number
 * 
 * @param [in] first: First digit
 * @param [in] second: Second digit
 * 
 * @return One number
*/
unsigned char PCF8563::bcd_to_number(uint8_t first, uint8_t second) {
  unsigned char output;
  output = first*10;
  output = output + second;
  return output;
}

/**
 * @brief Get ten’s place digit of the number
 * 
 * @param [in] number: Selected number
 * 
 * @return Digit
*/
uint8_t PCF8563::get_first_number(unsigned short number)
{
  uint8_t output = number%10;
  return output;
}

/**
 * @brief Get unit place digit of the number
 * 
 * @param [in] number: Selected number
 * 
 * @return Digit
*/
uint8_t  PCF8563::get_second_number(unsigned short number)
{
  uint8_t output = number/10;
  return output;
}

/**
 * @brief Write one byte of data to the register
 * 
 * @param [in] address: Register address
 * @param [in] data: One byte of data
 * 
 * @return None
*/
void PCF8563::write(uint8_t address, uint8_t data)
{
  Wire.beginTransmission(PCF8563_address);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

/**
 * @brief Write data to the register using OR operations
 * 
 * @param [in] address: Register address
 * @param [in] data: One byte of data that we want to put in the register
 * 
 * @return None
*/
void PCF8563::write_OR(uint8_t address, uint8_t data)
{
  uint8_t c = read(address);
  c = c | data;
  write(address,c);
}

/**
 * @brief Write data to the register using AND operation
 * 
 * @param [in] address: Register address
 * @param [in] data: One byte of data that we want to put in the register
 * 
 * @return None
*/
void PCF8563::write_AND(uint8_t address, uint8_t data)
{
  uint8_t c = read(address);
  c = c & data;
  write(address,c);
}
