#include "utils.h"

std::string center_string(std::string str) {
  uint8_t capacity = 10;
  uint8_t margin = (capacity - str.length()) / 2; // Calculate the margin on each side
  if (margin < 0) {
    margin = 0; // If the margin is negative, set it to zero
  }
  // Add spaces to the left and right of the string
  for (uint8_t i = 0; i < margin; i++) {
    str = ' ' + str + ' ';
  }
  // If the string is still too short, add a space to the right
    if (str.length() < capacity) {
        str = str + ' ';
    }

  return str; // Return the centered string
}

std::string str_rjust(std::string str, uint8_t len, char c) {
    while(str.length() < len) {
        str = c + str;
    }
    return str;
}

std::string str_ljust(std::string str, uint8_t len, char c) {
    while(str.length() < len) {
        str = str + c;
    }
    return str;
}

std::string str_cjust(std::string str, uint8_t len, char c) {
    while(str.length() < len) {
        str = c + str;
        if(str.length() < len) {
            str = str + c;
        }
    }
    return str;
}

std::string str_align(std::string str, uint8_t len, char c, align_t align) {
    switch(align) {
        case LEFT: return str_ljust(str, len, c); break;
        case CENTER: return str_cjust(str, len, c); break;
        case RIGHT: return str_rjust(str, len, c); break;
        default: Serial.println("Invalid alignment!"); return str_ljust(str, len, c); break;
    }
}

uint8_t number_to_human(uint8_t *number) {
    uint8_t arr[] = {' ', 'k', 'M', 'B', 'T', 'Q'};
    uint8_t index = 0;
    while(*number > 1000) {
        *number /= 1000;
        index++;
    }
    return arr[index];
}