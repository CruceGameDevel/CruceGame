#include <deck.h>
#include <cutter.h>

static struct Deck deck;

void cut_setup()
{
    deckInit(&deck);
}

int cardsEqual(struct Card a, struct Card b)
{
    if (a.suit == b.suit && a.value == b.value) {
        return 1;
    }
    return 0;
}

void test_NewDeck()
{
    for (int i = 0; i < DECK_SIZE; i++) {
        for (int j = 0; j < DECK_SIZE; j++) {
            if (i != j) {
                cut_assert_false(cardsEqual(deck.cards[i], deck.cards[j]));
            }
        }
    }
}

void test_DeckShuffle()
{
    struct Deck shuffled = deck;
    deckShuffle(&shuffled);

    int differences = 0;
    for (int i = 0; i < DECK_SIZE; i++) {
        if (!cardsEqual(shuffled.cards[i], deck.cards[i])) {
            differences++;
        }
    }

    cut_assert_not_equal_int(differences, 0);
}

void test_extensive_DeckShuffle()
{
    for (int i = 0; i < 100; i++) {
        test_DeckShuffle();
    }
}

