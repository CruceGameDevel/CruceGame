/**
 * @file round.c
 * @brief Contains implementations of the functions used for creation or
 *        deletion of a round. Also, it contains implementations for the 
 *        functions used to handle different activities in a round, like
 *        removing a player.
 */

#include "round.h"
#include <errors.h>

#include <stdlib.h>
#include <stdio.h>

struct Round *round_createRound()
{
    struct Round *round = malloc(sizeof(struct Round));

    if (round == NULL)
        return NULL;

    for (int i = 0; i < MAX_HANDS; i++)
        round->hands[i] = NULL;

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        round->bids[i] = 0;
        round->players[i] = 0;
    }

    round->trump = SuitEnd;

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        round->pointsNumber[i] = 0;

    return round;
}

int round_deleteRound(struct Round **round)
{
    if (round == NULL)
        return POINTER_NULL;
    if (*round == NULL)
        return ROUND_NULL;

    free(*round);
    *round = NULL;

    return NO_ERROR;
}

struct Hand *round_createHand()
{
    struct Hand *hand = malloc(sizeof(struct Hand));
    if (hand == NULL)
        return NULL;

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        hand->cards[i]   = NULL;
        hand->players[i] = NULL;
    }

    return hand;
}

int round_deleteHand(struct Hand **hand)
{
    if (hand == NULL)
        return POINTER_NULL;
    if (*hand == NULL)
        return HAND_NULL;

    free(*hand);
    *hand = NULL;

    return NO_ERROR;
}

struct Player *round_getBidWinner(const struct Round *round)
{
    if (round == NULL)
        return NULL;

    int maxBidIndex = 0;
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (round->players[i] != NULL &&
            round->bids[i] > round->bids[maxBidIndex]) {
            maxBidIndex = i;
        }
    }
    return round->players[maxBidIndex];
}

int round_findPlayerIndexRound(const struct Player *player, 
                                const struct Round *round)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (round == NULL)
        return ROUND_NULL;

    int i = 0;
    while (i < MAX_GAME_PLAYERS && round->players[i] != player)
        i++;

    if (i == MAX_GAME_PLAYERS)
        return NOT_FOUND;

    return i;
}

int round_placeBid(const struct Player *player, const int bid, 
                    struct Round *round)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (round == NULL)
        return ROUND_NULL;
    if (bid < 0 || bid > 6)
        return ILLEGAL_VALUE;

    if (bid > 0) {
        for(int i = 0; i < MAX_GAME_PLAYERS; i++) {
            if(round->bids[i] > bid)
                return ILLEGAL_VALUE;
        }
    }

    int index = round_findPlayerIndexRound(player, round);

    if(index < 0)
        return NOT_FOUND;

    round->bids[index] = bid;

    return NO_ERROR;
}

int round_addPlayer(struct Player *player, struct Round *round)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (round == NULL)
        return ROUND_NULL;

    int index = round_findPlayerIndexRound(player, round);
    if(index >= 0) //impossible to add same player multiple time
        return DUPLICATE;

    int i = 0;
    while(round->players[i] != NULL && i < MAX_GAME_PLAYERS)
        i++;

    if (i == MAX_GAME_PLAYERS)
        return FULL;

    round->players[i] = player;

    return NO_ERROR;
}

int round_addPlayerHand(struct Player *player, struct Hand *hand)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (hand == NULL)
        return HAND_NULL;

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        if (hand->players[i] == player)
            return DUPLICATE;

    int i = 0;
    while(hand->players[i] != NULL && i < MAX_GAME_PLAYERS)
        i++;

    if (i == MAX_GAME_PLAYERS)
        return FULL;

    hand->players[i] = player;

    return NO_ERROR;
}

int round_removePlayer(const struct Player *player, struct Round *round)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (round == NULL)
        return ROUND_NULL;

    int index = round_findPlayerIndexRound(player, round);

    if (index < 0)
        return NOT_FOUND;

    round->players[index] = NULL;

    return NO_ERROR;
}

int round_removePlayerHand(const struct Player *player, struct Hand *hand)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (hand == NULL)
        return HAND_NULL;

    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        if (hand->players[i] == player) {
            hand->players[i] = NULL;
            return NO_ERROR;
        }

    return NOT_FOUND;
}

