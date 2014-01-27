#include <deck.h>
#include <cutter.h>

static struct Deck deck;

void cut_setup()
{
    deck_deckInit(&deck);
}

void test_deck_test_deckInit()
{
    cut_assert_equal_int(0, deck_deckInit(&deck));
}

int cardsEqual(struct Card a, struct Card b)
{
    if (a.suit == b.suit && a.value == b.value) {
        return 1;
    }
    return 0;
}

void test_deck_test_NewDeck()
{
    int duplicates = 0;
    for (int i = 0; i < DECK_SIZE; i++) {
        for (int j = 0; j < DECK_SIZE; j++) {
            if (i != j) {
                duplicates += cardsEqual(deck.cards[i], deck.cards[j]);
            }
        }
    }
}

void test_deck_test_DeckShuffle()
{
    struct Deck shuffled = deck;
    cut_assert_equal_int(0, deck_deckShuffle(&shuffled));

    int differences = 0;
    for (int i = 0; i < DECK_SIZE; i++) {
        if (!cardsEqual(shuffled.cards[i], deck.cards[i])) {
            differences++;
        }
    }

    cut_assert_not_equal_int(differences, 0);
}

void test_deck_test_extensive_DeckShuffle()
{
    for (int i = 0; i < 100; i++) {
        test_deck_test_DeckShuffle();
    }
}

