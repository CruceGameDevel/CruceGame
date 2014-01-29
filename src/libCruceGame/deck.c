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

/**
 * This function initializes a deck by iterating over all values and suits
 * available. The deck will be always the same.
 */
struct Deck *deck_createDeck()
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

int deck_compareCards(struct Card *card1, struct Card *card2, enum Suit trump)
{
    if (card1 == NULL || card2 == NULL)
        return CARD_NULL;

    struct Card *card1_copy = malloc(sizeof(struct Card));
    memcpy(card1_copy, card1, sizeof(struct Card));
    struct Card *card2_copy = malloc(sizeof(struct Card));
    memcpy(card2_copy, card2, sizeof(struct Card));

    if (trump == SuitEnd ||
        card1_copy->suit == SuitEnd ||
        card2_copy->suit == SuitEnd)
        return ILLEGAL_VALUE;

    int checkValue = 0;
    for(int i = 0; VALUES[i] != -1; i++) {
        if (card1_copy->value == VALUES[i])
            checkValue++;
        if (card2_copy->value == VALUES[i])
            checkValue++;
    }
    if (checkValue != 2) 
        return ILLEGAL_VALUE;

    if (card1_copy->value == 0)
        card1_copy->value = 9;
    if (card2_copy->value == 0)
        card2_copy->value = 9;

    if (card1_copy->suit == card2_copy->suit &&
        card1_copy->value == card2_copy->value)
        return 0;
    if (card1_copy->suit == trump && card2_copy->suit != trump)
        return 1;
    if (card2_copy->suit == trump && card1_copy->suit != trump)
        return 2;
    if (card1_copy->suit != card2_copy->suit)
        return 1;
    if (card1_copy->suit == card2_copy->suit) {
        if (card1_copy->value > card2_copy->value)
            return 1;
        else
            return 2;
    }
    if (card1_copy->suit == trump && card2_copy->suit == trump) {
        if (card1_copy->value > card2_copy->value)
            return 1;
        else
            return 2;
    }
}
