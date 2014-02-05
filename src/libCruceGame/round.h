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
 * @brief Function to find the bid winner in a round.
 *
 * @param round Pointer to the round where to find the bid winner.
 *
 * @return Pointer to the bid winner player. Otherwise NULL.
 */
struct Player *round_getBidWinner(const struct Round *round);

/**
 * @brief Function to place bid for a player.
 *
 * @param player Pointer to the player who places the bid.
 * @param bid The value of the bid.
 * @param round Pointer to the round where to place the bid.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int round_placeBid(const struct Player *player, int bid, struct Round *round);

/**
 * @brief Function to add player to a round.
 *
 * @param player Pointer to the Player to be added.
 * @param round Pointer to the round where to add player.
 * 
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int round_addPlayer(struct Player* player, struct Round *round);

/**
 * @brief Function to add player to a hand.
 *
 * @param player Pointer to the player to be added.
 * @param hand Pointer to the hand where to add player.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int round_addPlayerHand(struct Player *player, struct Hand *hand);

/**
 * @brief Function to give cards from players.
 *
 * @param player Pointer to the player who gives the card.
 * @param cardId Card id given by player (id from Player.cards).
 * @param hand Pointer to the hand in which the card is given.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int round_putCard(struct Player *player, int cardId, struct Hand *hand);
 
/**
 * @brief Function to find the score of the hand (in game points).
 *
 * @param hand Pointer to the hand to compute score.
 *
 * @return positive or zero on success, negative on failure.
 *
 */
int round_computeScore(const struct Hand *hand);

/**
 * @brief Function to allocate memory for and initialize a Round.
 *
 * @return Pointer to the new round on success, NULL otherwise.
 */
struct Round *round_createRound();

/**
 * @brief Function to free memory of a Round. Makes pointer NULL.
 *
 * @param round Pointer to the round to be freed.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int round_deleteRound(struct Round **round);

/**
 * @brief Function to allocate memory for and initialize a Hand.
 *
 * @return Pointer to the new Hand on success, NULL otherwise.
 */
struct Hand *round_createHand();

/**
 * @brief Function to free memory of a hand. Makes pointer NULL.
 *
 * @param hand Pointer to the hand to be freed.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int round_deleteHand(struct Hand **hand);

/**
 * @brief Function to remove a player from a round.
 *
 * @param player Pointer to the player to be removed.
 * @param round Pointer to the round from where to be removed.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 */
int round_removePlayer(struct Player *player, struct Round *round);

/**
 * @brief Function to remove a player from a hand.
 * 
 * @param player Pointer to the player to be removed.
 * @param hand Pointer to the hand from where to be removed.
 *
 * @return NO_ERROR or 0 on success, other value on failure.
 *
 */
int round_removePlayerHand(struct Player *player, struct Hand *hand);

/**
* @brief Function for determining of the hand winner.
*
* @param hand Pointer to the hand that determines the winner.
* @param trump Is the trump of round.
*
* @return Pointer to the winner player. Otherwise NULL.
*/
struct Player *round_handWinner(const struct Hand *hand, enum Suit trump);

/**
* @brief The function distributes one card to every player.
*
* @param deck Pointer to the deck from where it distributes.
* @param round Pointer to the round containing the  players to receive the cards.
*
* @return NO_ERROR or 0 on success, other value on failure.
*/
int round_distributeCard(struct Deck *deck,const struct Round *round);

/**
* @brief Function for distributing the cards to the players.
*
* @param deck Pointer to the deck to be distributed.
* @param hand Pointer to the round that deck is distributed to.
*
* @return NO_ERROR or 0 on success, other value on failure.
*/
int round_distributeDeck(struct Deck *deck,const struct Round *round);

#endif

