#line 1 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\utils.h"
#include <Arduino.h>

typedef enum align_t {
    LEFT,
    CENTER,
    RIGHT
} align_t;

String center_string(String inputString);
String str_rjust(String str, uint8_t len, char c=' ');
String str_ljust(String str, uint8_t len, char c=' ');
String str_cjust(String str, uint8_t len, char c=' ');

/**
 * @brief Align string to the left, center or right
 * 
 * @param str String to align
 * @param len Length of the string
 * @param align 0 = left, 1 = center, 2 = right
*/
String str_align(String str, uint8_t len, char c=' ', align_t align=RIGHT);
uint8_t number_to_human(uint8_t *number);