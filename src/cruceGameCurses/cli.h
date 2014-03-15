#include <cruceGame.h>

/**
 * @brief Function to display a greeting message at the beginning of a new
 *        game.
 * 
 * @return void.
 */
void welcomeMessage();

/**
 * @brief Function to print a Card.
 *
 * @param card Pointer to the card to be printed.
 * @param position Position of the card on screen (in cards).
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int printCard(struct Card *card, int position);

/**
 * @brief Function to print all cards in a player's hand.
 *
 * @param player The player whose cards to be printed.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int printPlayerCards(struct Player *player);

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
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int printScore(struct Game *game, struct Round *round);

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
 * @brief Function to display the player who wins
 *
 * @param winner The player who wins
 *
 * @return NO_ERROR on success, otherwise POINTER_NULL on failure
 */
int displayWinner(struct Team *winner);

