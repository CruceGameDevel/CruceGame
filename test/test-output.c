#include <deck.h>
#include <output.h>

void test_printCard()
{
    struct Deck *deck = deck_createDeck();

    for (int i = 0; i < 24; i++)
        printCard(stdout, deck->cards[i]);
        
}
