/**
 * @file player.h
 * @brief Player structure, with helper functions.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "deck.h"

/*
 * @brief The maximum number of cards in a player's hand.
 */
#define MAX_CARDS 8

/*
 * @brief The maximum number of players in a team.
 */
#define MAX_PLAYERS 2

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
    struct Player *players[MAX_PLAYERS];
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
* @param player The player that to be added in the team.
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

#endif
