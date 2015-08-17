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
#include "ircHandlers.h"

/**
 * @brief Define the version of current game
 */
#define GAME_VERSION "0.4.0"

/**
 * @bried Path to the game help
 */
#define GAME_HELP_MANUAL "../docs/help.txt"

/**
 * @brief Starts the game, connecting libraries and UI
 */
int multiplayerLogic()
{
    WINDOW *welcomeWin = newwin(80, 79, 0, 0);
    welcomeMessage(welcomeWin);

#ifdef DEBUG
    wprintw(welcomeWin, "DEBUG MODE\n");
#endif

    char *name = getPlayerName(welcomeWin);  //has to be freed
    sleep(1);
    wclear(welcomeWin);
    delwin(welcomeWin);

    Connect(name);

    initWindows();

    struct Handlers *handlers = malloc(sizeof(struct Handlers)); //TODO: free it
    handlers->onPRIVMSG = onPRIVMSG_handler;
    handlers->onJOIN    = onJOIN_handler;
    handlers->onQUIT    = onQUIT_handler;
    handlers->onNOTICE  = onNOTICE_handler;

    pthread_t readFromSocket_var;
    pthread_create(&readFromSocket_var, NULL, &readFromSocket, handlers);

    readFromKeyboard(getReadWin());

    pthread_join(readFromSocket_var, NULL);

    return EXIT_SUCCESS;
}

#if defined(WIN32) && defined(NDEBUG)

int CALLBACK multiplayerWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int multiplayerMain(int argc, char *argv[])
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
        multiplayerLogic();
#ifndef WIN32
    }
#endif
    return EXIT_SUCCESS;
}

