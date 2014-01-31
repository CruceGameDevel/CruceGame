#include "output.h"
#include <libCruceGame.h>

int main()
{
    struct Deck *deck = deck_createDeck();
    deck_deckShuffle(deck);
    struct Player *player = team_createPlayer("FirstPlayer", 0, 0);
    team_addCard(player, deck->cards[0]);

    printPlayerCards(player);
    return 0;
}
