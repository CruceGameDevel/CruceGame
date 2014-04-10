/**
 * @file team.h
 * @brief Player and Team structures, with helper functions.
 */

#ifndef TEAM_H
#define TEAM_H

#include "deck.h"
#include "round.h"

/**
 * @struct Player
 * @brief Player structure.
 *
 * Structure to keep relevant informations about the players.
 *
 * @var Player::id
 *     Identifier of the player.
 * @var Player::name
 *     Pointer to the name of the player.
 * @var Player::hand
 *     Pointer to the cards of the player.
 * @var Player::score
 *     The points achieved by the player in this game. It is equal with
 *     team points, if the game is played in teams.
 * @var Player::isHuman
 *     Flag used to indicate if the player is human or robot.
 */
struct Player{
    int id;
    char *name;
    struct Card *hand[MAX_CARDS];
    int score;
    int isHuman; //0 for AI, non-zero for human.
};

/**
 * @struct Team
 * @brief Team structure.
 *
 * Players are grouped in teams. One team for 2-3 players,
 * and two teams for 4 players.
 *
 * @var Team::id
 *     The identifier of the team.
 * @var Team::score
 *     The score achieved by the team in this game.
 * @var Team::name
 *     Pointer to the name of the team.
 * @var Team::players
 *     Pointer to the players of the team.
 */
struct Team{
    int id;
    int score;
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

/**
 * @brief Updates all players scores, assigning team score to them.
 *
 * @param team The team where to update the score.
 *
 * @return NO_ERROR or 0 on success, negative value otherwise.
 */
EXPORT int team_updatePlayersScore(struct Team *team);

/**
 * @brief Compute round points of a team.
 *
 * @param team The team to compute points for.
 * @param round The round where to compute points.
 *
 * @return Number of round points on success, negative value otherwise.
 */
EXPORT int team_computePoints(struct Team *team, struct Round *round);
#ifdef __cplusplus
}
#endif

#endif

