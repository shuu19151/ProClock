#line 1 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\display.h"
#include <Adafruit_SSD1306.h>
#include <iostream>
#include "TCA9548.h"
#include "fonts.h"
#include "utils.h"

class ProSSD1306 {
    private:
    Adafruit_SSD1306* m_display;
    public:
    uint8_t char_width;
    bool begin(void);
    ProSSD1306(Adafruit_SSD1306* display);
    void render_character(wchar_t character, uint8_t x_offset);
    void render_symbol(const uint8_t* symbol, uint8_t x_offset);
    void display();
    void clearBuffer(void);
    void clearDisplay(void);
};

class MultiProDisplay {
    private:
    Adafruit_SSD1306 m_baseDisplay;
    TCA9548 m_multiplexer;
    ProSSD1306 m_displays[5];
    const uint8_t m_num_displays = 5;
    const uint8_t m_first_character_x_offset = 0;
    const uint8_t m_second_character_x_offset = 79;
    public:
    MultiProDisplay(uint8_t addr, uint8_t num_displays, uint8_t display_width, uint8_t display_height);
    bool begin(void);
    void render_string(String string, bool center, bool emty_as_transparent);
    ProSSD1306& getDisplay(uint8_t index);
    void render_character(void);
    void clearBuffer(void);
    void clear(void);
    void show(void);
};