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
 * @brief Constants for suit.
 *
 * end is a flag used when iterating
 */
enum Suit {DIAMONDS = 0, CLUBS, SPADES, HEARTS, SuitEnd};

/**
 * @brief Game values for cards
 *
 * -1 is a flag used whe iterating
 */
extern const int VALUES[7];

#endif
