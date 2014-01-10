/**
 * @file deck.h
 * @brief Card and Deck structures, as well as helper functions.
 */

#ifndef CARD_H
#define CARD_H

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
const int VALUES[] = {2, 3, 4, 0, 10, 11, -1};

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
 * @return void
 */
void deck_deckInit(struct Deck *deck);

/**
 * @brief Shuffle a deck.
 *
 * Randomly shuffles a deck.
 *
 * @param deck The deck to be shuffled.
 * @return void
 */
void deck_deckShuffle(struct Deck* deck);

#endif
