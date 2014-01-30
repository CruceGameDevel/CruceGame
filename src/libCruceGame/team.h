/**
 * @file player.h
 * @brief Player structure, with helper functions.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "deck.h"

/**
 * @brief Player structure.
 */
struct Player{
    int id;
    char *name;
    struct Card *hand[MAX_CARDS];
    int score;
    int sockfd;
    int isHuman; //0 for AI, non-zero for human.
};

/**
 * @brief Team structure.
 */
struct Team{
    int id;
    char *name;
    struct Player *players[MAX_TEAM_PLAYERS];
};

/**
 * @brief Function for basic player creation.
 *
 * @param name The name of the new player.
 * @param sockfd Socket file descriptor for connection.
 * @param isHuman Player type.
 *
 * @return Pointer to the created player. Needs to be freed.
 */
struct Player *team_createPlayer(char *name, int sockfd, int isHuman);

/**
 * @brief Function for basic team creation.
 *
 * @param name The name of the new team.
 *
 * @return Pointer to the created team. Needs to be freed.
 */
struct Team *team_createTeam(char *name);

/**
* @brief Function for adding a player in a team.
*
* @param team The team in that is added the player.
* @param player The player to be added in the team.
*
* @return 0 If the player was added in team. Otherwise -1.
*/
int team_addPlayer(struct Team *team, struct Player *player);

/**
* @brief Function for removing a player from a team.
*
* @param team The team from that is remove the player.
* @param player The player that will be removed.
*
* @return 0 If the player was removed from a team. Otherwise -1.
*/
int team_removePlayer(struct Team *team, struct Player *player);

/**
 * @brief Function for deleting a Team. Sets pointer to NULL.
 *
 * @param team The team to be freed.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int team_deleteTeam(struct Team **team);

/**
 * @brief Function for deleting a Player. Sets pointer to NULL.
 *
 * @param player The player to be freed.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int team_deletePlayer(struct Player **player);

/**
 * @brief function for calculating the score of a team
 *
 * @param team The team to be computed
 *
 * @return int value representing the score, other value on failure.
 */
int team_computeScore(const struct Team *team);

/**
* @brief Function for adding a card of a player. The function doesn't check
*        if the card has valid value and valid suit. 
*
* @param player The player who receives the card.
* @param card The card to be received.
*
* @return NO_ERROR or 0 on success. Other value on failure.
*/
int team_addCard(struct Player *player, struct Card *card);

#endif
