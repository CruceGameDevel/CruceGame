#ifndef ERRORS_H
#define ERRORS_H

#include "platform.h"

enum ERROR_CODE {
    NO_ERROR      = 0,

    POINTER_NULL  = -1,
    MALLOC_ERROR  = -2,

    CARD_NULL     = -3,
    DECK_NULL     = -4,
    PLAYER_NULL   = -5,
    TEAM_NULL     = -6,

    TEAM_FULL     = -7,
    TEAM_EMPTY    = -8,

    DUPLICATE     = -9,
    NOT_FOUND     = -10,

    ROUND_NULL    = -11,
    HAND_NULL     = -12,

    ILLEGAL_VALUE = -13,

    FULL          = -14,
    ERROR_COMPARE = -15,

    HAND_EMPTY    = -16,
    DECK_EMPTY    = -17,

    INSUFFICIENT_PLAYERS  = -18,
    LESS_CARDS    = -19,

    GAME_NULL     = -20,
    ROUND_EMPTY   = -21,
    GAME_EMPTY    = -22
};

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief Returns the error code text of the error code given.
*
* @param error_code The error code value as integer type.
*
* @return Pointer to a const char representing the text of the error code.
*/
EXPORT const char *error(int error_code);

#ifdef __cplusplus
}
#endif

#endif

