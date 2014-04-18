#ifndef CLI_H
#define CLI_H

/**
 * @file cli.h
 * @brief Contains declaration of the functions used for building the user
 *        interface.
 */

#include <cruceGame.h>
#include <ncurses.h>

/**
 * @brief Function to display a greeting message at the beginning of a new
 *        game.
 * 
 * @return void.
 */
void welcomeMessage();

/**
 * @brief Function to display the enfing message and the members of
 *        the winning team.
 *
 * @param team Pointer o the winning team.
 *
 * @return void
 */
 void gameEndingMessage(struct Team *team);

/**
 * @brief Function to print a Card.
 *
 * @param card Pointer to the card to be printed.
 * @param frameColor The color with which is colored the frame of card.
 * @param win The window in which to display the card.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int printCard(struct Card *card, int frameColor, WINDOW *win);

/**
 * @brief Function to print all cards in a player's hand.
 *
 * @param player The player whose cards to be printed.
 * @param game The game which contains the player.
 * @param selected The position of the selected card. If there is no card at
 *                 that position (there is a NULL card or it is out of range) 
 *                 no card will be selected.
 * @param win The window in which to display the cards.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int printPlayerCards(struct Game *game, struct Player *player, int selecte,
                     WINDOW *win);

/**
 * @brief Function to get the number of players from user.
 *
 * @return The number of players.
 */
int getNoOfPlayers();

/**
 * @brief Function to get user data and create a new Player.
 *
 * @param i The player number.
 *
 * @return Pointer to the new Player.
 */
struct Player *newPlayer(int i);

/**
 * @brief Function to print the score table.
 *
 * @param game Pointer to the game of where to be printed the score.
 * @param round Pointer to the round of where to be printed the points.
 * @param win Pointer to the window in which to display the score.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int printScore(struct Game *game, struct Round *round, WINDOW *win);

/**
 * @brief Function to form the teams.
 *
 * @param game Pointer to the game where form it the teams.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int formTeams(struct Game* game);

/**
 * @brief Function displays the trump, the cards on table, the player's cards
 *        and puts the player to pick a card.
 *
 * @param game Pointer to the game where to be the player and the table.
 * @param playerId The player's id to whom displays the cards.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int displayCardsAndPickCard(struct Game *game, int playerId);

/*
 * @brief Function to display player's cards and ask for a bid.
 *
 * @param game Pointer to the Game.
 * @param playerId Id of the bidding player.
 *
 * @return NO_ERROR or 0 un success, other value on failure.
 */
int getBid(struct Game* game, int playerId);

/**
 * @brief Function to display the team who wins
 *
 * @param winner The team who wins
 *
 * @return NO_ERROR on success, otherwise POINTER_NULL on failure
 */
int displayWinner(struct Team *winner);

/**
 * @brief Function to get the score limit from user.
 *
 * @return The score limit (11-15-21).
 */
int getScoreLimit();

/**
 * @brief Prints the score differences between rounds.
 *
 * @param currentGame The current game.
 * @param oldScore Old score for every player before the terminated round. 
 *
 * @return int The size of the biggest name of a player in a round.
 */
int printRoundTerminationMessage(struct Game *currentGame, int *oldScore);

/**
 * @brief Displays how much bid each player.
 *
 * @param round The round in which are the players.
 * @param currentPlayer The player which have to bid.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int displayBids(struct Round *round, int currentPlayer);

#endif
