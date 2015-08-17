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
#define GAME_HELP_MANUAL "../docs/man1/help.txt"

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
    }

    while( (text = fgetc(helpFile)) != EOF ) {
        printf("%c", text);
    }
    fclose(helpFile);
}

/**
 * @brief Prints the help manual of cruce game to the ncurses window
 */
void wCruceGameHelp(WINDOW *win)
{
	char text;
    FILE *helpFile;
    helpFile = fopen(GAME_HELP_MANUAL, "r");
    if(!(helpFile)) {
        wprintw(win, "Unable to open\n");
        if(errno == ENOENT) {
            wprintw(win, "File manual not exist\n");
        }
        if(errno == EACCES) {
            wprintw(win, "No permission to read manual\n");
        }
    }

    while( (text = fgetc(helpFile)) != EOF ) {
        wprintw(win, "%c", text);
    }
    fclose(helpFile);
}

/**
 * @brief Starts the game, connecting libraries and UI
 */
int singleplayerLogic()
{
    WINDOW *welcomeWin = newwin(80, 79, 0, 0);
    welcomeMessage(welcomeWin);

#ifdef DEBUG
    wprintw(welcomeWin, "DEBUG MODE\n");
#endif

    int limitScore  = getScoreLimit(welcomeWin);
    int noOfPlayers = getNoOfPlayers(welcomeWin);

    struct Game *game = game_createGame(limitScore);
    for (int i = 0; i < noOfPlayers; i++) {
        int err;
        while ((err = game_addPlayer(newPlayer(welcomeWin, i + 1), game)) == DUPLICATE_NAME)
            wprintw(welcomeWin, "The player's name have to be unique\n");
        if (err != 0)
            wprintw(welcomeWin, "ERROR: game_addPlayer() %d\n", err);
    }
    formTeams(welcomeWin, game);

    delwin(welcomeWin);
    curs_set(0);

    for (int i = 0; !game_winningTeam(game); i++) {
        game_arrangePlayersRound(game, i % MAX_GAME_PLAYERS);

        struct Deck *deck = deck_createDeck();
        deck_deckShuffle(deck);

        round_distributeDeck(deck, game->round);

        WINDOW *bidSelectWindow = newwin(80, 79, 0, 0);
        for (int i = 0; i < game->numberPlayers; i++) {
            getBid(bidSelectWindow, game, i);
            if (i < game->numberPlayers - 1)
                wmove(bidSelectWindow, 0, 0);
        }
        wmove(bidSelectWindow, 0, 0);
        wclear(bidSelectWindow);
        bidSummary(bidSelectWindow, game);
        wrefresh(bidSelectWindow);
        wgetch(bidSelectWindow);
        delwin(bidSelectWindow);

        struct Player *bidWinner = round_getBidWinner(game->round);
        int first = round_findPlayerIndexRound(bidWinner, game->round);
        for (int i = 0; team_hasCards(game->players[0]); i++) {
            round_arrangePlayersHand(game->round, first);

            WINDOW *scoreTableWindow = newwin(11, 49, 0, 30);
#ifdef BORDERS
            box(scoreTableWindow, 0, 0);
#endif
            for (int j = 0; j < game->numberPlayers; j++) {
                printScore(scoreTableWindow, game, game->round);
                wmove(scoreTableWindow, 0, 0);
                wrefresh(scoreTableWindow);
                displayCardsAndPickCard(game, j);
            }
            delwin(scoreTableWindow);

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

        WINDOW *roundTerminationWindow = newwin(30, 79, 0, 0);
        printRoundTerminationMessage(roundTerminationWindow, game, oldScore);
        wrefresh(roundTerminationWindow);
        wgetch(roundTerminationWindow);
        delwin(roundTerminationWindow);

        deck_deleteDeck(&deck);
        round_deleteRound(&game->round);
    }

    WINDOW *gameEndingWindow = newwin(30, 79, 0, 0);
    gameEndingMessage(gameEndingWindow, game_winningTeam(game));

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        if (game->players[i])
            team_deletePlayer(&game->players[i]);
    for (int i = 0; i < MAX_GAME_TEAMS; i++)
        if (game->teams[i])
            team_deleteTeam(&game->teams[i]);
    game_deleteGame(&game);

    return EXIT_SUCCESS;
}

#if defined(WIN32) && defined(NDEBUG)

int CALLBACK singleplayerWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int singleplayerMain(int argc, char *argv[])
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
        singleplayerLogic();
#ifndef WIN32
    }
#endif
    return EXIT_SUCCESS;
}

