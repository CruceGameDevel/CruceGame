#include <deck.h>
#include <cutter.h>
#include <string.h>
#include "../src/constants.h"

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

