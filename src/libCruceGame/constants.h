#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @brief The maximum number of cards in a player's hand.
 */
#define MAX_CARDS 8

/**
 * @brief The maximum number of players in a team.
 */
#define MAX_TEAM_PLAYERS 2

/**
 * @brief Deck size
 */
#define DECK_SIZE 24

/**
 * @brief Minimum number of swaps performed by deckShuffle
 */
#define SWAP_MIN 100

/**
 * @brief Maximum number of swaps performed by deckSuffle
 */
#define SWAP_MAX 200

/**
 * @brief Maximum number of hands in a round.
 */
#define MAX_HANDS 8

/**
 * @brief Maximum number of players in a game
 */
#define MAX_GAME_PLAYERS 4

/** 
* @brief MAximum number of teams in a game
*/
#define MAX_GAME_TEAMS 4

/**
 * @brief Constants for suit.
 *
 * SuitEnd is a flag used when iterating
 */
enum Suit {DIAMONDS = 0, CLUBS, SPADES, HEARTS, SuitEnd};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Game values for cards
 *
 * -1 is a flag used when iterating
 */
extern EXPORT const int VALUES[7];

#ifdef __cplusplus
}
#endif

#endif
