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

void testNewDeck()
{
    for (int i = 0; i < DECK_SIZE; i++) {
        for (int j = 0; j < DECK_SIZE; j++) {
            if (i != j) {
                cut_assert_false(cardsEqual(deck[i], deck[j]));
            }
        }
    }
}

void testDeckShuffle()
{
    struct Deck shuffled = deck;
    deckShuffle(&shuffled);

    int differences = 0;
    for (int i = 0; i < DECK_SIZE; i++) {
        if (!cardsEqual(shuffled[i], deck[i])) {
            differences++;
        }
    }

    cut_assert_not_equal_int(differences, 0);
}

