#ifndef _UTILS_H_
#define _UTILS_H_

#include <Arduino.h>

#define SET_BIT(BF, N) BF |= ((uint8_t)0b00000001 << N)
#define CLR_BIT(BF, N) BF &= ~((uint8_t)0b00000001 << N)
#define IS_BIT_SET(BF, N) ((BF >> N) & 0b00000001)

#define SIZE_OF_8BIT_FLAG (8)

typedef enum align_t {
    LEFT,
    CENTER,
    RIGHT
} align_t;

std::string center_string(std::string inputString);
std::string str_rjust(std::string str, uint8_t len, char c=' ');
std::string str_ljust(std::string str, uint8_t len, char c=' ');
std::string str_cjust(std::string str, uint8_t len, char c=' ');

/**
 * @brief Align string to the left, center or right
 * 
 * @param str std::string to align
 * @param len Length of the string
 * @param align 0 = left, 1 = center, 2 = right
*/
std::string str_align(std::string str, uint8_t len, char c=' ', align_t align=RIGHT);

#endif