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

#endif

