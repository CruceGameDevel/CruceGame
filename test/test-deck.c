#include <deck.h>
#include <cutter.h>
#include <string.h>
#include "../src/constants.h"
#include "../src/errors.h"

int cardsEqual(struct Card a, struct Card b)
{
    if (a.suit == b.suit && a.value == b.value) {
        return 1;
    }
    return 0;
}

void test_deck_deleteDeck()
{
    struct Deck *deck = deck_newDeck();

    deck_deleteDeck(&deck);
    cut_assert_equal_pointer(deck, NULL);
}

void test_deck_newDeck()
{
    struct Deck *deck = deck_newDeck();

    int duplicates = 0;
    for (int i = 0; i < DECK_SIZE; i++) {
        for (int j = 0; j < DECK_SIZE; j++) {
            if (i != j) {
                duplicates += cardsEqual(deck->cards[i], deck->cards[j]);
            }
        }
    }
    cut_assert_equal_int(duplicates, 0);

    deck_deleteDeck(&deck);
}

void test_deck_deckShuffle()
{
    struct Deck *deck = deck_newDeck();
    struct Deck *shuffled = malloc(sizeof(struct Deck));
    memcpy(shuffled, deck, sizeof(struct Deck));

    cut_assert_equal_int(0, deck_deckShuffle(shuffled));

    int differences = 0;
    for (int i = 0; i < DECK_SIZE; i++) {
        if (!cardsEqual(shuffled->cards[i], deck->cards[i])) {
            differences++;
        }
    }

    cut_assert_not_equal_int(differences, 0);

    deck_deleteDeck(&deck);
    deck_deleteDeck(&shuffled);
}

void test_extensive_deck_deckShuffle()
{
    for (int i = 0; i < 100; i++) {
        test_deck_deckShuffle();
    }
}

void test_deck_compareCards()
{
    struct Card *card1 = malloc(sizeof(struct Card));
    struct Card *card2 = malloc(sizeof(struct Card));
    for (enum Suit i = 0; i < SuitEnd; i++) {
        cut_assert_equal_int(CARD_NULL, deck_compareCards(i, i, NULL, NULL));
        cut_assert_equal_int(CARD_NULL, deck_compareCards(i, i, NULL, card2));
        cut_assert_equal_int(CARD_NULL, deck_compareCards(i, i, card1, NULL));

        card1->suit = i;
        card2->suit = i;
        cut_assert_equal_int(ILLEGAL_VALUE,
                             deck_compareCards(SuitEnd, i, card1, card2));
        cut_assert_equal_int(ILLEGAL_VALUE,
                             deck_compareCards(i, SuitEnd, card1, card2));
        cut_assert_equal_int(ILLEGAL_VALUE,
                             deck_compareCards(SuitEnd, SuitEnd,
                                               card1, card2));

        card1->suit = SuitEnd;
        cut_assert_equal_int(ILLEGAL_VALUE,
                             deck_compareCards(i, i, card1, card2));
        cut_assert_equal_int(ILLEGAL_VALUE,
                             deck_compareCards(SuitEnd, i, card1, card2));
        cut_assert_equal_int(ILLEGAL_VALUE,
                             deck_compareCards(i, SuitEnd, card1, card2));
        cut_assert_equal_int(ILLEGAL_VALUE,
                             deck_compareCards(SuitEnd, SuitEnd,
                                               card1, card2));

        cut_assert_equal_int(ILLEGAL_VALUE,
                             deck_compareCards(i, i, card2, card1));
        cut_assert_equal_int(ILLEGAL_VALUE,
                             deck_compareCards(SuitEnd, i, card2, card1));
        cut_assert_equal_int(ILLEGAL_VALUE,
                             deck_compareCards(i, SuitEnd, card2, card1));
        cut_assert_equal_int(ILLEGAL_VALUE,
                             deck_compareCards(SuitEnd, SuitEnd,
                                               card2, card1));

        for (enum Suit j = 0; j < SuitEnd; j++) {
            for (enum Suit k = 0; k < SuitEnd; k++) {
                if (k != i && k != j) {
                    card1->suit = k;
                    card2->suit = k;
                    cut_assert_equal_int(INCOMPARABLE,
                                         deck_compareCards(i, j,
                                                           card1, card2));
                }
            }
        }

        for (int j = 0; VALUES[j] != -1; j++) {
            card1->suit = i;
            card1->value = VALUES[j];
            cut_assert_equal_int(0, deck_compareCards(i, i, card1, card1));
        }

        for (enum Suit j = 0; j < SuitEnd; j++) {
            if (i != j) {
                for (int k = 0; VALUES[k] != -1; k++) {
                    card1->value = VALUES[k];
                    card2->value = VALUES[k];
                    card1->suit = i;
                    card2->suit = j;
                    cut_assert_equal_int(1,
                                         deck_compareCards(i, i, 
                                                           card1, card2));
                    card1->value = VALUES[k];
                    card2->value = VALUES[k];
                    cut_assert_equal_int(2, deck_compareCards(i, i, 
                                                              card2, card1));
                }
            }
        }

        for (enum Suit j = 0; j < SuitEnd; j++)
            for (enum Suit k = 0; k < SuitEnd; k++)
                if (k != i && k != j && i != j)
                    for (int n = 0; VALUES[n] != -1; n++) {
                        card1->value = VALUES[n];
                        card2->value = VALUES[n];
                        card1->suit = i;
                        card2->suit = k;
                        cut_assert_equal_int(1,
                                             deck_compareCards(i, j,
                                                               card1, card2));
                        card1->value = VALUES[k];
                        card2->value = VALUES[k];
                        cut_assert_equal_int(2,
                                             deck_compareCards(i, j,
                                                               card2, card1));
                    }

        for (enum Suit j = 0; j < SuitEnd; j++)
            if (i != j)
                for (int k = 0; VALUES[k] != -1; k++)
                    for (int n = 0; VALUES[n] !=-1; n++)
                        if (k < n) {
                            card1->suit = i;
                            card2->suit = i;
                            card1->value = VALUES[k];
                            card2->value = VALUES[n];
                            cut_assert_equal_int(1,
                                                 deck_compareCards(i, j,
                                                                   card2,
                                                                   card1));
                            card1->value = VALUES[k];
                            card2->value = VALUES[n];
                            cut_assert_equal_int(2,
                                                 deck_compareCards(i, j, 
                                                                   card1,
                                                                   card2));
                            card1->value = VALUES[k];
                            card2->value = VALUES[n];
                            cut_assert_equal_int(1,
                                                 deck_compareCards(j, i,
                                                                   card2,
                                                                   card1));
                            card1->value = VALUES[k];
                            card2->value = VALUES[n];
                            cut_assert_equal_int(2,
                                                 deck_compareCards(j, i,
                                                                   card1,
                                                                   card2));
                        }
    }
}
