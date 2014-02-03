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

