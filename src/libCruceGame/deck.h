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
    struct Card *cards[DECK_SIZE];
};

/**
 * @brief Function to allocate and initialize a Card.
 *
 * @param suit The suit of the new Card.
 * @param value The value of the new Card.
 *
 * @return Pointer to the new card on success, NULL on failure.
 */
struct Card *deck_createCard(enum Suit suit, int value);

/**
 * @brief Function to free memory of a Card. Makes pointer NULL.
 *
 * @param card Pointer to the pointer to be freed.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int deck_deleteCard(struct Card **card);

/**
 * @brief Function to allocate and initialize a Deck.
 *
 * @return Pointer to the new Deck on success, NULL on failure.
 */
struct Deck *deck_createDeck();

/**
 * @brief Shuffle a deck.
 *
 * Randomly shuffles a deck.
 *
 * @param deck The deck to be shuffled.
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int deck_deckShuffle(struct Deck *deck);

/**
 * @brief Free memory for a Deck. Sets the pointer to NULL.
 *
 * @param deck Pointer to the deck to be freed.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int deck_deleteDeck(struct Deck **deck);

/**
* @brief Compare two cards.
*
* @param card1 Is the first card.
* @param card2 Is the second card.
* @param trump Is the trump of the round.
*
* @return 0 If the cards are equal. 
*         1 If the first card is winning.
*         2 If the second card is winning.
*         Other value on failure.
*/
int deck_compareCards(const struct Card *card1,const struct Card *card2, enum Suit trump);

#endif
