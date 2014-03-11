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
        int first = round_findPlayerIndexRound(bidWinner, game->round);
        for (int i = 0; team_hasCards(game->players[0]); i++) {
            round_arrangePlayersHand(game->round, first);

            for (int j = 0; j < game->numberPlayers; j++) {
                printScore(game, game->round);
                displayCardsAndPickCard(game, j);
                clear();
            }

            struct Player *handWinner = round_handWinner(game->round->hands[i],
                                                         game->round->trump, 
                                                         game->round);
            first = round_findPlayerIndexRound(handWinner, game->round);
        }
   }

    getch();
    endwin();
    return 0;
}
