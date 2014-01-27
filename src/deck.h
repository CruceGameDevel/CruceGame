/**
 * @file deck.h
 * @brief Card and Deck structures, as well as helper functions.
 */

#ifndef DECK_H
#define DECK_H

#include "constants.h"

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