int round_putCard(struct Player *player, const int cardId,
                  const int handId, struct Round *round)
{
    if (player == NULL)
        return PLAYER_NULL;
    if (player->hand[cardId] == NULL)
        return CARD_NULL;
    if (round == NULL)
        return ROUND_NULL;
    if (round->hands[handId] == NULL)
        return HAND_NULL;

    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if (round->hands[handId]->players[i] == player){
            round->hands[handId]->cards[i] = player->hand[cardId];
            enum Suit suit = player->hand[cardId]->suit;
            int value = player->hand[cardId]->value;
            player->hand[cardId] = NULL;
            if (i == 0 && (value == 3 || value == 4)) {
                int check = 0;
                for (int j = 0; j < MAX_CARDS; j++) {
                    if (player->hand[j] != NULL &&
                       (player->hand[j]->value == 3 ||
                        player->hand[j]->value == 4) &&
                        suit == player->hand[j]->suit) {
                        check = 1;
                    }
                }
                if (check == 1) {
                    int position = round_findPlayerIndexRound(player, round);
                    if (suit == round->trump)
                        round->pointsNumber[position] += 40;
                    else
                        round->pointsNumber[position] += 20;
                }
            }
            return NO_ERROR;
        }
    }

    return NOT_FOUND;
}

/**
 * @brief Function to compute the number of points in a hand.
 *
 * @param hand The hand where to compute the number of points.
 *
 * @return The number of points in hand on succes, negative otherwise.
 */
int totalPointsNumber(const struct Hand *hand)
{
    if (hand == NULL)
        return HAND_NULL;

    int points = 0;
    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        if (hand->players[i] != NULL && hand->cards[i] != NULL)
            points += hand->cards[i]->value;

    return points;
}

struct Player *round_handWinner(const struct Hand *hand, struct Round *round)
{
    if (hand == NULL || round == NULL || round->trump == SuitEnd)
        return NULL;

    enum Suit trump = round->trump;
    int playerWinner = -1;
    int numberPlayers = 0;
    for (int i = 0; i < MAX_GAME_PLAYERS; i++) {
        if ((hand->players[i] != NULL && hand->cards[i] == NULL) ||
            (hand->players[i] == NULL && hand->cards[i] != NULL))
            return NULL;
        if (hand->players[i] != NULL && hand->cards[i] != NULL) {
            numberPlayers++;
            if (playerWinner == -1)
                playerWinner = i;
        }
    }
    if (playerWinner == -1 || numberPlayers == 1)
        return NULL;

    int cardWinner;
    for (int i = playerWinner + 1; i < MAX_GAME_PLAYERS; i++) {
        if (hand->players[i] != NULL && hand->cards[i] != NULL) {
            cardWinner = deck_compareCards(hand->cards[playerWinner],
                                               hand->cards[i], trump);
            if (cardWinner == 2)
                playerWinner = i;
            if (cardWinner <= 0)
                return NULL;
        }
    }

    int playerWinner_inRound = 
        round_findPlayerIndexRound(hand->players[playerWinner], round);
    round->pointsNumber[playerWinner_inRound] += totalPointsNumber(hand);
    return hand->players[playerWinner];
}

int round_distributeCard(struct Deck *deck, const struct Round *round)
{
    if (deck == NULL)
        return DECK_NULL;
    if (round == NULL)
        return ROUND_NULL;

    int i, j;
    int distributedCards = 0;
    for (i = 0, j = 0; i < MAX_GAME_PLAYERS && j < DECK_SIZE; i++, j++) {
        //do not change while's order
        while (deck->cards[j] == NULL && j < DECK_SIZE)
            j++;
        while (round->players[i] == NULL && i < MAX_GAME_PLAYERS)
            i++;
        if (i < MAX_GAME_PLAYERS && j < DECK_SIZE) {
            int checkError = team_addCard(round->players[i], deck->cards[j]);
            if (checkError != NO_ERROR)
                return checkError;
            deck->cards[j] = NULL;
            distributedCards++;
        }
    }

    if (distributedCards == 0 && j == DECK_SIZE + 1)
        return DECK_EMPTY;
    if (distributedCards == 0 && i == MAX_GAME_PLAYERS + 1)
        return ROUND_EMPTY;
    if (distributedCards == 1 && i == MAX_GAME_PLAYERS + 1)
        return INSUFFICIENT_PLAYERS;
    if (distributedCards == 1 && j == DECK_SIZE + 1)
        return LESS_CARDS;

    return NO_ERROR;
}

