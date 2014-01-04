/**
 * @file round.h
 * @brief Round and Hand structures, as well as helper functions.
 */

#ifndef ROUND_H
#define ROUND_H

#define MAX_HANDS 8
#define MAX_GAME_PLAYERS 4

#include "deck.h"
#include "team.h"

/**
 * @brief Hand structure.
 *
 * In a hand, player[i] gives cards[i] and bids bid[i].
 * The players array should use NULL as terminator.
 * The players should be added in the order of the bids.
 */
struct Hand{
    struct Card *cards[MAX_GAME_PLAYERS + 1];
    struct Player *players[MAX_GAME_PLAYERS + 1];
    int bids[MAX_GAME_PLAYERS];
};

/**
 * @brief Round structure.
 * 
 * hands uses NULL as terminator.
 */
struct Round{
    int id;
    enum Suit trump;
    struct Hand *hands[MAX_HANDS + 1];
};

/**
 * @brief function to find the bid winner in a hand.
 *
 * @param hand The Hand where to find the bid winner.
 * @return The bid winner.
 */
struct Player *getBidWinner(struct Hand *hand);

/**
 * @brief Function to place bid for a player.
 *
 * @param player The player who places the bid.
 * @param bid The value of the bid.
 * @param hand The hand where to place the bid.
 * @return 0 on success, non-zero on failure.
 */
int placeBid(struct Player *player, int bid, struct Hand *hand);

/**
 * @brief Function to add player to a hand.
 *
 * @param hand Hand where to add player.
 * @param player Player to be added.
 * @return 0 on success, non-zero on failure.
 */
int addPlayer(struct Player* player, struct Hand *hand);

/**
 * @brief Function to give cards from players.
 *
 * @param player Player who gives the card.
 * @param card Card id given by player (id from Player.cards).
 * @param hand Hand in which the card is given.
 * @return 0 on success, non-zero on failure.
 */
int giveCard(struct Player *player, int cardId, struct Hand *hand);
 
/**
 * @brief Function to find the score of the hand (in game points).
 *
 * @param hand Hand to compute score.
 * @return score on success, negative on failure.
 */
int computeScore(struct Hand *hand);

#endif

