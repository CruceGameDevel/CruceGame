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

    Connect(name);

    initWindows();

    struct Handlers *handlers = malloc(sizeof(struct Handlers)); //TODO: free it
    handlers->onPRIVMSG = onPRIVMSG_handler;
    handlers->onJOIN    = onJOIN_handler;
    handlers->onQUIT    = onQUIT_handler;
    handlers->onNOTICE  = onNOTICE_handler;

    pthread_t readFromSocket_var, readFromKeyboard_var;

    pthread_create(&readFromSocket_var, NULL, &readFromSocket, handlers);
    pthread_create(&readFromKeyboard_var, NULL,
                   &readFromKeyboard, getReadWin());

    pthread_join(readFromKeyboard_var, NULL);
    pthread_join(readFromSocket_var, NULL);

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

