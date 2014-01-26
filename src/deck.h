/**
 * @file deck.h
 * @brief Card and Deck structures, as well as helper functions.
 */

#ifndef DECK_H
#define DECK_H

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

/**
 * @brief Card structure, to keep suit and value.
 *
 * Note that value is the game value (i.e. the points), not the face value.
 */
struct Card{
    enum Suit suit;
    int value;
};

/**
 * @brief A 28 card deck used in this game.
 *
 * Before using a Deck, please use deckInit function to assign the cards.
 */
struct Deck{
    struct Card cards[DECK_SIZE];
};

/**
 * @brief Function to initialize a Deck.
 *
 * @param deck The deck to be initialised.
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int deck_deckInit(struct Deck *deck);

/**
 * @brief Shuffle a deck.
 *
 * Randomly shuffles a deck.
 *
 * @param deck The deck to be shuffled.
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int deck_deckShuffle(struct Deck* deck);

#endif
