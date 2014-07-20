/**
 * @file errors.h
 * @brief This file contains definitions of the symbolic constants that represents 
 *        error codes, as wel the declarations of the functions that belongs to the 
 *        error module.
 */

#ifndef ERRORS_H
#define ERRORS_H

#include "platform.h"

/**
 * \enum ERROR_CODE
 * \brief Specifies the error codes used to specify the wrong behaviour of the functions used 
 *        in the game.
 */
enum ERROR_CODE {
    NO_ERROR      = 0,   //!< Normal behaviour, no error is recieved. 

    POINTER_NULL  = -1,  //!< The parameter of the function is NULL. 
    MALLOC_ERROR  = -2,  //!< This error status is encountered when malloc faild to allocate memory. 

    CARD_NULL     = -3,  //!< The player's card is NULL.
    DECK_NULL     = -4,  //!< The deck of the current player is NULL. 
    PLAYER_NULL   = -5,  //!< The value of the variable that points to a Player is equal to NULL.
    TEAM_NULL     = -6,  //!< The value of the argument that points to a Team is equal to NULL.

    TEAM_FULL     = -7,  //!< There are no free spaces in the current team.
    TEAM_EMPTY    = -8,  //!< An attempt to operate on a team that has no members.

    DUPLICATE     = -9,  //!< There is one more player or card with that name.
    NOT_FOUND     = -10, //!< The item you are operating on was not found.

    ROUND_NULL    = -11, //!< The value of the argument that is supposed to point to a Round is equal to NULL.
    HAND_NULL     = -12, //!< The value of the argument that is supposed to point to a Hand is equal to NULL.

    ILLEGAL_VALUE = -13, //!< The bid's value o the player's id is not a valid one.

    FULL          = -14, //!< There is no place left for one more player or hand.
    ERROR_COMPARE = -15, //!< In this context, the values you are trying to compare are illegal.

    HAND_EMPTY    = -16, //!< The current hand is empty.
    DECK_EMPTY    = -17, //!< Can't distriute cards from an empty deck.

    INSUFFICIENT_PLAYERS  = -18, //!< In this context, the number of players is not big enough.
    LESS_CARDS    = -19, //!< The operation can't be completed due to the insufficient number of cards.

    GAME_NULL     = -20, //!< The value of the argument that should point to a Game is equal to NULL.
    ROUND_EMPTY   = -21, //!< There are no players in a round.
    GAME_EMPTY    = -22, //!< There are no players in a game.

    DUPLICATE_NAME = -23, //!< There is one more player with this name.

    ALLOC_ERROR   = -24  //!< There have been a memory allocation error.
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
EXPORT const char *error(const int error_code);

#ifdef __cplusplus
}
#endif

#endif

