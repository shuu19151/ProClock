#line 1 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\utils.cpp"
#include "utils.h"

String center_string(String str) {
  uint8_t capacity = 10;
  uint8_t margin = (capacity - str.length()) / 2; // Calculate the margin on each side
  if (margin < 0) {
    margin = 0; // If the margin is negative, set it to zero
  }
  // Add spaces to the left and right of the string
  for (uint8_t i = 0; i < margin; i++) {
    str = String(' ') + str + String(' ');
  }
  // If the string is still too short, add a space to the right
    if (str.length() < capacity) {
        str = str + String(' ');
    }

  return str; // Return the centered string
}

String str_rjust(String str, uint8_t len, char c) {
    while(str.length() < len) {
        str = String(c) + str;
    }
    return str;
}

String str_ljust(String str, uint8_t len, char c) {
    while(str.length() < len) {
        str = str + String(c);
    }
    return str;
}

String str_cjust(String str, uint8_t len, char c) {
    while(str.length() < len) {
        str = String(c) + str;
        if(str.length() < len) {
            str = str + String(c);
        }
    }
    return str;
}

String str_align(String str, uint8_t len, char c, align_t align) {
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