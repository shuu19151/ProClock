#line 1 "A:\\Tri\\Laptrinh\\Arduino\\ProClock\\fonts.h"
#include <stdint.h>
#include "symbols.h"
#include <cstdint>

const wchar_t CHAR_ADA = L'\ue000';
const wchar_t CHAR_BAKED = L'\ue001';
const wchar_t CHAR_BTC = L'\ue002';
const wchar_t CHAR_DOGE = L'\ue003';
const wchar_t CHAR_ETH = L'\ue004';
const wchar_t CHAR_LTC = L'\ue005';
const wchar_t CHAR_POLKADOT = L'\ue006';
const wchar_t CHAR_WIDECOLON = L'\ue007';
const wchar_t CHAR_SATOSHI = L'\ue008';
const wchar_t CHAR_KSM = L'\ue009';
const wchar_t CHAR_CHAIN = L'\ue00A';
const wchar_t CHAR_WIDESATOSHI = L'\ue00B';
const wchar_t CHAR_MONEY_BAG = L'\ue00C';
const wchar_t CHAR_HALVING = L'\ue00D';
const wchar_t CHAR_WIFI = L'\ue00E';
const wchar_t CHAR_CHECK = L'\ue00F';
const wchar_t CHAR_CROSS = L'\ue010';
const wchar_t CHAR_DELTA = L'\ue011';
const wchar_t CHAR_SIGMA = L'\ue012';
const wchar_t CHAR_MOSCOW = L'\ue013';
const wchar_t CHAR_MONEY_BAG_SAT = L'\ue014';

const uint8_t* get_symbol_for_character(wchar_t c, uint8_t *width);