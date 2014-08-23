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

/**
 * @brief Function to display a greeting message at the beginning of a new
 *        game.
 * 
 * @param win Pointer to the window in which the output will be displayed.
 * @return void.
 */
void welcomeMessage(WINDOW *win);

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
int onPRIVMSG_handler(struct Message *message);

WINDOW *getReadWin(); //TEMPORARY


#endif
