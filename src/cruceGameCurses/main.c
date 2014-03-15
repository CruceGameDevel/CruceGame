#include "cli.h"
#include <curses.h>
#include <locale.h>
#include <string.h>
#include <stddef.h>

int main()
{
    setlocale(LC_ALL, "");
    initscr();
    raw();

    welcomeMessage();
    int noOfPlayers = getNoOfPlayers();

    struct Game *game = game_createGame(11);
    for (int i = 0; i < noOfPlayers; i++) {
        int err = game_addPlayer(newPlayer(i + 1), game);
        if (err != 0)
            printw("ERROR: game_addPlayer() %d\n", err);
    }
    formTeams(game);

    while (!game_winningTeam(game)) {
        game->round = round_createRound();

        struct Deck *deck = deck_createDeck();
        deck_deckShuffle(deck);

        memcpy(game->round->players, game->players, 
               sizeof(struct Player*) * game->numberPlayers);
        round_distributeDeck(deck, game->round);
        clear();

        for (int i = 0; i < game->numberPlayers; i++) {
            getBid(game, i);
            clear();
        }

        struct Player *bidWinner = round_getBidWinner(game->round);
        int bidWinnerId = 0;
        while (game->round->players[bidWinnerId] != bidWinner)
            bidWinnerId++;

        for (int i = 0; i < 8; i++) {
            round_arrangePlayersHand(game->round, 
                                     (bidWinnerId + i) % game->numberPlayers);

            for (int j = 0; j < game->numberPlayers; j++) {
                displayCardsAndPickCard(game, j);
                clear();
            }
        }
   }

    getch();
    endwin();
    return 0;
}
