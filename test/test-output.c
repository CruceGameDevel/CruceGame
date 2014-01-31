#include <deck.h>
#include <output.h>

void test_printCard()
{
    struct Card *card = deck_createCard(CLUBS, VALUES[0]);

    printCard(stdout, card);
}
