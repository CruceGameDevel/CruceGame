/**
 * @file round.h
 * @brief Round and Hand structures, as well as helper functions.
 */

#ifndef ROUND_H
#define ROUND_H

#include "deck.h"
#include "team.h"
#include "constants.h"
#include "errors.h"

/**
 * @brief Hand structure.
 *
 * In a hand, player[i] gives cards[i] and bids bid[i].
 * The players should be added in the order of the bids.
 */
struct Hand{
    struct Card *cards[MAX_GAME_PLAYERS];
    struct Player *players[MAX_GAME_PLAYERS];
};

/**
 * @brief Round structure.
 */
struct Round{
    int id;
    enum Suit trump;
    struct Hand *hands[MAX_HANDS];
    int bids[MAX_GAME_PLAYERS];
    struct Player *players[MAX_GAME_PLAYERS];
};

/**
 * @brief Finds the winner of a bid in a round.
 *
 * @param round Pointer to the round where to find the bid winner.
 *
 * @return Pointer to the bid winner player on success or NULL on failure.
 */
struct Player *round_getBidWinner(const struct Round *round);

/**
 * @brief Places the bid of a player.
 *
 * @param player Pointer to the player who places the bid.
 * @param bid The value of the bid.
 * @param round Pointer to the round where to place the bid.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
int round_placeBid(const struct Player *player, int bid, struct Round *round);

/**
 * @brief Add a player to a round.
 *
 * @param player Pointer to the player to be added.
 * @param round Pointer to the round where to add player.
 * 
 * @return NO_ERROR on success, error code otherwise.
 */
int round_addPlayer(struct Player* player, struct Round *round);

/**
 * @brief Adds a player to a hand.
 *
 * @param player Pointer to the player to be added.
 * @param hand Pointer to the hand where the player is to be added.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
int round_addPlayerHand(struct Player *player, struct Hand *hand);

/**
 * @brief Places a card from a player to a hand.
 *
 * @param player Pointer to the player who places the card.
 * @param cardId Id of the card placed by the player (id from Player.cards).
 * @param hand Pointer to the hand in which the card is placed.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
int round_putCard(struct Player *player, int cardId, struct Hand *hand);
 
/**
 * @brief Computes the score of a hand (in game points).
 *
 * @param hand Pointer to the hand for which the score is computed.
 *
 * @return Integer representing the score or negative error code on failure.
 *
 */
int round_computeScore(const struct Hand *hand);

/**
 * @brief Allocates memory for and initializes a round.
 *
 * @return Pointer to the new round on success or NULL on failure.
 */
struct Round *round_createRound();

/**
 * @brief Frees the memory of a round. Makes pointer NULL.
 *
 * @param round Pointer to pointer to the round to be deleted.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
int round_deleteRound(struct Round **round);

/**
 * @brief Allocates memory for and initializes a hand.
 *
 * @return Pointer to the new hand on success or NULL on failure.
 */
struct Hand *round_createHand();

/**
 * @brief Frees the memory of a hand. Makes pointer NULL.
 *
 * @param hand Pointer to pointer to the hand to be deleted.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
int round_deleteHand(struct Hand **hand);

/**
 * @brief Removes a player from a round.
 *
 * @param player Pointer to the player to be removed.
 * @param round Pointer to the round from which the player is removed.
 *
 * @return NO_ERROR on success, error code otherwise.
 */
int round_removePlayer(struct Player *player, struct Round *round);

/**
 * @brief Removes a player from a hand.
 * 
 * @param player Pointer to the player to be removed.
 * @param hand Pointer to the hand from where the player is removed.
 *
 * @return NO_ERROR on success, error code otherwise.
 *
 */
int round_removePlayerHand(struct Player *player, struct Hand *hand);

/**
* @brief Determines the winner of a hand.
*
* @param hand Pointer to the hand.
* @param trump The trump of round.
*
* @return Pointer to the winning player or NULL on failure.
*/
struct Player *round_handWinner(const struct Hand *hand, enum Suit trump);

/**
* @brief Distributes one card to every player.
*
* @param deck Pointer to the deck from where cards are distributed.
* @param round Pointer to the round containing the players that receive the cards.
*
* @return NO_ERROR on success, error code otherwise.
*/
int round_distributeCard(struct Deck *deck, const struct Round *round);

/**
* @brief Distributes cards to players.
*
* @param deck Pointer to the deck from where cards are distributed.
* @param hand Pointer to the round that deck is distributed to.
*
* @return NO_ERROR on success, error code otherwise.
*/
int round_distributeDeck(struct Deck *deck, const struct Round *round);

#endif

