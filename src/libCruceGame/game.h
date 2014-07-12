/**
 * @file game.h
 * @brief Game structures, as well as helper functions.
 */

#ifndef GAME_H
#define GAME_H

#include "platform.h"
#include "constants.h"
#include "team.h"
#include "deck.h"
#include "round.h"

/**
 * @struct Game
 * @brief Game structure.
 * 
 * Structure used to keep information about the game data.
 * 
 * @var Game::numberPlayers
 *     The number of the players that joined the game.
 * @var Game::pointsNumber
 *     The total amount of the points of the game.
 * @var Game::round
 *     Pointer to the current round of the game.
 * @var Game::players
 *     Pointer to the players of the game.
 * @var Game::teams
 *     Pointer to the teams of the game.
 * @var Game::deck
 *     Pointer to the deck of the game.
 */
struct Game {
    int numberPlayers;
    int pointsNumber;
    struct Round *round;
    struct Player *players[MAX_GAME_PLAYERS];
    struct Team *teams[MAX_GAME_TEAMS];
    struct Deck *deck;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocates memory for and initializes a game.
 *
 * @param numberPoints The number of points required for winning the game.
 *
 * @return Pointer to the new game on success or NULL on failure.
 */
EXPORT struct Game *game_createGame(const int numberPoints);

/**
 * @brief Frees the memory of a game and makes the pointer NULL.
 *
 * @param game Pointer to the game to be deleted.
 *
 * @return \ref NO_ERROR on success, other value on failure.
 */
EXPORT int game_deleteGame(struct Game **game);

/**
 * @brief Adds a player to a game.
 *
 * @param player The player to be added.
 * @param game The game where the player is to be added.
 *
 * @return \ref NO_ERROR on success, other value on failure.
 */
EXPORT int game_addPlayer(struct Player *player, struct Game *game);

/**
 * @brief Removes a player from a game.
 *
 * @param player The player to be removed.
 * @param game The game from where the player is to be removed.
 *
 * @return \ref NO_ERROR on success, other value on failure.
 */
EXPORT int game_removePlayer(const struct Player *player, struct Game *game);

/**
 * @brief Adds a team to a game.
 *
 * @param team The team to be added.
 * @param game The game where the team is to be added to.
 *
 * @return \ref NO_ERROR on success, other value on failure.
 */
EXPORT int game_addTeam(struct Team *team, struct Game *game);

 
/**
 * @brief Removes a team from a game.
 *
 * @param team The team to be removed.
 * @param game The game from where the team is to be removed.
 *
 * @return \ref NO_ERROR on success, error code otherwise.
 */
EXPORT int game_removeTeam(const struct Team *team, struct Game *game);

/**
 * @brief Searches the winning team of a game.
 *
 * @param game The game in which the winning team is to be search.
 *
 * @return Pointer to the winner team on success or NULL on failure.
 */
EXPORT struct Team *game_winningTeam(struct Game *game);

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
EXPORT int game_checkCard(struct Player *player, const struct Game *game,
                          struct Hand *hand, const int idCard);

/**
 * @brief Function to find the next allowed card after currentCard.
 *        Uses game_checkCard to check if a card is allowed.
 *
 * @param player The player who has the cards.
 * @param game The game where the player is located.
 * @param hand The hand in which should put the card.
 * @param currentCard Function finds the first allowed card after currentCard.
 *
 * @return The next allowed card's id on success (value between 0 and
 *         \ref MAX_CARDS -1). Negative value on failure.
 */
EXPORT int game_findNextAllowedCard(struct Player *player,
                                    const struct Game *game,
                                    struct Hand *hand, int currentCard);

/**
 * @brief Function to find the previous allowed card before currentCard.
 *        Uses game_checkCard to check if a card is allowed.
 *
 * @param player The player who has the cards.
 * @param game The game where the player is located.
 * @param hand The hand in which should put the card.
 * @param currentCard Function finds the last allowed card before currentCard.
 *
 * @return The next allowed card's id on success (value between 0 and
 *         \ref MAX_CARDS -1). Negative value on failure.
 */
EXPORT int game_findPreviousAllowedCard(struct Player *player,
                                        const struct Game *game,
                                        struct Hand *hand,
                                        int currentCard);

/**
 * @brief Function to find the team in which a player belongs.
 *
 * @param game The game where to search.
 * @param player The player to find.
 *
 * @return Pointer to the required team on success, NULL on failure.
 */
EXPORT struct Team *game_findTeam(const struct Game *game, struct Player *player);

/**
 * @brief Function to update the score of teams and players after a round.
 *
 * @param game Game where to update the scores.
 * @param bidWinner Pointer to the player that won the bid in the last round.
 *
 * @return \ref NO_ERROR or 0 on success, other value on failure.
 */
EXPORT int game_updateScore(const struct Game *game, struct Player *bidWinner);

/**
 * @brief Function to add a round to a game and to arrange players into it,
 *        according to game rules.
 *
 * @param game The game to process.
 * @param i The index of the first player in that round.
 *
 * @return \ref NO_ERROR or 0 on success, other value on failure.
 */
EXPORT int game_arrangePlayersRound(struct Game *game, const int i);

#ifdef __cplusplus
}
#endif

#endif

