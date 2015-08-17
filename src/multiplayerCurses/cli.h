#ifndef CLI_H
#define CLI_H

/**
 * @file cli.h
 * @brief Contains declaration of the functions used for building the user
 *        interface.
 */

#include <cruceGame.h>
#include <ncurses.h>
#include <network.h>

#define GAME_COMMAND_FLAG '#'

/**
 * @brief Function to get current player's name.
 *
 * @param win Pointer to the window from which the input will be obtained.
 * @return Pointer to the string which contains the name of the player.
 *         WARNING: THIS MUST BE FREED.
 */
char *getPlayerName(WINDOW *win);

/**
 * @brief Function to initialize the windows used for lobby (readWin and
 *        writeWin) defined in cli.c.
 *
 * @return void.
 */
void initWindows();

/**
 * @brief Handler for IRC PRIVMSG command.
 *
 * @param message Pointer to the Message recieved from IRC server parsed by
 *                ircParse().
 * @return NO_ERROR on success, other value on failure.
 */
int onPRIVMSG_handler(struct Privmsg *privmsg);

/**
 * @brief Handler for IRC JOIN command.
 *
 * @param message Pointer to the Message recieved from IRC server parsed by
 *                ircParse().
 * @return NO_ERROR on success, other value on failure.
 */
int onJOIN_handler(struct Message *message);

/**
 * @brief Handler for IRC QUIT command.
 *
 * @param message Pointer to the Message recieved from IRC server parsed by
 *                ircParse().
 * @return NO_ERROR on success, other value on failure.
 */
int onQUIT_handler(struct Message *message);

WINDOW *getReadWin(); //TEMPORARY
WINDOW *getPrintWin();

#endif
