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
    NO_ERROR                   = 0,   //!< Normal behaviour, no error reported.

    POINTER_NULL               = -1,  //!< One parameter is NULL.
    MALLOC_ERROR               = -2,  //!< Malloc faild to allocate memory.

    CARD_NULL                  = -3,  //!< The player's card is NULL.
    DECK_NULL                  = -4,  //!< The deck of the current player
                                      //   is NULL.
    PLAYER_NULL                = -5,  //!< The value of the variable that
                                      //   points to a Player is equal to NULL.
    TEAM_NULL                  = -6,  //!< The value of the argument that
                                      //   points to a Team is equal to NULL.

    TEAM_FULL                  = -7,  //!< There are no free slots in the
                                      //   current team.
    TEAM_EMPTY                 = -8,  //!< An attempt to operate on a team that
                                      //   has no members.

    DUPLICATE                  = -9,  //!< There is one more player or card
                                      //   with that name.
    NOT_FOUND                  = -10, //!< The requested item was not found.

    ROUND_NULL                 = -11, //!< The Round pointer is NULL.
    HAND_NULL                  = -12, //!< The Hand pointer is NULL.

    ILLEGAL_VALUE              = -13, //!< The bid value is not valid.

    FULL                       = -14, //!< There is no slot left for one more
                                      //   player or hand.
    ERROR_COMPARE              = -15, //!< In this context, the comparison
                                      //   is illegal.

    HAND_EMPTY                 = -16, //!< The current hand is empty.
    DECK_EMPTY                 = -17, //!< Can't distriute cards from an empty
                                      //   deck.

    INSUFFICIENT_PLAYERS       = -18, //!< In this context, the number of
                                      //   players is not big enough.
    LESS_CARDS                 = -19, //!< The operation can't be completed due
                                      //   to the insufficient number of cards.

    GAME_NULL                  = -20, //!< The Game pointer is NULL.
    ROUND_EMPTY                = -21, //!< There are no players in a round.
    GAME_EMPTY                 = -22, //!< There are no players in a game.

    DUPLICATE_NAME             = -23, //!< There is one more player with this
                                      //   name.

    ALLOC_ERROR                = -24, //!< There has been a memory allocation
                                      //   error.

    CONNECTION_IN_USE          = -25, //!< There is already an active
                                      //   connection.
    INVALID_HOSTNAME           = -26, //!< The hostname is invalid.
    CONNECTING_ERROR           = -27, //!< The connection attempt failed.

    UNINITIALIZED_CONNECTION   = -28, //!< Trying to perform a netowrk operation
                                      //   on an unitialized connection.

    WRITING_ERROR              = -29, //!< The internal call to `write` in
                                      //   \ref network_send returns an error
                                      //   code.
    READING_ERROR              = -30, //!< The internal call to `read` in
                                      //   \ref network_read returns an error
                                      //   code.
    PARAMETER_OUT_OF_RANGE     = -31, //!< The parameter's value is out of
                                      //   range.
    MESSAGE_TOO_LONG           = -32, //!< The message is too long.
    DISCONNECT_ERROR           = -33, //!< The disconnect attempt failed.
    LEAVE_ROOM_ERROR           = -34, //!< The attmept to leave the room failed.
    SEND_LOBBY_MESSAGE_FAILURE = -35  //!< Sending message to lobby failed.
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

