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
    int pointsNumber;
    struct Round *round;
    struct Player *players[MAX_GAME_PLAYERS];
    struct Team *teams[MAX_GAME_TEAMS];
    struct Deck *deck;
};

/**
 * @brief Allocates memory for and initializes a game.
 *
 * @param numberPoints The number of points required for winning the game.
 *
 * @return Pointer to the new game on success or NULL on failure.
 */
struct Game *game_createGame(int numberPoints);

/**
 * @brief Frees the memory of a game and makes the pointer NULL.
 *
 * @param game Pointer to the game to be deleted.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
int game_deleteGame(struct Game **game);

/**
 * @brief Adds a player to a game.
 *
 * @param player The player to be added.
 * @param game The game where the player is to be added.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
int game_addPlayer(struct Player *player, struct Game *game);

/**
 * @brief Removes a player from a game.
 *
 * @param player The player to be removed.
 * @param game The game from where the player is to be removed.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
int game_removePlayer(struct Player *player, struct Game *game);

/**
 * @brief Adds a team to a game.
 *
 * @param team The team to be added.
 * @param game The game where the team is to be added to.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
int game_addTeam(struct Team *team, struct Game *game);

 
/**
 * @brief Removes a team from a game.
 *
 * @param team The team to be removed.
 * @param game The game from where the team is to be removed.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
int game_removeTeam(struct Team *team, struct Game *game);

/**
 * @brief Searches the winning team of a game.
 *
 * @param game The game in which the winning team is to be search.
 *
 * @return Pointer to the winner team on success or NULL on failure.
 */
struct Team *game_winningTeam(struct Game *game);

/**
 * @brief Function checks if the player can put a card down.
 *
 * @param player The player who wants to put the card down.
 * @param game The game where the player is located.
 * @param hand The hand in which should put the card.
 * @param idCard The id of the card.
 *
 * @return 1 if the player may to put the card down 
 *         0 if the player can't to put the card down
 *         other value on failure.
 */
int game_checkCard(struct Player *player, struct Game *game,
                   struct Hand *hand, int idCard);

#endif

