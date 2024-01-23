#include "fonts.h"

const uint8_t* get_symbol_for_character(wchar_t c, uint8_t *width) {
    switch(c) {
        // ASCII
        case '0': *width = 48;                   return ZERO;        break;
        case '1': *width = 48;                   return ONE;         break;
        case '2': *width = 48;                   return TWO;         break;
        case '3': *width = 48;                   return THREE;       break;
        case '4': *width = 48;                   return FOUR;        break;
        case '5': *width = 48;                   return FIVE;        break;
        case '6': *width = 48;                   return SIX;         break;
        case '7': *width = 48;                   return SEVEN;       break;
        case '8': *width = 48;                   return EIGHT;       break;
        case '9': *width = 48;                   return NINE;        break;
        case 'A': *width = 48;                   return A;           break;
        case 'B': *width = 48;                   return B;           break;
        case 'C': *width = 48;                   return C;           break;
        case 'D': *width = 48;                   return D;           break;
        case 'E': *width = 48;                   return E;           break;
        case 'F': *width = 48;                   return F;           break;
        case 'G': *width = 48;                   return G;           break;
        case 'H': *width = 48;                   return H;           break;
        case 'I': *width = 48;                   return I;           break;
        case 'J': *width = 48;                   return J;           break;
        case 'K': *width = 48;                   return K;           break;
        case 'L': *width = 48;                   return L;           break;
        case 'M': *width = 48;                   return M;           break;
        case 'N': *width = 48;                   return N;           break;
        case 'O': *width = 48;                   return O;           break;
        case 'P': *width = 48;                   return P;           break;
        case 'Q': *width = 48;                   return Q;           break;
        case 'R': *width = 48;                   return R;           break;
        case 'S': *width = 48;                   return S;           break;
        case 'T': *width = 48;                   return T;           break;
        case 'U': *width = 48;                   return U;           break;
        case 'V': *width = 48;                   return V;           break;
        case 'W': *width = 48;                   return W;           break;
        case 'X': *width = 48;                   return X;           break;
        case 'Y': *width = 48;                   return Y;           break;
        case 'Z': *width = 48;                   return Z;           break;
        case 'a': *width = 48;                   return A;           break;
        case 'b': *width = 48;                   return B;           break;
        case 'c': *width = 48;                   return C;           break;
        case 'd': *width = 48;                   return D;           break;
        case 'e': *width = 48;                   return E;           break;
        case 'f': *width = 48;                   return F;           break;
        case 'g': *width = 48;                   return G;           break;
        case 'h': *width = 48;                   return H;           break;
        case 'i': *width = 48;                   return I;           break;
        case 'j': *width = 48;                   return J;           break;
        case 'k': *width = 48;                   return K;           break;
        case 'l': *width = 48;                   return L;           break;
        case 'm': *width = 48;                   return M;           break;
        case 'n': *width = 48;                   return N;           break;
        case 'o': *width = 48;                   return O;           break;
        case 'p': *width = 48;                   return P;           break;
        case 'q': *width = 48;                   return Q;           break;  
        case 'r': *width = 48;                   return R;           break;
        case 's': *width = 48;                   return S;           break;
        case 't': *width = 48;                   return T;           break;
        case 'u': *width = 48;                   return U;           break;
        case 'v': *width = 48;                   return V;           break;
        case 'w': *width = 48;                   return W;           break;
        case 'x': *width = 48;                   return X;           break;
        case 'y': *width = 48;                   return Y;           break;
        case 'z': *width = 48;                   return Z;           break;
        case '.': *width = 48;                   return DOT;         break;
        case '$': *width = 48;                   return DOLLAR;      break;
        case '£': *width = 48;                   return GBP;         break;
        case ':': *width = 48;                   return COLON;       break;
        case ' ': *width = 48;                   return EMPTY;       break;
        case CHAR_EUR: *width = 48;              return EUR;         break;

        // // Crypto logos
        case CHAR_ADA: *width = 128;             return ADA;        break;
        case CHAR_BAKED: *width = 128;           return BAKED;      break;
        case CHAR_BTC: *width = 128;             return BTC;        break;
        case CHAR_DOGE: *width = 128;            return DOGE;       break;
        case CHAR_ETH: *width = 128;             return ETH;        break;
        case CHAR_LTC: *width = 128;             return LTC;        break;
        case CHAR_POLKADOT: *width = 128;        return POLKADOT;   break;
        
        // // Other logos, characters
        case CHAR_WIDECOLON: *width = 128;       return WIDECOLON;       break;
        case CHAR_SATOSHI: *width = 128;         return SATOSHI;         break;
        case CHAR_KSM: *width = 128;             return KSM;             break;
        case CHAR_CHAIN: *width = 128;           return CHAIN;           break;
        case CHAR_WIDESATOSHI: *width = 128;     return WIDESATOSHI;     break;
        case CHAR_MONEY_BAG: *width = 128;       return MONEY_BAG;       break;
        case CHAR_HALVING: *width = 128;         return HALVING;         break;
        case CHAR_WIFI: *width = 128;            return WIFI;            break;
        case CHAR_CHECK: *width = 128;           return CHECK;           break;
        case CHAR_CROSS: *width = 128;           return CROSS;           break;
        case CHAR_DELTA: *width = 128;           return DELTA;           break;
        case CHAR_SIGMA: *width = 128;           return SIGMA;           break;
        case CHAR_MOSCOW: *width = 128;          return MOSCOW;          break;
        case CHAR_MONEY_BAG_SAT: *width = 128;   return MONEY_BAG_SAT;   break;

        default: *width = 48;                    return EMPTY;           break;
    }
}