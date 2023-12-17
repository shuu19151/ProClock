#include "display.h"

ProSSD1306::ProSSD1306(Adafruit_SSD1306* display = nullptr)
: m_display(display) {}

void ProSSD1306::render_character(wchar_t character, uint8_t x_offset) {
    render_symbol(get_symbol_for_character(character, &(char_width)), x_offset);
}

void ProSSD1306::render_symbol(const uint8_t* symbol, uint8_t x_offset) {
    uint8_t w = this->char_width;
    this->m_display->drawBitmap(x_offset, 0, symbol, w, 64, 1);
}

bool ProSSD1306::begin(void) {
    bool err = false;
    err = this->m_display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    this->m_display->clearDisplay();
    return err;
}

void ProSSD1306::clearBuffer(void) {
    this->m_display->clearDisplay();
}

void ProSSD1306::show(void) {
    this->m_display->display();
}

/**
 * @brief Construct a new Display Group :: Display Group object
 * 
 * @param addr The address of the multiplexer
 * @param num_displays The number of displays in the group
 * @param display_width The width of the display
 * @param display_height The height of the display
 *
*/
MultiProDisplay::MultiProDisplay(uint8_t addr, uint8_t num_displays, uint8_t display_width, uint8_t display_height)
: m_multiplexer(addr), m_num_displays(num_displays) {
    for (uint8_t i = 0; i < num_displays; i++) {
        this->m_baseDisplays[i] = Adafruit_SSD1306(display_width, display_height, &Wire, -1);
    }
    for (uint8_t i = 0; i < num_displays; i++) {
        this->m_displays[i] = ProSSD1306(&(this->m_baseDisplays[i]));
    }
}

bool MultiProDisplay::begin(void) {
    uint8_t err=0;
    this->m_multiplexer.begin();
    for (uint8_t i = 0; i < this->m_num_displays; i++) {
        this->m_multiplexer.selectChannel(i);
        if(!(this->m_displays[i].begin())) {
            err++;
        }
    }
    return err;
}

void MultiProDisplay::render_string(String string, bool center, bool empty_as_transparent=true) {
    if(center) {
        string = center_string(string);
    }
    for (uint8_t display_index = 0; display_index < m_num_displays; display_index++) {
        this->m_multiplexer.selectChannel(display_index);
        this->m_displays[display_index].clearBuffer();
        for (uint8_t char_index = 0; char_index < 2; char_index++) {
            uint8_t i = display_index * 2 + char_index; // the index of the character in the string
            if (i >= string.length()) {
                break; // no more characters to display
            }
            if (string[i] == ' ' && empty_as_transparent) {
                continue; // skip spaces if empty_as_transparent is true
            }
            if (char_index == 0) {
                this->m_displays[display_index].render_character(string[i], this->m_first_character_x_offset);
            } else {
                this->m_displays[display_index].render_character(string[i], this->m_second_character_x_offset);
            }
        }
    }
}

ProSSD1306& MultiProDisplay::getDisplay(uint8_t index) {
    this->m_multiplexer.selectChannel(index);
    return this->m_displays[index];
}

void MultiProDisplay::clearAllDisplay(void) {
    for (uint8_t i = 0; i < this->m_num_displays; i++) {
        this->m_multiplexer.selectChannel(i);
        this->m_displays[i].clearBuffer();
        this->m_displays[i].show();
    }
}

void MultiProDisplay::show(void) {
    for (uint8_t i = 0; i < this->m_num_displays; i++) {
        this->m_multiplexer.selectChannel(i);
        this->m_displays[i].show();
    }
}
