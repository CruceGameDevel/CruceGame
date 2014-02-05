#include "cli.h"
#include <curses.h>
#include <locale.h>

int main()
{
    setlocale(LC_ALL, "");
    initscr();
    raw();

    welcomeMessage();
    int noOfPlayers = getNoOfPlayers();

    struct Deck *deck = deck_createDeck();
    deck_deckShuffle(deck);

    struct Game *game = game_createGame();
    for (int i = 0; i < noOfPlayers; i++) {
        int err = game_addPlayer(newPlayer(i), game);
        if (err != 0)
            printw("ERROR: game_addPlayer() %d\n", err);
    }


    getch();
    endwin();
    return 0;
}
