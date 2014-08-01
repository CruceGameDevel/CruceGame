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
#include <pthread.h>
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

#include <network.h>
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

    WINDOW *welcomeWin = newwin(80, 79, 0, 0);
    welcomeMessage(welcomeWin);

#ifdef DEBUG
    wprintw(welcomeWin, "DEBUG MODE\n");
#endif

    char *name = getPlayerName(welcomeWin);  //has to be freed
    sleep(1);
    wclear(welcomeWin);
    delwin(welcomeWin);

    WINDOW *printWin = newwin(35, 150, 0, 0);
    WINDOW *readWin  = newwin(1,  79,  36, 0);

    scrollok(readWin,  TRUE);
    scrollok(printWin, TRUE);

    Connect(name);

    pthread_t readFromSocket_var;
    pthread_create(&readFromSocket_var, NULL, &readFromSocket, readWin);

    pthread_t readFromKeyboard_var;
    pthread_create(&readFromKeyboard_var, NULL, &readFromKeyboard, printWin);

    pthread_join(readFromKeyboard_var, NULL);
    pthread_join(readFromSocket_var, NULL);

//old code
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

