#line 1 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\utils.h"
#include <Arduino.h>

String center_string(String inputString);
String str_rjust(String str, uint8_t len);
String str_ljust(String str, uint8_t len);
String str_cjust(String str, uint8_t len);

/**
 * @brief Align string to the left, center or right
 * 
 * @param str String to align
 * @param len Length of the string
 * @param align 0 = left, 1 = center, 2 = right
*/
String str_align(String str, uint8_t len, uint8_t align);
uint8_t number_to_human(uint8_t *number);