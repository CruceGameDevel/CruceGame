#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "deck.h"
#include "errors.h"
#include "constants.h"

#include <stdio.h>


/**
 * Constants for game values
 */
const int VALUES[] = {2, 3, 4, 0, 10, 11, -1};

struct Card *deck_createCard(enum Suit suit, int value)
{
    struct Card *card = malloc(sizeof(struct Card));

    if (card == NULL)
        return NULL;

    if (suit == SuitEnd)
        return NULL;

    int count = 0;
    for (int i = 0; VALUES[i] != -1; i++)
        if (value == VALUES[i])
            count++;
    if (count != 1)
        return NULL;

    card->value = value;
    card->suit = suit;

    return card;
}

int deck_deleteCard(struct Card **card)
{
    if (card == NULL)
        return POINTER_NULL;
    if (*card == NULL)
        return CARD_NULL;

    free(*card);
    *card = NULL;

    return NO_ERROR;
}

/**
 * This function initializes a deck by iterating over all values and suits
 * available. The deck will be always the same.
 */
struct Deck *deck_createDeck()
{
    struct Deck *deck = malloc(sizeof(struct Deck));
    if (deck == NULL)
        return NULL;

    int k = 0;
    for (enum Suit i = 0; i < SuitEnd; i++) {
        for (int j = 0; VALUES[j] != -1; j++) {
            struct Card *card = deck_createCard(i, VALUES[j]);
            deck->cards[k++] = card;
        }
    }

    return deck;
}

int deck_deleteDeck(struct Deck **deck)
{
    if(deck == NULL)
        return POINTER_NULL;
    if (*deck == NULL)
        return DECK_NULL;

    for (int i = 0; i < DECK_SIZE; i++) {
        free((*deck)->cards[i]);
    }

    free(*deck);
    *deck = NULL;

    return NO_ERROR;
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
void deck_swap(struct Card **a, struct Card **b)
{
    struct Card *c = *a;
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
            struct Card **cardA = &(deck->cards[swapA]);
            struct Card **cardB = &(deck->cards[swapB]);
            deck_swap(cardA, cardB);
        }
    }

    return NO_ERROR;
}

int deck_compareCards(const struct Card *card1,const struct Card *card2, enum Suit trump)
{
    if (card1 == NULL || card2 == NULL)
        return CARD_NULL;

    if (trump == SuitEnd)
        return ILLEGAL_VALUE;

    if (card1->suit == card2->suit &&
        card1->value == card2->value)
        return 0;
    if (card1->suit == trump && card2->suit != trump)
        return 1;
    if (card2->suit == trump && card1->suit != trump)
        return 2;
    if (card1->suit != card2->suit)
        return 1;

    if (card1->suit == card2->suit) {
        if (card1->value > card2->value)
            return 1;
        else 
            return 2;
    }

    return ERROR_COMPARE;
}

int deck_cardsNumber(struct Deck *deck)
{
    if (deck == NULL)
        return DECK_NULL;

    int cardsNumber = 0;
    for (int i = 0; i < DECK_SIZE; i++)
        if (deck->cards[i] != NULL)
            cardsNumber++;

    return cardsNumber;
}


