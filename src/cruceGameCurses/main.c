#include "output.h"
#include <curses.h>
#include <locale.h>

int main()
{
    setlocale(LC_ALL, "");
    initscr();
    raw();

    welcomeMessage();

    struct Deck *deck = deck_createDeck();
    deck_deckShuffle(deck);
    struct Player *player = team_createPlayer("FirstPlayer", 0, 0);
    team_addCard(player, deck->cards[0]);

    printPlayerCards(player);

    getch();
    endwin();
    return 0;
}
