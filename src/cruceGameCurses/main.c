#include "cli.h"
#include <curses.h>
#include <locale.h>
#include <string.h>
#include <stddef.h>

#if defined(WIN32) && defined(NDEBUG)
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
    setlocale(LC_ALL, "");
    initscr();
    cbreak();

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support colors!");
        return 0;
    }

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);

    welcomeMessage();
    int limitScore  = getScoreLimit();
    int noOfPlayers = getNoOfPlayers();

    struct Game *game = game_createGame(limitScore);
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

            if (deck_cardsNumber(deck) > 0)
                round_distributeCard(deck, game->round);

        }
        for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
            if (game->round->players[i] != NULL && 
                game->round->players[i] != bidWinner) {
                game->round->players[i]->score += 
                    game->round->pointsNumber[i] / 33;
            } else if (game->round->players[i] == bidWinner) {
                if (game->round->bids[i] <= game->round->pointsNumber[i] / 33)
                    bidWinner->score += game->round->pointsNumber[i] / 33;
                else
                    bidWinner->score -= game->round->bids[i];
            }
        }

        deck_deleteDeck(&deck);
        round_deleteRound(&game->round);
    }

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        if (game->players[i])
            team_deletePlayer(&game->players[i]);
    for (int i = 0; i < MAX_GAME_TEAMS; i++)
        if (game->teams[i])
            team_deleteTeam(&game->teams[i]);
    game_deleteGame(&game);

    getch();
    endwin();
    return 0;
}
