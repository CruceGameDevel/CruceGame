/**
 * @file game.h
 * @brief Game structures, as well as helper functions.
 */

#ifndef GAME_H
#define GAME_H

#include "constants.h"
#include "team.h"
#include "deck.h"
#include "round.h"

/**
 * @brief Game structure.
 */
struct Game {
    int numberPlayers;
    struct Round *round;
    struct Player *players[MAX_GAME_PLAYERS];
    struct Team *teams[MAX_GAME_TEAMS];
    struct Deck *deck;
};

/**
 * @brief Function to allocate memory for and initialize a Game.
 *
 * @return Pointer to the new Game on success, NULL on failure.
 */
struct Game *game_createGame();

/**
 * @brief Function to free memory of a Game. Makes pointer NULL.
 *
 * @param game Pointer to the game to be freed.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int game_deleteGame(struct Game **game);

/**
 * @brief Function to add player of a Game.
 *
 * @param player The player to be added.
 * @param game The game where to be added.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int game_addPlayer(struct Player *player, struct Game *game);

/**
 * @brief Function to remove player of a Game.
 *
 * @param player The player to be removed.
 * @param game The game where to be removed from.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int game_removePlayer(struct Player *player, struct Game *game);

/**
 * @brief Function to remove team of a Game.
 *
 * @param team The team to be removed.
 * @param game The game where to be removed from.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int game_removeTeam(struct Team *team, struct Game *game);

#endif

