/**
 * @file deck.h
 * @brief Card and Deck structures, as well as helper functions.
 */

#ifndef DECK_H
#define DECK_H

#include "platform.h"
#include "constants.h"

/**
 * @struct Card
 * @brief Card structure, to keep suit and value.
 *
 * Note that value is the game value (i.e. the points), not the face value.
 *
 * @var Card::suit
 *     The suit of the card.
 * @var Card::value
 *     The value of the card.
 */
struct Card{
    enum Suit suit;
    int value;
};

/**
 * @struct Deck
 * @brief A 28 card deck used in this game.
 *
 * Before using a Deck, please use deckInit function to assign the cards.
 *
 * @var Deck::cards
 *     Pointer to the cards of the deck.
 */
struct Deck{
    struct Card *cards[DECK_SIZE];
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocates and initializes a card.
 *
 * @param suit The suit of the new card.
 * @param value The value of the new card.
 *
 * @return Pointer to the new card on success or NULL on failure.
 */
EXPORT struct Card *deck_createCard(enum Suit suit, int value);

/**
 * @brief Frees the memory of a card and makes the pointer NULL.
 *
 * @param card Pointer to the pointer to be freed.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
EXPORT int deck_deleteCard(struct Card **card);

/**
 * @brief Allocates and initializes a deck.
 *
 * @return Pointer to the new deck on success or NULL on failure.
 */
EXPORT struct Deck *deck_createDeck();

/**
 * @brief Shuffles a deck.
 *
 * @param deck The deck to be shuffled.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
EXPORT int deck_deckShuffle(struct Deck *deck);

/**
 * @brief Frees the memory of a deck and sets the pointer to NULL.
 *
 * @param deck Pointer to the pointer to be freed.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
EXPORT int deck_deleteDeck(struct Deck **deck);

/**
* @brief Compare two cards.
*
* @param card1 The first card.
* @param card2 The second card.
* @param trump The trump of the round.
*
* @return 0 If the cards are equal. 
*         1 If the first card is winning.
*         2 If the second card is winning.
*         Error code otherwise.
*/
EXPORT int deck_compareCards(const struct Card *card1,const struct Card *card2, enum Suit trump);

/**
 * @brief The function counts the cards from deck.
 *
 * @param deck Pointer to the deck from which it counts.
 *
 * @return The cards number from deck.
 */
EXPORT int deck_cardsNumber(struct Deck *deck);

#ifdef __cplusplus
}
#endif

#endif

