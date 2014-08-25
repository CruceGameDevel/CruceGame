/**
 * @file cli.c
 * @brief Contains implementations of the functions used for creating
 *        the user interface. These functions are declared in the
 *        header file cli.h.
 */

#include "cli.h"

#include <curses.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <Windows.h>
#define sleep(s) Sleep(s*1000)
#else
#include <unistd.h>
#endif

#define MAX_CARDS_PER_LINE 8
#define MAX_NAME_SIZE 9
#define ROUND_DIALOG_SCORE_SIZE 5
#define SLEEP_TIME 2

static WINDOW *printWin, *readWin;

void welcomeMessage(WINDOW *win)
{
    wprintw(win,
            "  _____                        _____                           \n"
            " / ____|                      / ____|                          \n"
            "| |     _ __ _   _  ___ ___  | |  __  __ _ _ __ ___   ___      \n"
            "| |    | '__| | | |/ __/ _ \\ | | |_ |/ _` | '_ ` _ \\ / _ \\  \n"
            "| |____| |  | |_| | (_|  __/ | |__| | (_| | | | | | |  __/     \n"
            " \\_____|_|   \\__,_|\\___\\___|  \\_____|\\__,_|_| |_| |_|\\___|"
            "\n\n");
}

char *getPlayerName(WINDOW *win)
{
    char *name = malloc(MAX_NAME_SIZE);
    wprintw(win, "Insert your name: ");
    wscanw(win, "%s", name);
    return name;
}

void initWindows()
{
    printWin = newwin(35, 150, 0, 0);
    readWin  = newwin(1,  79,  36, 0);

    scrollok(readWin,  TRUE);
    scrollok(printWin, TRUE);
}

WINDOW *getReadWin()
{
    return readWin;
}

WINDOW *getPrintWin()
{
    return printWin;
}

