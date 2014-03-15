/**
 * @file player.h
 * @brief Player structure, with helper functions.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "deck.h"

/**
 * @brief Player structure.
 *
 * Structure to keep relevant informations about the players.
 *
 * @var id Identifier of the player.
 * @var name Pointer to the name of the player.
 * @var hand Pointer to the cards of the player.
 * @var score The amount of points earned in a hand.
 * @var isHuman Flag used to indicate if the player is human or robot.
 */
struct Player{
    int id;
    char *name;
    struct Card *hand[MAX_CARDS];
    int score;
    int isHuman; //0 for AI, non-zero for human.
};

/**
 * @brief Team structure.
 *
 * Players are grouped in teams. One team for 2-3 players,
 * and two teams for 4 players.
 *
 * @var id The identifier of the team.
 * @var name Pointer to the name of the team.
 * @var players Pointer to the players of the team.
 */
struct Team{
    int id;
    char *name;
    struct Player *players[MAX_TEAM_PLAYERS];
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Creates a player.
 *
 * @param name The name of the new player.
 * @param isHuman Player type.
 *
 * @return Pointer to the created player. Needs to be freed.
 */
EXPORT struct Player *team_createPlayer(const char *name, int isHuman);

/**
 * @brief Creates a team.
 *
 * @param name The name of the new team.
 *
 * @return Pointer to the created team. Needs to be freed.
 */
EXPORT struct Team *team_createTeam(const char *name);

/**
* @brief Adds a player to a team.
*
* @param team The team to which the player is added.
* @param player The player to be added to the team.
*
* @return NO_ERROR on success, error code otherwise.
*/
EXPORT int team_addPlayer(struct Team *team, struct Player *player);

/**
* @brief Removes a player from a team.
*
* @param team The team from where the player is removed.
* @param player The player that will be removed.
*
* @return NO_ERROR on success, error code otherwise.
*/
EXPORT int team_removePlayer(struct Team *team, const struct Player *player);

/**
 * @brief Deletes a team and sets the pointer to NULL.
 *
 * @param team The team to be deleted.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
EXPORT int team_deleteTeam(struct Team **team);

/**
 * @brief Deletes a player and sets the pointer to NULL.
 *
 * @param player The player to be deleted.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
EXPORT int team_deletePlayer(struct Player **player);

/**
 * @brief Calculates the score of a team
 *
 * @param team The team for which the score is to be be calculated
 *
 * @return Integer representing the score or negative error code on failure.
 */
EXPORT int team_computeScore(const struct Team *team);

/**
* @brief Passes a card to a player. The function doesn't check
*        if the card has valid value and valid suit. 
*
* @param player The player who receives the card.
* @param card The card to be received.
*
* @return NO_ERROR on success, error code otherwise.
*/
EXPORT int team_addCard(struct Player *player, struct Card *card);

/**
* @brief Checks if a player has any card
* 
* @param player Pointer to the player that is ckecked
*
* @return 1 in case of succes, 0 otherwise
*/
EXPORT int team_hasCards(struct Player *player);

#ifdef __cplusplus
}
#endif

#endif
