#include <stdlib.h>
#include <time.h>
#include "deck.h"
#include "errors.h"
#include "constants.h"

#include <stdio.h>


/**
 * Constants for game values
 */
const int VALUES[] = {2, 3, 4, 0, 10, 11, -1};

/**
 * This function initializes a deck by iterating over all values and suits
 * available. The deck will be always the same.
 */
struct Deck *deck_newDeck()
{
    struct Deck *deck = malloc(sizeof(struct Deck));
    if (deck == NULL)
        return NULL;

    struct Card newCard;
    int k = 0;
    for (enum Suit i = 0; i < SuitEnd; i++) {
        for (int j = 0; VALUES[j] != -1; j++) {
            newCard.suit = i;
            newCard.value = VALUES[j];
            deck->cards[k++]= newCard;
        }
    }

    return deck;
}

/**
 * @brief Swap 2 Cards
 * 
 * Helper for deckShuffle
 *
 * @param a
 * @param b
 * @return void
 */
void deck_swap(struct Card *a, struct Card *b)
{
    struct Card c = *a;
    *a = *b;
    *b = c;
}

/**
 * This function randomly shuffles the deck.
 * It uses rand function from stdlib with time as seed.
 * The shuffle is performed by random swaps. The number of swaps is also
 * random, but it is at least SWAP_MIN and smaller then SWAP_MAX.
 */
int deck_deckShuffle(struct Deck *deck)
{
    srand(time(NULL));
    int swapInterval = SWAP_MAX - SWAP_MIN;
    int noOfSwaps = rand() % swapInterval + SWAP_MIN;

    for (int i = 0; i < noOfSwaps; i++) {
        int swapA = rand() % DECK_SIZE;
        int swapB = rand() % DECK_SIZE;
        if (swapA != swapB) {
            deck_swap(&deck->cards[swapA], &deck->cards[swapB]);
        }
    }

    return NO_ERROR;
}

int deck_deleteDeck(struct Deck **deck)
{
    if(deck == NULL)
        return POINTER_NULL;
    if (*deck == NULL)
        return DECK_NULL;

    free(*deck);
    *deck = NULL;

    return NO_ERROR;
}

int deck_compareCards(enum Suit firstCard, enum Suit trump,
                      struct Card *card1, struct Card *card2)
{
    if (card1 == NULL || card2 == NULL)
        return CARD_NULL;
    if (firstCard == SuitEnd ||
        trump == SuitEnd ||
        card1->suit == SuitEnd ||
        card2->suit == SuitEnd)
        return ILLEGAL_VALUE;
    if (card1->suit != firstCard &&
        card1->suit != trump &&
        card2->suit != firstCard &&
        card2->suit != trump)
        return INCOMPARABLE;

    int checkValue = 0;
    for(int i = 0; VALUES[i] != -1; i++) {
        if (card1->value == VALUES[i])
            checkValue++;
        if (card2->value == VALUES[i])
            checkValue++;
    }
    if (checkValue != 2) 
        return ILLEGAL_VALUE;

    if (card1->value == 0)
        card1->value = 9;
    if (card2->value == 0)
        card2->value = 9;

    if (card1->suit == card2->suit && card1->value == card2->value)
        return 0;
    if (card1->suit == trump && card2->suit != trump)
        return 1;
    if (card2->suit == trump && card1->suit != trump)
        return 2;
    if (card1->suit == firstCard && card2->suit != trump && 
        card2->suit != firstCard)
        return 1;
    if (card2->suit == firstCard && card1->suit != trump &&
        card1->suit != firstCard)
        return 2;
    if (card1->suit == firstCard && card2->suit == firstCard) {
        if (card1->value > card2->value)
            return 1;
        else
            return 2;
    }
    if (card1->suit == trump && card2->suit == trump) {
        if (card1->value > card2->value)
            return 1;
        else
            return 2;
    }

    if (card1->value == 9)
        card1->value = 0;
    if (card2->value == 9)
        card2->value = 0;
}
