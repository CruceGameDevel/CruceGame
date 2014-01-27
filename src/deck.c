#include <stdlib.h>
#include <time.h>
#include "deck.h"
#include "errors.h"

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
    if (*deck == NULL)
        return POINTER_NULL;

    free(*deck);
    *deck = NULL;
}

