#include "round.h"

#include <stdlib.h>
#include <stdio.h>

struct Round *round_newRound(enum Suit trump)
{
    struct Round *round = malloc(sizeof(struct Round));

    if (round == NULL)
        return NULL;

    round->trump = trump;

    for (int i = 0; i < MAX_HANDS + 1; i++)
        round->hands[i] = NULL;

    return round;
}

struct Player *round_getBidWinner(struct Hand *hand)
{
    if (hand == NULL)
        return NULL;

    int maxBidIndex = 0;
    for (int i = 0; hand->players[i] != NULL; i++) {
        if (hand->bids[i] > hand->bids[maxBidIndex]) {
            maxBidIndex = i;
        }
    }
    return hand->players[maxBidIndex];
}

/**
 * @brief Helper to find player in a hand.
 *
 * @param player Player to find.
 * @param hand Hand to search for player.
 * @return 0 on success, non-zero on failure.
 */
int findPlayerIndexHand(struct Player *player, struct Hand *hand)
{
    if (player == NULL || hand == NULL)
        return -1;

    int i = 0;
    while (hand->players[i] != player && hand->players[i] != NULL)
        i++;

    if (hand->players[i] == NULL)
        return -1;

    return i;
}

int round_placeBid(struct Player *player, int bid, struct Hand *hand)
{
    if (player == NULL || hand == NULL)
        return -1;
    if (bid < 0)
        return -1;

    int index = findPlayerIndexHand(player, hand);;

    if(index < 0)
        return -1;

    hand->bids[index] = bid;

    return 0;
}

int round_addPlayer(struct Player *player, struct Hand *hand)
{
    if (player == NULL || hand == NULL)
        return -1;

    int index = findPlayerIndexHand(player, hand);
    if(index >= 0) //impossible to add same player multimple time
        return -1;

    int i = 0;
    while(hand->players[i] != NULL)
        i++;

    hand->players[i]     = player;
    hand->players[i + 1] = NULL;

    return 0;
}

int round_giveCard(struct Player *player, int cardId, struct Hand *hand)
{
    if (player == NULL || player->hand[cardId] == NULL || hand == NULL)
        return -1;

    int index = findPlayerIndexHand(player, hand);

    if(index < 0)
        return -1;

    hand->cards[index] = player->hand[cardId];
    player->hand[cardId] = NULL;

    return 0;
}

int round_computeScore(struct Hand *hand)
{
    if (hand == NULL)
        return -1;

    int cardsScore = 0;
    for (int i = 0; hand->players[i] != NULL; i++) {
        if (hand->cards[i] == NULL)
            return -1;
        cardsScore += hand->cards[i]->value;
    }

    int gameScore = cardsScore / 33;

    return gameScore;
}