int round_distributeDeck(struct Deck *deck, const struct Round *round)
{
    if (deck == NULL)
        return DECK_NULL;
    if (round == NULL)
        return ROUND_NULL;

    int numberPlayers = 0;
    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        if (round->players[i] != NULL)
            numberPlayers++;

    if (numberPlayers == 1)
        return INSUFFICIENT_PLAYERS;
    if (numberPlayers == 0)
        return ROUND_EMPTY;

    for (int i = 0; i < MAX_CARDS && i < DECK_SIZE / numberPlayers; i++) {
        int distributeCard = round_distributeCard(deck, round);
        if (distributeCard != NO_ERROR)
            return distributeCard;
    }

    return NO_ERROR;
}

int round_arrangePlayersHand(struct Round *round, int i)
{
    if (round == NULL)
        return ROUND_NULL;
    if (i < 0 || i >= MAX_GAME_PLAYERS)
        return ILLEGAL_VALUE;

    int handId = 0;
    while (round->hands[handId] != NULL)
        handId++;

    if (handId >= MAX_HANDS)
        return FULL;

    struct Hand *hand = round_createHand();
    for (int j = i; j < i + MAX_GAME_PLAYERS; j++)
        if (round->players[j % MAX_GAME_PLAYERS] != NULL)
            round_addPlayerHand(round->players[j % MAX_GAME_PLAYERS], hand);

    round->hands[handId] = hand;

    return NO_ERROR;
}

int round_computePoints(const struct Team *team, const struct Round *round)
{
    if (team == NULL)
        return TEAM_NULL;
    if (round == NULL)
        return ROUND_NULL;

    int playersNumber = 0;
    int points = 0;
    for (int i = 0; i < MAX_TEAM_PLAYERS; i++)
        if (team->players[i] != NULL) {
            playersNumber++;
            int j = round_findPlayerIndexRound(team->players[i], round);
            if (j < 0)
                return j;
            points += round->pointsNumber[j];
        }

    if (playersNumber == 0)
        return TEAM_EMPTY;

    return points;
}

int round_getMaximumBid(struct Round *round)
{
    if (round == NULL)
        return ROUND_NULL;

    int maximumBid = 0;
    for (int i = 0; i < MAX_GAME_PLAYERS; i++)
        if (round->players[i] != NULL)
            if (round->bids[i] > maximumBid)
                maximumBid = round->bids[i];

    return maximumBid;
}

/**
 * @brief Helper for findAllowedBid().)
 *
 * @param currentBid The bid which is bid.
 * @param maximumBid Maximum bid from auction.
 *
 * @return 1 if the current bid can be bid, otherwise 0.
 */
int checkBid(int currentBid, int maximumBid)
{
    if (currentBid > maximumBid || currentBid == 0 )
        return 1;

    return 0;
}

/**
 * @brief Helper for functions which search a allowed bid.
 *
 * @param round The round in which are the bids.
 * @param currentBid The bid which is bid.
 * @param searchPattern This parameter indicate where to search (-1 to left,
 *                      1 to right).
 *
 * @return The first allowed bid found.
 */
int findAllowedBid(struct Round *round, int currentBid, int searchPattern)
{
    if (round == NULL)
        return ROUND_NULL;
    if (currentBid < 0 || currentBid > 6)
        return ILLEGAL_VALUE;
    if (searchPattern != 1 && searchPattern != -1)
        return ILLEGAL_VALUE;

    currentBid += searchPattern;
    if (currentBid == -1)
        currentBid = 6;
    if (currentBid == 7)
        currentBid = 0;

    while(!checkBid(currentBid, round_getMaximumBid(round))) {
        currentBid += searchPattern;
        if (currentBid == -1)
            currentBid = 6;
        if (currentBid == 7)
            currentBid = 0;
    }

    return currentBid;               
}

int round_findNextAllowedBid(struct Round *round, int currentBid)
{
    return findAllowedBid(round, currentBid, 1);
}

int round_findPreviousAllowedBid(struct Round *round, int currentBid)
{
    return findAllowedBid(round, currentBid, -1);
}

