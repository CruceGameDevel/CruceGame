/*
 * @file main.c
 * @brief The code in this file controls the activity of the whole game.
 *        This module is the controller.
 */

#include "cli.h"
#include <curses.h>
#include <locale.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#ifndef WIN32
#include <getopt.h>
#endif
#include <errno.h>
#ifdef WIN32
#include <Windows.h>
#define sleep(s) Sleep(s*1000)
#else
#include <unistd.h>
#endif 

/**
 * @brief Define the version of current game
 */
#define GAME_VERSION "0.4.0"

/**
 * @bried Path to the game help
 */
#define GAME_HELP_MANUAL "../docs/help.txt"

/**
 * @brief Prints the help manual of cruce game to the screen
 */
void cruceGameHelp()
{
    char text;
    FILE *helpFile;
    helpFile = fopen(GAME_HELP_MANUAL, "r");
    if(!(helpFile)) {
        printf("Unable to open\n");
        if(errno == ENOENT) {
            printf("File manual not exist\n");
        }
        if(errno == EACCES) {
            printf("No permission to read manual\n");
        }
        exit(EXIT_SUCCESS);
    }

    while( (text = fgetc(helpFile)) != EOF ) {
        printf("%c", text);
    }
    fclose(helpFile);
    exit(EXIT_SUCCESS);
}

/**
 * @brief Starts the game, connecting libraries and UI
 */
int cruceGameLogic()
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
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    refresh();

    welcomeMessage();
    int limitScore  = getScoreLimit();
    int noOfPlayers = getNoOfPlayers();

    struct Game *game = game_createGame(limitScore);
    for (int i = 0; i < noOfPlayers; i++) {
        int err;
        while ((err = game_addPlayer(newPlayer(i + 1), game)) == DUPLICATE_NAME)
             printw("The player's name have to be unique\n");
        if (err != 0)
            printw("ERROR: game_addPlayer() %d\n", err);
    }
    formTeams(game);

    for (int i = 0; !game_winningTeam(game); i++) {
        game_arrangePlayersRound(game, i % MAX_GAME_PLAYERS);

        struct Deck *deck = deck_createDeck();
        deck_deckShuffle(deck);

        round_distributeDeck(deck, game->round);
        clear();
        refresh();

        for (int i = 0; i < game->numberPlayers; i++) {
            getBid(game, i);
            clear();
            refresh();
        }
        displayBids(game, game->numberPlayers);
        refresh();
        sleep(2);
        clear();
        refresh();

        struct Player *bidWinner = round_getBidWinner(game->round);
        int first = round_findPlayerIndexRound(bidWinner, game->round);
        for (int i = 0; team_hasCards(game->players[0]); i++) {
            round_arrangePlayersHand(game->round, first);

            for (int j = 0; j < game->numberPlayers; j++) {
                WINDOW *scoreTableWindow = newwin(11, 49, 0, 30);
#ifdef BORDERS
                box(scoreTableWindow, 0, 0);
#endif
                printScore(game, game->round, scoreTableWindow);
                wrefresh(scoreTableWindow);
                delwin(scoreTableWindow);

                displayCardsAndPickCard(game, j);
                clear();
                refresh();
            }

            struct Player *handWinner = round_handWinner(game->round->hands[i],
                                                         game->round);
            first = round_findPlayerIndexRound(handWinner, game->round);

            if (deck_cardsNumber(deck) > 0)
                round_distributeCard(deck, game->round);

        }
        
        int oldScore[MAX_GAME_PLAYERS];
        for(int i = 0; i < MAX_GAME_TEAMS; i++) {
            if(game->teams[i] != NULL) {
                oldScore[i] = game->teams[i]->score;
            }
        }

        game_updateScore(game, bidWinner);

        printRoundTerminationMessage(game, oldScore);
        getch();

        deck_deleteDeck(&deck);
        round_deleteRound(&game->round);
    }

    clear();
    refresh();
    gameEndingMessage(game_winningTeam(game));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        if (game->players[i])
            team_deletePlayer(&game->players[i]);
    for (int i = 0; i < MAX_GAME_TEAMS; i++)
        if (game->teams[i])
            team_deleteTeam(&game->teams[i]);
    game_deleteGame(&game);

    getch();
    endwin();
    return EXIT_SUCCESS;
}

#if defined(WIN32) && defined(NDEBUG)
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
#ifndef WIN32
    if (argc >= 2) {
        int getoptCheck;
        struct option long_options[] = {
            {"help", no_argument, 0, 'h'},
            {"version", no_argument, 0, 'v'},
            {0, 0, 0, 0}
        };

        while ((getoptCheck =
                getopt_long(argc, argv, "hv", long_options, NULL)) != -1) {
            if (getoptCheck == -1)
                break;
            switch (getoptCheck) {
                case 'h':
                    cruceGameHelp();
                    break;
                case 'v':
                    printf("CruceGame Version: %s\n", GAME_VERSION);
                    exit(EXIT_SUCCESS);
                    break;
                case '?':
                    exit(EXIT_FAILURE);
                default:
                    exit(EXIT_FAILURE);
            }
        }

        if (optind < argc) {
            printf("non-option elements passed: ");
            while (optind < argc) {
                printf("%s ", argv[optind++]);
            }
            exit(EXIT_FAILURE);
        }
    } else {
#endif
        cruceGameLogic();
#ifndef WIN32
    }
#endif
    return EXIT_SUCCESS;
}

